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
#include <QtCore/QFile>
#include <QtCore/QResource>

#include <QtGui/QFontDatabase>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>

#include <QtSql/QSqlDatabase>

#include "ulog.hpp"
#include "database.hpp"

#include "config/manager.hpp"

#include "windows/about.hpp"
#include "windows/license.hpp"
#include "windows/qtinfo.hpp"
#include "windows/config.hpp"

using namespace ulog;
using namespace ulog::app;

ULog::ULog(QObject *parent) : QObject(parent) {
    status = new Status();
    config = new config::Config();

    database = new Database();

    mainWindow = new windows::Main(config);

    rigCtl = new rigctl::RigCtl();
}

ULog::~ULog() {
    delete database;
    delete mainWindow;
    delete rigCtl;
    delete config;
    delete status;
}

void ULog::entryPoint() {
    qInfo() << "Entrypoint";
    connectSignals();
    start();
}

void ULog::connectSignals() const {
    qInfo() << "Connecting signals";

    connect(mainWindow, &windows::Main::closeApplication, this, &ULog::stop, Qt::QueuedConnection);

    connect(mainWindow, &windows::Main::displayConfig, this, &ULog::displayConfig, Qt::QueuedConnection);
    connect(mainWindow, &windows::Main::displayAbout, this, &ULog::displayAbout, Qt::QueuedConnection);
    connect(mainWindow, &windows::Main::displayLicense, this, &ULog::displayLicense, Qt::QueuedConnection);
    connect(mainWindow, &windows::Main::displayQtInfo, this, &ULog::displayQtInfo, Qt::QueuedConnection);

    connect(mainWindow, &windows::Main::rigCtlStart, this, &ULog::rigCtlStart, Qt::QueuedConnection);
    connect(mainWindow, &windows::Main::rigCtlStop, this, &ULog::rigCtlStop, Qt::QueuedConnection);

    connect(mainWindow, &windows::Main::modelReady, database, &Database::readAll, Qt::QueuedConnection);

    connect(mainWindow, &windows::Main::qsoAdd, database, &Database::qsoAdd, Qt::QueuedConnection);
    connect(mainWindow, &windows::Main::qsoRemove, database, &Database::qsoRemove, Qt::QueuedConnection);

    connect(database, &Database::newQsoTableModel, mainWindow, &windows::Main::setQsoTableModel, Qt::QueuedConnection);
    connect(database, &Database::dataRead, mainWindow, &windows::Main::updateTableView, Qt::QueuedConnection);

    connect(rigCtl, &rigctl::RigCtl::connected, mainWindow, &windows::Main::rigCtlConnected, Qt::QueuedConnection);
    connect(rigCtl, &rigctl::RigCtl::disconnected, mainWindow, &windows::Main::rigCtlDisconnected,
            Qt::QueuedConnection);
    connect(rigCtl, &rigctl::RigCtl::vfoUpdate, mainWindow, &windows::Main::vfoUpdate, Qt::QueuedConnection);
}

void ULog::start() {
    qInfo() << "Start";

    qDebug() << "Init config RigCtl";
    QMetaObject::invokeMethod(this, &ULog::configLoad, Qt::DirectConnection);
    QMetaObject::invokeMethod(this, &ULog::configSave, Qt::DirectConnection);

    qDebug() << "Opening Database";
    QMetaObject::invokeMethod(database, &Database::open, Qt::QueuedConnection);

    qDebug() << "Displaying main window";
    QMetaObject::invokeMethod(mainWindow, &QWidget::show, Qt::QueuedConnection);
}

void ULog::stop() {
    qInfo() << "Stop";

    qDebug() << "Closing RigCtl";
    QMetaObject::invokeMethod(rigCtl, &rigctl::RigCtl::stop, Qt::QueuedConnection);

    qDebug() << "Closing Main Window";
    QMetaObject::invokeMethod(mainWindow, &windows::Main::close, Qt::QueuedConnection);

    qDebug() << "Closing Database";
    QMetaObject::invokeMethod(database, &Database::close, Qt::QueuedConnection);

    QMetaObject::invokeMethod(this, &ULog::finished, Qt::QueuedConnection);
}

void ULog::configLoad() {
    qInfo() << "Loading config";
    config::Manager::load(config);
}

void ULog::configSave() {
    qInfo() << "Saving config";
    config::Manager::save(config);
}

void ULog::displayConfig() {
    qInfo() << "Display Config window";

    auto *configWindow = new windows::Config(config);
    connect(configWindow, &windows::Config::configLoad, this, &ULog::configLoad);
    connect(configWindow, &windows::Config::configSave, this, &ULog::configSave);
    configWindow->exec();
    configWindow->deleteLater();
}

void ULog::displayAbout() {
    qInfo() << "Display About window";

    auto *aboutWindow = new windows::About();
    aboutWindow->exec();
    aboutWindow->deleteLater();
}

void ULog::displayLicense() {
    qInfo() << "Display License window";

    auto *licenseWindow = new windows::License();
    licenseWindow->exec();
    licenseWindow->deleteLater();
}

void ULog::displayQtInfo() {
    qInfo() << "Display Qt Info window";

    auto *qtInfoWiondow = new windows::QtInfo();
    qtInfoWiondow->exec();
    qtInfoWiondow->deleteLater();
}

void ULog::rigCtlStart() {
    qInfo() << "Starting RigCtl";

    rigCtl->getParams()->setRigModel(config->getRigCtl()->getModel());
    rigCtl->getParams()->setIcomIcv(config->getRigCtl()->getIcomCiv());
    rigCtl->getParams()->setPollingInterval(config->getRigCtl()->getPollingInterval());

    rigCtl->getParams()->setPortName(config->getRigCtl()->getSerialPort()->getName());
    rigCtl->getParams()->setPortBaudRate(config->getRigCtl()->getSerialPort()->getBaudRate());
    rigCtl->getParams()->setPortDataBits(config->getRigCtl()->getSerialPort()->getDataBits());
    rigCtl->getParams()->setPortParity(config->getRigCtl()->getSerialPort()->getParity());
    rigCtl->getParams()->setPortStopBits(config->getRigCtl()->getSerialPort()->getStopBits());
    rigCtl->getParams()->setPortFlowControl(config->getRigCtl()->getSerialPort()->getFlowControl());
    rigCtl->getParams()->setPortRtsState(config->getRigCtl()->getSerialPort()->getRtsState());
    rigCtl->getParams()->setPortDtrState(config->getRigCtl()->getSerialPort()->getDtrState());

    QMetaObject::invokeMethod(mainWindow, &windows::Main::rigCtlConnecting, Qt::QueuedConnection);
    QMetaObject::invokeMethod(rigCtl, &rigctl::RigCtl::start, Qt::QueuedConnection);
}

void ULog::rigCtlStop() {
    qInfo() << "Stopping RigCtl";

    QMetaObject::invokeMethod(mainWindow, &windows::Main::rigCtlDisconnecting, Qt::QueuedConnection);
    QMetaObject::invokeMethod(rigCtl, &rigctl::RigCtl::stop, Qt::QueuedConnection);
}
