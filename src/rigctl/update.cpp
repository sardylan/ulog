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


#include "update.hpp"

using namespace ulog::rigctl;

Update::Update() {
    vfoNum = 0;
    frequency = 0;
    mode = Mode::NONE;
    bandwidth = 0;
    ptt = false;
    power = 0;
    strength = 0;
}

Update::~Update() = default;

int Update::getVfoNum() const {
    return vfoNum;
}

void Update::setVfoNum(int newValue) {
    Update::vfoNum = newValue;
}

quint64 Update::getFrequency() const {
    return frequency;
}

void Update::setFrequency(quint64 newValue) {
    Update::frequency = newValue;
}

Mode Update::getMode() const {
    return mode;
}

void Update::setMode(Mode newValue) {
    Update::mode = newValue;
}

qint32 Update::getBandwidth() const {
    return bandwidth;
}

void Update::setBandwidth(qint32 newValue) {
    Update::bandwidth = newValue;
}

bool Update::isPtt() const {
    return ptt;
}

void Update::setPtt(bool newValue) {
    Update::ptt = newValue;
}

float Update::getPower() const {
    return power;
}

void Update::setPower(float newValue) {
    Update::power = newValue;
}

int Update::getStrength() const {
    return strength;
}

void Update::setStrength(int newValue) {
    Update::strength = newValue;
}

bool Update::isValid() {
    return frequency > 0
           && mode != Mode::NONE
           && bandwidth > 0;
}

void Update::registerMetaTypes() {
    qRegisterMetaType<ulog::rigctl::Update>("ulog::rigctl::Update");
}

QDebug ulog::rigctl::operator<<(QDebug debug, const Update &update) {
    QDebugStateSaver saver(debug);

    debug.nospace()
            << "VFO num: " << update.getVfoNum()
            << " - "
            << "Freq: " << update.getFrequency()
            << " - "
            << "Mode: " << update.getMode()
            << " - "
            << "BW: " << update.getBandwidth()
            << " - "
            << "PWR: " << update.getPower()
            << " - "
            << "Strength: " << update.getStrength();

    return debug;
}
