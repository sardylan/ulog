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

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QComboBox>

#include <QtSql/QSqlDatabase>

#include <hamlib/riglist.h>

#include <rigctl/models.hpp>

#include "config.hpp"
#include "ui_config.h"

using namespace ulog::app;
using namespace ulog::app::windows;

Config::Config(config::Config *config, QWidget *parent) : QDialog(parent), ui(new Ui::Config) {
    ui->setupUi(this);

    Config::config = config;
    models = rigctl::models();

    connectSignals();
    initUi();
    valuesLoad();
}

Config::~Config() {
    delete ui;
}

void Config::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Config::valuesSave);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Config::close);

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &Config::valuesSave);
    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &Config::close);
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &Config::valuesLoad);

    connect(ui->dbTypeComboBox, &QComboBox::currentIndexChanged, this, &Config::updateDbFieldsVisibility);
}

void Config::initUi() {
    qInfo() << "Initalizing UI";

    qDebug() << "Initializing Rig Model Combo Box";
    initDbTypeComboBox();

    initRigModelComboBox();
    initRigSerialPortComboBoxes();
}

void Config::initDbTypeComboBox() {
    ui->dbTypeComboBox->clear();

    const QStringList &sqlDrivers = QSqlDatabase::drivers();

    for (const QString &sqlDriver: sqlDrivers) {
        QString text;

        if (sqlDriver == "QDB2")
            text = "IBM DB2";
        else if (sqlDriver == "QIBASE")
            text = "Borland InterBase";
        else if (sqlDriver == "QMYSQL")
            text = "MySQL";
        else if (sqlDriver == "MARIADB")
            text = "MariaDB";
        else if (sqlDriver == "QOCI")
            text = "Oracle Call Interface Driver";
        else if (sqlDriver == "QODBC")
            text = "Open Database Connectivity (ODBC)";
        else if (sqlDriver == "QPSQL")
            text = "PostgreSQL";
        else if (sqlDriver == "QSQLITE2")
            text = "SQLite 2";
        else if (sqlDriver == "QSQLITE")
            text = "SQLite";
        else if (sqlDriver == "QTDS")
            text = "Sybase Adaptive Server";

        if (text.isEmpty())
            continue;

        ui->dbTypeComboBox->addItem(text, sqlDriver);
    }
}

void Config::updateDbFieldsVisibility() {
    bool dbIsFile = false;

    if (ui->dbTypeComboBox->currentData().toString() == "QSQLITE" ||
        ui->dbTypeComboBox->currentData().toString() == "QSQLITE2")
        dbIsFile = true;

    ui->dbFileLayout->setEnabled(dbIsFile);
    ui->dbFileLabel->setEnabled(dbIsFile);
    ui->dbFileValue->setEnabled(dbIsFile);
    ui->dbFileButton->setEnabled(dbIsFile);

    ui->dbHostLabel->setEnabled(!dbIsFile);
    ui->dbHostValue->setEnabled(!dbIsFile);
    ui->dbPortLabel->setEnabled(!dbIsFile);
    ui->dbPortValue->setEnabled(!dbIsFile);
    ui->dbUsernameLabel->setEnabled(!dbIsFile);
    ui->dbUsernameValue->setEnabled(!dbIsFile);
    ui->dbPasswordLabel->setEnabled(!dbIsFile);
    ui->dbPasswordValue->setEnabled(!dbIsFile);
    ui->dbNameLabel->setEnabled(!dbIsFile);
    ui->dbNameValue->setEnabled(!dbIsFile);
    ui->dbTestButton->setEnabled(!dbIsFile);
}

void Config::initRigModelComboBox() {
    ui->rigctlSerialPortModelComboBox->clear();
    for (auto i = models.keyBegin(); i != models.keyEnd(); i++)
        addRigModel(ui->rigctlSerialPortModelComboBox, *i);
}

void Config::initRigSerialPortComboBoxes() {
    qInfo() << "Initializing serial port widgets";

    qDebug() << "initializing portName ComboBox";
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator portIterator;
    ui->rigctlSerialPortNameComboBox->clear();
    for (portIterator = portList.begin(); portIterator != portList.end(); portIterator++) {
        QSerialPortInfo serialPortInfo = *portIterator;
        const QString &systemLocation = serialPortInfo.systemLocation();
        ui->rigctlSerialPortNameComboBox->addItem(systemLocation, systemLocation);
    }

    qDebug() << "initializing baudRate ComboBox";
    ui->rigctlSerialPortBaudRateComboBox->clear();
    ui->rigctlSerialPortBaudRateComboBox->addItem("1200 baud", QSerialPort::Baud1200);
    ui->rigctlSerialPortBaudRateComboBox->addItem("2400 baud", QSerialPort::Baud2400);
    ui->rigctlSerialPortBaudRateComboBox->addItem("4800 baud", QSerialPort::Baud4800);
    ui->rigctlSerialPortBaudRateComboBox->addItem("9600 baud", QSerialPort::Baud9600);
    ui->rigctlSerialPortBaudRateComboBox->addItem("19200 baud", QSerialPort::Baud19200);
    ui->rigctlSerialPortBaudRateComboBox->addItem("38400 baud", QSerialPort::Baud38400);
    ui->rigctlSerialPortBaudRateComboBox->addItem("57600 baud", QSerialPort::Baud57600);
    ui->rigctlSerialPortBaudRateComboBox->addItem("115200 baud", QSerialPort::Baud115200);

    qDebug() << "initializing dataBits ComboBox";
    ui->rigctlSerialPortDataBitsComboBox->clear();
    ui->rigctlSerialPortDataBitsComboBox->addItem("5 bits", QSerialPort::Data5);
    ui->rigctlSerialPortDataBitsComboBox->addItem("6 bits", QSerialPort::Data6);
    ui->rigctlSerialPortDataBitsComboBox->addItem("7 bits", QSerialPort::Data7);
    ui->rigctlSerialPortDataBitsComboBox->addItem("8 bits", QSerialPort::Data8);

    qDebug() << "initializing parity ComboBox";
    ui->rigctlSerialPortParityComboBox->clear();
    ui->rigctlSerialPortParityComboBox->addItem("None", QSerialPort::NoParity);
    ui->rigctlSerialPortParityComboBox->addItem("Even", QSerialPort::EvenParity);
    ui->rigctlSerialPortParityComboBox->addItem("Odd", QSerialPort::OddParity);
    ui->rigctlSerialPortParityComboBox->addItem("Space", QSerialPort::SpaceParity);
    ui->rigctlSerialPortParityComboBox->addItem("Mark", QSerialPort::MarkParity);

    qDebug() << "initializing stopBits ComboBox";
    ui->rigctlSerialPortStopBitsComboBox->clear();
    ui->rigctlSerialPortStopBitsComboBox->addItem("1 bit", QSerialPort::OneStop);
    ui->rigctlSerialPortStopBitsComboBox->addItem("1.5 bits", QSerialPort::OneAndHalfStop);
    ui->rigctlSerialPortStopBitsComboBox->addItem("2 bits", QSerialPort::TwoStop);

    qDebug() << "initializing flowControl ComboBox";
    ui->rigctlSerialPortFlowControlComboBox->clear();
    ui->rigctlSerialPortFlowControlComboBox->addItem("None", QSerialPort::NoFlowControl);
    ui->rigctlSerialPortFlowControlComboBox->addItem("Hardware (RTS-CTS)", QSerialPort::HardwareControl);
    ui->rigctlSerialPortFlowControlComboBox->addItem("Software (XON-XOFF)", QSerialPort::SoftwareControl);
}

void Config::addRigModel(QComboBox *comboBox, qlonglong number) {
    QString name = models.value(number);

    if (name.length() > 0) {
        QString text = QString("%2 [%3]").arg(name).arg(number);
        comboBox->addItem(text, number);
    }
}

void Config::valuesLoad() {
    qInfo() << "Loading values";

    qDebug() << "Loading configuration";
    QMetaObject::invokeMethod(this, &Config::configLoad);

    qDebug() << "Setting initial values for DB";
    auto dbType = config->getDatabase()->getType();
    for (int i = 0; i < ui->dbTypeComboBox->count(); i++)
        if (ui->dbTypeComboBox->itemData(i).toString() == dbType) {
            ui->dbTypeComboBox->setCurrentIndex(i);
            break;
        }

    ui->dbFileValue->setText(config->getDatabase()->getFile());
    ui->dbHostValue->setText(config->getDatabase()->getHost());
    ui->dbPortValue->setText(QString::number(config->getDatabase()->getPort()));
    ui->dbUsernameValue->setText(config->getDatabase()->getUsername());
    ui->dbPasswordValue->setText(config->getDatabase()->getPassword());
    ui->dbNameValue->setText(config->getDatabase()->getFile());

    qDebug() << "Updating db fields";
    QMetaObject::invokeMethod(this, &Config::updateDbFieldsVisibility, Qt::QueuedConnection);

    qDebug() << "Selecting initial value for HamLib";
    auto rigCtlModel = config->getRigCtl()->getModel();
    for (int i = 0; i < ui->rigctlSerialPortModelComboBox->count(); i++)
        if (ui->rigctlSerialPortModelComboBox->itemData(i).value<QSerialPort::BaudRate>() == rigCtlModel) {
            ui->rigctlSerialPortModelComboBox->setCurrentIndex(i);
            break;
        }

    ui->rigctlSerialPortPollingIntervalSpinBox->setValue(config->getRigCtl()->getPollingInterval());
    ui->rigctlSerialPortCIVAddressValue->setText(QString::number(config->getRigCtl()->getIcomCiv(), 16));

    auto &rigCtlSerialPortName = const_cast<QString &>(config->getRigCtl()->getSerialPort()->getName());
    int index = -1;
    for (int i = 0; i < ui->rigctlSerialPortNameComboBox->count(); i++)
        if (ui->rigctlSerialPortNameComboBox->itemData(i).toString() == rigCtlSerialPortName) {
            index = i;
            break;
        }
    if (index != -1) {
        ui->rigctlSerialPortNameComboBox->setCurrentIndex(index);
    } else {
        ui->rigctlSerialPortNameComboBox->setItemText(0, rigCtlSerialPortName);
        ui->rigctlSerialPortNameComboBox->setItemData(0, rigCtlSerialPortName);
        ui->rigctlSerialPortNameComboBox->setCurrentIndex(0);
    }

    auto rigCtlSerialPortBaudRate = config->getRigCtl()->getSerialPort()->getBaudRate();
    for (int i = 0; i < ui->rigctlSerialPortBaudRateComboBox->count(); i++)
        if (ui->rigctlSerialPortBaudRateComboBox->itemData(i).value<QSerialPort::BaudRate>() ==
            rigCtlSerialPortBaudRate) {
            ui->rigctlSerialPortBaudRateComboBox->setCurrentIndex(i);
            break;
        }

    auto rigCtlSerialPortDataBits = config->getRigCtl()->getSerialPort()->getDataBits();
    for (int i = 0; i < ui->rigctlSerialPortDataBitsComboBox->count(); i++)
        if (ui->rigctlSerialPortDataBitsComboBox->itemData(i).value<QSerialPort::DataBits>() ==
            rigCtlSerialPortDataBits) {
            ui->rigctlSerialPortDataBitsComboBox->setCurrentIndex(i);
            break;
        }

    auto rigCtlSerialPortParity = config->getRigCtl()->getSerialPort()->getParity();
    for (int i = 0; i < ui->rigctlSerialPortParityComboBox->count(); i++)
        if (ui->rigctlSerialPortParityComboBox->itemData(i).value<QSerialPort::Parity>() == rigCtlSerialPortParity) {
            ui->rigctlSerialPortParityComboBox->setCurrentIndex(i);
            break;
        }

    auto rigCtlSerialPortStopBits = config->getRigCtl()->getSerialPort()->getStopBits();
    for (int i = 0; i < ui->rigctlSerialPortStopBitsComboBox->count(); i++)
        if (ui->rigctlSerialPortStopBitsComboBox->itemData(i).value<QSerialPort::StopBits>() ==
            rigCtlSerialPortStopBits) {
            ui->rigctlSerialPortStopBitsComboBox->setCurrentIndex(i);
            break;
        }

    auto rigCtlSerialPortFlowControl = config->getRigCtl()->getSerialPort()->getFlowControl();
    for (int i = 0; i < ui->rigctlSerialPortFlowControlComboBox->count(); i++)
        if (ui->rigctlSerialPortFlowControlComboBox->itemData(i).value<QSerialPort::FlowControl>() ==
            rigCtlSerialPortFlowControl) {
            ui->rigctlSerialPortFlowControlComboBox->setCurrentIndex(i);
            break;
        }
}

void Config::valuesSave() {
    qInfo() << "Saving values";

    qDebug() << "Saving DB params";
    config->getDatabase()->setType(ui->dbTypeComboBox->currentData().toString());
    config->getDatabase()->setFile(ui->dbFileValue->text());
    config->getDatabase()->setHost(ui->dbHostValue->text());
    config->getDatabase()->setPort(ui->dbPortValue->text().toUShort());
    config->getDatabase()->setUsername(ui->dbUsernameValue->text());
    config->getDatabase()->setPassword(ui->dbPasswordValue->text());
    config->getDatabase()->setName(ui->dbNameValue->text());

    qDebug() << "Saving HamLib params";
    config->getRigCtl()->setModel(ui->rigctlSerialPortModelComboBox->currentData().value<qint32>());
    config->getRigCtl()->setPollingInterval(ui->rigctlSerialPortPollingIntervalSpinBox->value());
    config->getRigCtl()->setIcomCiv(ui->rigctlSerialPortCIVAddressValue->text().toUShort(nullptr, 16));
    config->getRigCtl()->getSerialPort()->setName(
            ui->rigctlSerialPortNameComboBox->currentData().value<QString>());
    config->getRigCtl()->getSerialPort()->setBaudRate(
            ui->rigctlSerialPortBaudRateComboBox->currentData().value<QSerialPort::BaudRate>());
    config->getRigCtl()->getSerialPort()->setDataBits(
            ui->rigctlSerialPortDataBitsComboBox->currentData().value<QSerialPort::DataBits>());
    config->getRigCtl()->getSerialPort()->setParity(
            ui->rigctlSerialPortParityComboBox->currentData().value<QSerialPort::Parity>());
    config->getRigCtl()->getSerialPort()->setStopBits(
            ui->rigctlSerialPortStopBitsComboBox->currentData().value<QSerialPort::StopBits>());
    config->getRigCtl()->getSerialPort()->setFlowControl(
            ui->rigctlSerialPortFlowControlComboBox->currentData().value<QSerialPort::FlowControl>());

    qDebug() << "Saving configuration";
    QMetaObject::invokeMethod(this, &Config::configSave, Qt::QueuedConnection);
}
