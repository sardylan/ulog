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


#include "rigctl.hpp"

using namespace ulog::app::config;

RigCtl::RigCtl() {
    model = RIGCTL_MODEL_DEFAULT;
    pollingInterval = RIGCTL_POLLING_INTERVAL_DEFAULT;

    serialPort = new SerialPort();
}

RigCtl::~RigCtl() {
    delete serialPort;
}

qint32 RigCtl::getModel() const {
    return model;
}

void RigCtl::setModel(qint32 newValue) {
    RigCtl::model = newValue;
}

quint8 RigCtl::getIcomCiv() const {
    return icomCIV;
}

void RigCtl::setIcomCiv(quint8 newValue) {
    icomCIV = newValue;
}

int RigCtl::getPollingInterval() const {
    return pollingInterval;
}

void RigCtl::setPollingInterval(int newValue) {
    RigCtl::pollingInterval = newValue;
}

SerialPort *RigCtl::getSerialPort() const {
    return serialPort;
}
