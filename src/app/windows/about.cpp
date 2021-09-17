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
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <QtCore/QRect>

#include <QtGui/QPixmap>

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QAction>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include <hamlib/rig.h>

#include <utilities/resources.hpp>

#include "about.hpp"
#include "ui_about.h"

using namespace ulog::app::windows;

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About) {
    ui->setupUi(this);

    counter = 0;
    gridLayout = new QGridLayout();

    initUi();
    connectSignals();
}

About::~About() {
    delete gridLayout;
    delete ui;
}

void About::initUi() {
    qInfo() << "Initializing UI";

    qDebug() << "Setting title";
    QFont titleFont = ui->titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(18);
    ui->titleLabel->setFont(titleFont);

    const QString &title = QString("%1\n%2").arg(
            QCoreApplication::applicationName(),
            QCoreApplication::applicationVersion()
    );
    ui->titleLabel->setText(title);

    qDebug() << "Setting logo";
    QByteArray data = utilities::Resources::readFile(":/images/logos/application");
    QPixmap pixmap;
    pixmap.loadFromData(data);
    const QPixmap &scaled = pixmap.scaledToHeight(64, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(scaled);

    qDebug() << "Setting versions";
    ui->versionsWidget->setLayout(gridLayout);
    addParamValue("Qt Version", QT_VERSION_STR);
    addParamValue("HamLib Version", hamlib_version);

    qDebug() << "Setting text";
    data = utilities::Resources::readFile(":/text/about/markdown");
    ui->textLabel->setTextFormat(Qt::MarkdownText);
    ui->textLabel->setAlignment(Qt::AlignJustify);
    ui->textLabel->setText(QString(data));
    ui->textLabel->setWordWrap(true);
}

void About::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QAbstractButton::clicked, this, &About::close);
}

void About::addParamValue(const QString &param, const QString &value) {
    auto *paramLabel = new QLabel(this);
    auto *valueLabel = new QLabel(this);

    paramLabel->setAlignment(Qt::AlignLeft);
    valueLabel->setAlignment(Qt::AlignRight);

    paramLabel->setText(param);
    valueLabel->setText(value);

    gridLayout->addWidget(paramLabel, counter, 0);
    gridLayout->addWidget(valueLabel, counter, 1);

    counter++;
}
