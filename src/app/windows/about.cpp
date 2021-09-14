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

#include "about.hpp"
#include "ui_about.h"

using namespace ulog::app::windows;

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About) {
    ui->setupUi(this);

    connectSignals();
    initUi();
}

About::~About() {
    delete ui;
}

void About::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QAbstractButton::clicked, this, &About::close);
}

void About::initUi() {
    qInfo() << "Initalizing UI";

    QFont titleFont = ui->titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(18);
    ui->titleLabel->setFont(titleFont);
    ui->titleLabel->setText(QString("%1\n%2")
                                    .arg(QCoreApplication::applicationName())
                                    .arg(QCoreApplication::applicationVersion()));

    QByteArray data = readFile(":/icons/application");
    QPixmap pixmap;
    pixmap.loadFromData(data);
    const QPixmap &cropped = pixmap.copy(QRect(0, 256, 1024, 512));
    const QPixmap &scaled = cropped.scaledToHeight(64, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(scaled);

    data = readFile(":/files/text/about");
    ui->textLabel->setTextFormat(Qt::RichText);
    ui->textLabel->setAlignment(Qt::AlignJustify);
    ui->textLabel->setText(QString(data));
    ui->textLabel->setWordWrap(true);
}

QByteArray About::readFile(const QString &path) {
    qInfo() << "Reading file" << path;

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    return data;
}
