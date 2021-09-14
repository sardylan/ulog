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


#include "serialport.hpp"

using namespace ulog::app::config;

SerialPort::SerialPort() {
    SerialPort::name = SERIALPORT_NAME_DEFAULT;
    SerialPort::baudRate = SERIALPORT_BAUD_RATE_DEFAULT;
    SerialPort::dataBits = SERIALPORT_DATA_BITS_DEFAULT;
    SerialPort::parity = SERIALPORT_PARITY_DEFAULT;
    SerialPort::stopBits = SERIALPORT_STOP_BITS_DEFAULT;
    SerialPort::flowControl = SERIALPORT_FLOW_CONTROL_DEFAULT;
    SerialPort::rtsState = SERIALPORT_RTS_STATE_DEFAULT;
    SerialPort::dtrState = SERIALPORT_DTR_STATE_DEFAULT;
}

SerialPort::~SerialPort() = default;

const QString &SerialPort::getName() const {
    return name;
}

void SerialPort::setName(const QString &newValue) {
    SerialPort::name = newValue;
}

QSerialPort::BaudRate SerialPort::getBaudRate() const {
    return baudRate;
}

void SerialPort::setBaudRate(QSerialPort::BaudRate newValue) {
    SerialPort::baudRate = newValue;
}

QSerialPort::DataBits SerialPort::getDataBits() const {
    return dataBits;
}

void SerialPort::setDataBits(QSerialPort::DataBits newValue) {
    SerialPort::dataBits = newValue;
}

QSerialPort::Parity SerialPort::getParity() const {
    return parity;
}

void SerialPort::setParity(QSerialPort::Parity newValue) {
    SerialPort::parity = newValue;
}

QSerialPort::StopBits SerialPort::getStopBits() const {
    return stopBits;
}

void SerialPort::setStopBits(QSerialPort::StopBits newValue) {
    SerialPort::stopBits = newValue;
}

QSerialPort::FlowControl SerialPort::getFlowControl() const {
    return flowControl;
}

void SerialPort::setFlowControl(QSerialPort::FlowControl newValue) {
    SerialPort::flowControl = newValue;
}

TriState SerialPort::getRtsState() const {
    return rtsState;
}

void SerialPort::setRtsState(TriState newValue) {
    SerialPort::rtsState = newValue;
}

TriState SerialPort::getDtrState() const {
    return dtrState;
}

void SerialPort::setDtrState(TriState newValue) {
    SerialPort::dtrState = newValue;
}
