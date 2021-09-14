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


#include <QtCore/QtGlobal>
#include <QtCore/QSettings>
#include <QtCore/QString>

#include <QtSerialPort/QSerialPort>

#include "manager.hpp"

using namespace ulog::app::config;

void Manager::load(Config *config) {
    QSettings settings;

    settings.beginGroup("db");
    config->getDatabase()->setType(settings.value("type", DATABASE_TYPE_DEFAULT).toString());
    config->getDatabase()->setFile(settings.value("file", DATABASE_FILE_DEFAULT).toString());
    config->getDatabase()->setHost(settings.value("host", DATABASE_HOST_DEFAULT).toString());
    config->getDatabase()->setPort(settings.value("port", DATABASE_PORT_DEFAULT).value<quint16>());
    config->getDatabase()->setUsername(settings.value("username", DATABASE_USERNAME_DEFAULT).toString());
    config->getDatabase()->setPassword(settings.value("password", DATABASE_PASSWORD_DEFAULT).toString());
    config->getDatabase()->setName(settings.value("name", DATABASE_NAME_DEFAULT).toString());
    settings.endGroup();

    settings.beginGroup("rigCtl");

    config->getRigCtl()->setModel(settings.value("model", RIGCTL_MODEL_DEFAULT).value<qint32>());
    config->getRigCtl()->setIcomCiv(settings.value("icomIcv", RIGCTL_ICOM_ICV_DEFAULT).value<quint8>());
    config->getRigCtl()->setPollingInterval(
            settings.value("pollingInterval", RIGCTL_POLLING_INTERVAL_DEFAULT).value<qint32>());

    settings.beginGroup("serialPort");
    config->getRigCtl()->getSerialPort()->setName(
            settings.value("name", SERIALPORT_NAME_DEFAULT).value<QString>());
    config->getRigCtl()->getSerialPort()->setBaudRate(
            settings.value("baudRate", SERIALPORT_BAUD_RATE_DEFAULT).value<QSerialPort::BaudRate>());
    config->getRigCtl()->getSerialPort()->setDataBits(
            settings.value("dataBits", SERIALPORT_DATA_BITS_DEFAULT).value<QSerialPort::DataBits>());
    config->getRigCtl()->getSerialPort()->setParity(
            settings.value("parity", SERIALPORT_PARITY_DEFAULT).value<QSerialPort::Parity>());
    config->getRigCtl()->getSerialPort()->setStopBits(
            settings.value("stopBits", SERIALPORT_STOP_BITS_DEFAULT).value<QSerialPort::StopBits>());
    config->getRigCtl()->getSerialPort()->setFlowControl(
            settings.value("flowControl", SERIALPORT_FLOW_CONTROL_DEFAULT).value<QSerialPort::FlowControl>());
    config->getRigCtl()->getSerialPort()->setRtsState(
            static_cast<TriState>(settings.value("rtsState",
                                                 static_cast<int>(SERIALPORT_RTS_STATE_DEFAULT)).value<int>()));
    config->getRigCtl()->getSerialPort()->setDtrState(
            static_cast<TriState>(settings.value("dtrState",
                                                 static_cast<int>(SERIALPORT_DTR_STATE_DEFAULT)).value<int>()));
    settings.endGroup();

    settings.endGroup();
}

void Manager::save(Config *config) {
    QSettings settings;

    settings.beginGroup("db");
    settings.setValue("type", config->getDatabase()->getType());
    settings.setValue("file", config->getDatabase()->getFile());
    settings.setValue("host", config->getDatabase()->getHost());
    settings.setValue("port", config->getDatabase()->getPort());
    settings.setValue("username", config->getDatabase()->getUsername());
    settings.setValue("password", config->getDatabase()->getPassword());
    settings.setValue("name", config->getDatabase()->getName());
    settings.endGroup();

    settings.beginGroup("rigCtl");

    settings.setValue("model", config->getRigCtl()->getModel());
    settings.setValue("icomIcv", config->getRigCtl()->getIcomCiv());
    settings.setValue("pollingInterval", config->getRigCtl()->getPollingInterval());

    settings.beginGroup("serialPort");
    settings.setValue("name", config->getRigCtl()->getSerialPort()->getName());
    settings.setValue("baudRate", config->getRigCtl()->getSerialPort()->getBaudRate());
    settings.setValue("dataBits", config->getRigCtl()->getSerialPort()->getDataBits());
    settings.setValue("parity", config->getRigCtl()->getSerialPort()->getParity());
    settings.setValue("stopBits", config->getRigCtl()->getSerialPort()->getStopBits());
    settings.setValue("flowControl", config->getRigCtl()->getSerialPort()->getFlowControl());
    settings.setValue("rtsState", static_cast<int>(config->getRigCtl()->getSerialPort()->getRtsState()));
    settings.setValue("dtrState", static_cast<int>(config->getRigCtl()->getSerialPort()->getDtrState()));
    settings.endGroup();

    settings.endGroup();
}
