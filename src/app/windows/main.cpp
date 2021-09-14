/*
 * uLog
 * Copyright (C) 2021  Luca Cireddu - IS0GVH
 * sardylan@gmail.com - is0gvh@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QDateTime>
#include <QtCore/QItemSelectionModel>
#include <QtCore/QModelIndexList>

#include <QtGui/QShortcutEvent>

#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>

#include <delegators/datetime.hpp>
#include <delegators/string.hpp>

#include <models/qso/tablemodel.hpp>

#include <utilities/serialport.hpp>

#include <rigctl/models.hpp>

#include "main.hpp"
#include "ui_main.h"
#include "database.hpp"

using namespace ulog;
using namespace ulog::delegators;
using namespace ulog::app::windows;

Main::Main(config::Config *config, QWidget *parent) : QMainWindow(parent), ui(new Ui::Main) {
    ui->setupUi(this);

    Main::config = config;

    clockTimer = new QTimer(this);

    statusBarRigCtlStatus = new QLabel();
    statusBarRigCtlPollingInterval = new QLabel();
    statusBarRigCtlModel = new QLabel();
    statusBarRigCtlSerialPort = new QLabel();

    pttStatus.clear();

    focusColumn = 0;

    initUi();
    initStatusBar();
    connectSignals();

    startUi();
}

Main::~Main() {
    delete statusBarRigCtlSerialPort;
    delete statusBarRigCtlModel;
    delete statusBarRigCtlPollingInterval;
    delete statusBarRigCtlStatus;

    delete ui;
}

void Main::initUi() {
    qInfo() << "Initalizing UI";

#if QT_CONFIG(shortcut)
    qDebug() << "Setting Keyboard shortcuts";
    ui->qsoAddAction->setShortcut(QCoreApplication::translate("Main", "F1", nullptr));
    ui->qsoRemoveAction->setShortcut(QCoreApplication::translate("Main", "F4", nullptr));
#endif

    qDebug() << "Initializing Clock timer";
    clockTimer->setInterval(1000);
    clockTimer->setSingleShot(false);
    connect(clockTimer, &QTimer::timeout, this, &Main::updateClocks, Qt::QueuedConnection);
    clockTimer->start();

    qDebug() << "Setting Size Policy for CAT Widgets";
    setWidgetSizePolicy(ui->vfoALabel);
    setWidgetSizePolicy(ui->vfoAFrequency);
    setWidgetSizePolicy(ui->vfoAMode);
    setWidgetSizePolicy(ui->vfoARxIcon);
    setWidgetSizePolicy(ui->vfoATxIcon);
    setWidgetSizePolicy(ui->vfoAStrengthProgressBar);
    setWidgetSizePolicy(ui->vfoAStrengthValue);

    setWidgetSizePolicy(ui->vfoBLabel);
    setWidgetSizePolicy(ui->vfoBFrequency);
    setWidgetSizePolicy(ui->vfoBMode);
    setWidgetSizePolicy(ui->vfoBRxIcon);
    setWidgetSizePolicy(ui->vfoBTxIcon);
    setWidgetSizePolicy(ui->vfoBStrengthProgressBar);
    setWidgetSizePolicy(ui->vfoBStrengthValue);

    setWidgetSizePolicy(ui->onAirIcon);
}

void Main::initStatusBar() {
    qInfo() << "Initalizing Status Bar";

    auto *label = new QLabel(this);
    label->setText(QString("%1:").arg("CAT Status"));
    ui->statusBar->addPermanentWidget(label);

    addStatusBarPermanentWidget(statusBarRigCtlStatus);
    addStatusBarPermanentWidget(statusBarRigCtlPollingInterval);
    addStatusBarPermanentWidget(statusBarRigCtlModel);
    addStatusBarPermanentWidget(statusBarRigCtlSerialPort);
}

void Main::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->exitAction, &QAction::triggered, this, &QMainWindow::close, Qt::QueuedConnection);

    connect(ui->showSettingsAction, &QAction::triggered, this, &Main::displayConfig, Qt::QueuedConnection);

    connect(ui->toggleCATAction, &QAction::triggered, this, [this](bool checked) {
        if (checked)
            QMetaObject::invokeMethod(this, &Main::rigCtlStart, Qt::QueuedConnection);
        else
            QMetaObject::invokeMethod(this, &Main::rigCtlStop, Qt::QueuedConnection);
    });

    connect(ui->showAboutAction, &QAction::triggered, this, &Main::displayAbout, Qt::QueuedConnection);
    connect(ui->showLicenseAction, &QAction::triggered, this, &Main::displayLicense, Qt::QueuedConnection);
    connect(ui->showQtInfoAction, &QAction::triggered, this, &Main::displayQtInfo, Qt::QueuedConnection);

    connect(ui->qsoAddAction, &QAction::triggered, this, &Main::qsoAdd, Qt::QueuedConnection);
    connect(ui->qsoRemoveAction, &QAction::triggered, this, &Main::qsoRemoveConfirm, Qt::QueuedConnection);
}

void Main::updateWindowsTitle() {
    auto title = QString("%1 %2").arg(QCoreApplication::applicationName(), QCoreApplication::applicationVersion());
    setWindowTitle(title);
}

void Main::updateClocks() {
    ui->utcClockValue->setText(QDateTime::currentDateTimeUtc().toString(DATETIME_FORMAT));
    ui->localClockValue->setText(QDateTime::currentDateTime().toString(DATETIME_FORMAT));
}

void Main::startUi() {
    QMetaObject::invokeMethod(this, &Main::updateWindowsTitle, Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, &Main::updateClocks, Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, &Main::updateCatWidgets, Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, &Main::rigCtlDisconnected, Qt::QueuedConnection);
}

void Main::setQsoTableModel(QSqlTableModel *qsoTableModel) {
    qInfo() << "Set QSO table Model";

    ui->qsoTableView->setModel(qsoTableModel);

    int columnCount = qsoTableModel->columnCount();
    for (int i = 0; i < columnCount; i++) {
        bool columnVisible = qsoTableModel->headerData(i, Qt::Horizontal, DATABASE_DATA_ROLE_VISIBLE).toBool();

        ui->qsoTableView->setColumnHidden(i, !columnVisible);

        if (columnVisible) {
            QString columnType = qsoTableModel->headerData(i, Qt::Horizontal, DATABASE_DATA_ROLE_TYPE).toString();
            bool columnIsFocus = qsoTableModel->headerData(i, Qt::Horizontal, DATABASE_DATA_ROLE_FOCUS).toBool();

            QAbstractItemDelegate *itemDelegate = nullptr;

            if (columnType == "QString")
                itemDelegate = new delegators::StringDelegate(this);
            else if (columnType == "QDateTime")
                itemDelegate = new delegators::DateTimeDelegate(this);

            ui->qsoTableView->setItemDelegateForColumn(i, itemDelegate);

            if (columnIsFocus) {
                qDebug() << "Focus column for new records:" << i;
                focusColumn = i;
            }
        }
    }

    connect(ui->qsoTableView->model(), &QAbstractItemModel::rowsInserted, this, &Main::focusOnLastQso,
            Qt::QueuedConnection);

    QMetaObject::invokeMethod(this, &Main::modelReady, Qt::QueuedConnection);
}

void Main::rigCtlConnecting() {
    ui->toggleCATAction->setChecked(true);
    ui->toggleCATAction->setEnabled(false);
    statusBarRigCtlStatus->setText(tr("connecting..."));
}

void Main::rigCtlConnected(int vfoCount) {
    ui->toggleCATAction->setChecked(true);
    ui->toggleCATAction->setEnabled(true);
    statusBarRigCtlStatus->setText(tr("connected"));
    setRigCtlFieldsVisibility(true, vfoCount);
}

void Main::rigCtlDisconnecting() {
    ui->toggleCATAction->setChecked(false);
    ui->toggleCATAction->setEnabled(false);
    statusBarRigCtlStatus->setText(tr("disconnecting..."));
}

void Main::rigCtlDisconnected() {
    ui->toggleCATAction->setChecked(false);
    ui->toggleCATAction->setEnabled(true);
    statusBarRigCtlStatus->setText(tr("disconnected"));
    setRigCtlFieldsVisibility(false);
}

void Main::vfoUpdate(const ulog::rigctl::Update &update) {
    const QString &frequencyString = formatFrequency(update.getFrequency());

    switch (update.getVfoNum()) {

        case 1:
            ui->vfoAFrequency->setText(frequencyString);
            updateModeLabel(ui->vfoAMode, update.getMode());
            updateStrength(update.getVfoNum(), update.getStrength());
            break;

        case 2:
            ui->vfoBFrequency->setText(frequencyString);
            updateModeLabel(ui->vfoBMode, update.getMode());
            break;

        default:
            break;
    }

    pttStatus.insert(update.getVfoNum(), update.isPtt());

    bool onAirVisible = false;
    for (bool item: pttStatus)
        if (item) {
            onAirVisible = true;
            break;
        }
    ui->onAirIcon->setVisible(onAirVisible);
}

void Main::closeEvent(QCloseEvent *event) {
    QMetaObject::invokeMethod(this, &Main::closeApplication, Qt::QueuedConnection);
    QWidget::closeEvent(event);
}

QString Main::formatFrequency(quint64 frequency) {
    auto frequencyString = QString("%1").arg(frequency);
    int length = frequencyString.length();

    QString value;
    for (int i = 0; i < length; i++) {
        if (i > 0 && i % 3 == 0) {
            value = "." + value;
        }

        int pos = length - i - 1;
        value = frequencyString[pos] + value;
    }

    return value;
}

void Main::updateCatWidgets() {
    auto text = utilities::SerialPort::prettyConfig(config->getRigCtl()->getSerialPort()->getName(),
                                                    config->getRigCtl()->getSerialPort()->getBaudRate(),
                                                    config->getRigCtl()->getSerialPort()->getDataBits(),
                                                    config->getRigCtl()->getSerialPort()->getParity(),
                                                    config->getRigCtl()->getSerialPort()->getStopBits(),
                                                    config->getRigCtl()->getSerialPort()->getFlowControl());
    statusBarRigCtlSerialPort->setText(text);

    text = QString("%1 ms poll").arg(config->getRigCtl()->getPollingInterval());
    statusBarRigCtlPollingInterval->setText(text);

    text = rigctl::models().value(config->getRigCtl()->getModel());
    statusBarRigCtlModel->setText(text);
}

void Main::addStatusBarPermanentWidget(QLabel *widget) {
    widget->setFrameShape(QFrame::Panel);
    widget->setStyleSheet("padding: 0 5px;");
    ui->statusBar->addPermanentWidget(widget);
}

void Main::setRigCtlFieldsVisibility(bool visible, int vfoCount) {
    if (visible) {
        if (vfoCount >= 1) {
            ui->vfoALabel->setVisible(true);
            ui->vfoAFrequency->setVisible(true);
            ui->vfoAMode->setVisible(true);
            ui->vfoAStrengthProgressBar->setVisible(true);
            ui->vfoAStrengthValue->setVisible(true);
        }

        if (vfoCount == 1) {
            ui->vfoARxIcon->setVisible(true);
            ui->vfoATxIcon->setVisible(true);
        }

        if (vfoCount >= 2) {
            ui->vfoBLabel->setVisible(true);
            ui->vfoBFrequency->setVisible(true);
            ui->vfoBMode->setVisible(true);
            ui->vfoBStrengthProgressBar->setVisible(true);
            ui->vfoBStrengthValue->setVisible(true);
        }
    } else {
        pttStatus.clear();

        ui->vfoALabel->setVisible(false);
        ui->vfoAFrequency->setVisible(false);
        ui->vfoAMode->setVisible(false);
        ui->vfoAStrengthProgressBar->setVisible(false);
        ui->vfoAStrengthValue->setVisible(false);

        ui->vfoBLabel->setVisible(false);
        ui->vfoBFrequency->setVisible(false);
        ui->vfoBMode->setVisible(false);
        ui->vfoBStrengthProgressBar->setVisible(false);
        ui->vfoBStrengthValue->setVisible(false);

        ui->vfoARxIcon->setVisible(false);
        ui->vfoATxIcon->setVisible(false);

        ui->vfoBRxIcon->setVisible(false);
        ui->vfoBTxIcon->setVisible(false);

        ui->onAirIcon->setVisible(false);
    }
}

void Main::updateStrength(int vfoNum, int strength) {
    QString strengthString;

    int sValue = (54 + strength) / 6;

    if (sValue <= 9)
        strengthString = QString::number(sValue);
    else
        strengthString = QString("9+%1").arg(strength);

    if (vfoNum == 1) {
        ui->vfoAStrengthProgressBar->setValue(strength);
        ui->vfoAStrengthValue->setText(QString("S: %1").arg(strengthString));
    }
}

void Main::setWidgetSizePolicy(QWidget *widget) {
    QSizePolicy sizePolicy = widget->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    widget->setSizePolicy(sizePolicy);
}

void Main::updateModeLabel(QLabel *label, ulog::rigctl::Mode mode) {
    label->setText(modeToString(mode));
}

void Main::updateTableView() {
    ui->qsoTableView->resizeColumnsToContents();
}

void Main::qsoRemoveConfirm() {
    qInfo() << "Asking for confirmation before removing QSO";

    QItemSelectionModel *selectionModel = ui->qsoTableView->selectionModel();
    const QModelIndexList &indexList = selectionModel->selectedRows();
    int selectedRows = indexList.size();
    if (!selectionModel->hasSelection() || selectedRows == 0) {
        QMessageBox::information(this, tr("QSO Remove"), tr("No QSO selected"));
        return;
    }

    QString text;
    if (selectedRows > 1)
        text = tr("Are you sure to remove selected QSO?");
    else
        text = tr("Are you sure to remove selected QSOs?");


    QMessageBox::StandardButton clickedButton = QMessageBox::warning(
            this,
            tr("Confirming QSO removal"),
            text,
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
    );

    if (clickedButton == QMessageBox::No)
        return;

    for (const QModelIndex &index: indexList) {
        int rowNumber = index.row();
        QMetaObject::invokeMethod(this, "qsoRemove", Qt::QueuedConnection, Q_ARG(int, rowNumber));
    }
}

void Main::focusOnLastQso() {
    qInfo() << "Setting focus on last QSO record";

    int lastRow = ui->qsoTableView->model()->rowCount() - 1;
    const QModelIndex &modelIndex = ui->qsoTableView->model()->index(lastRow, focusColumn);
    QItemSelectionModel *selectionModel = ui->qsoTableView->selectionModel();
    QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::Select;
    selectionModel->select(modelIndex, flags);

    ui->qsoTableView->setFocus();
    ui->qsoTableView->edit(modelIndex);
}
