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


#include "params.hpp"

using namespace ulog::rigctl;

Params::Params() {
    rigModel = RIGCTL_RIG_MODEL_DEFAULT;
    icomIcv = RIGCTL_ICOM_ICV_DEFAULT;
    pollingInterval = RIGCTL_POLLING_INTERVAL_DEFAULT;

    portName = RIGCTL_PORT_NAME_DEFAULT;
    portBaudRate = RIGCTL_PORT_BAUD_RATE_DEFAULT;
    portDataBits = RIGCTL_PORT_DATA_BITS_DEFAULT;
    portParity = RIGCTL_PORT_PARITY_DEFAULT;
    portStopBits = RIGCTL_PORT_STOP_BITS_DEFAULT;
    portFlowControl = RIGCTL_PORT_FLOW_CONTROL_DEFAULT;
    portRtsState = RIGCTL_RTS_STATE_DEFAULT;
    portDtrState = RIGCTL_DTR_STATE_DEFAULT;
}

Params::~Params() = default;

rig_model_t Params::getRigModel() const {
    return rigModel;
}

void Params::setRigModel(rig_model_t newValue) {
    Params::rigModel = newValue;
}

quint8 Params::getIcomIcv() const {
    return icomIcv;
}

void Params::setIcomIcv(quint8 newValue) {
    Params::icomIcv = newValue;
}

int Params::getPollingInterval() const {
    return pollingInterval;
}

void Params::setPollingInterval(int newValue) {
    Params::pollingInterval = newValue;
}

const QString &Params::getPortName() const {
    return portName;
}

void Params::setPortName(const QString &newValue) {
    Params::portName = newValue;
}

QSerialPort::BaudRate Params::getPortBaudRate() const {
    return portBaudRate;
}

void Params::setPortBaudRate(QSerialPort::BaudRate newValue) {
    Params::portBaudRate = newValue;
}

QSerialPort::DataBits Params::getPortDataBits() const {
    return portDataBits;
}

void Params::setPortDataBits(QSerialPort::DataBits newValue) {
    Params::portDataBits = newValue;
}

QSerialPort::Parity Params::getPortParity() const {
    return portParity;
}

void Params::setPortParity(QSerialPort::Parity newValue) {
    Params::portParity = newValue;
}

QSerialPort::StopBits Params::getPortStopBits() const {
    return portStopBits;
}

void Params::setPortStopBits(QSerialPort::StopBits newValue) {
    Params::portStopBits = newValue;
}

QSerialPort::FlowControl Params::getPortFlowControl() const {
    return portFlowControl;
}

void Params::setPortFlowControl(QSerialPort::FlowControl newValue) {
    Params::portFlowControl = newValue;
}

enums::TriState Params::getPortRtsState() const {
    return portRtsState;
}

void Params::setPortRtsState(enums::TriState newValue) {
    Params::portRtsState = newValue;
}

enums::TriState Params::getPortDtrState() const {
    return portDtrState;
}

void Params::setPortDtrState(enums::TriState newValue) {
    Params::portDtrState = newValue;
}
