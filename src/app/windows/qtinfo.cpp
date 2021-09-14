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

#include <QtWidgets/QAction>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>

#include <hamlib/rig.h>

#include "qtinfo.hpp"
#include "ui_qtinfo.h"

using namespace ulog::app::windows;

QtInfo::QtInfo(QWidget *parent) : QDialog(parent), ui(new Ui::QtInfo) {
    ui->setupUi(this);

    gridLayout = new QGridLayout();
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    counter = 0;

    connectSignals();
    initUi();
}

QtInfo::~QtInfo() {
    delete gridLayout;
    delete verticalSpacer;
    delete ui;
}

void QtInfo::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QAbstractButton::clicked, this, &QtInfo::close);
}

void QtInfo::initUi() {
    qInfo() << "Initalizing UI";

    ui->groupBox->setLayout(gridLayout);

    addParamValue("Qt Version", QT_VERSION_STR);
    addParamValue("HamLib Version", hamlib_version);

    gridLayout->addItem(verticalSpacer, counter, 0);
}

void QtInfo::addParamValue(const QString &param, const QString &value) {
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
