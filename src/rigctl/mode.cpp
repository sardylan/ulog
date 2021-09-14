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


#include "mode.hpp"

QString ulog::rigctl::modeToString(const Mode &mode) {
    switch (mode) {
        case Mode::NONE:
            return "NONE";
        case Mode::AM:
            return "AM";
        case Mode::CW:
            return "CW";
        case Mode::USB:
            return "USB";
        case Mode::LSB:
            return "LSB";
        case Mode::RTTY:
            return "RTTY";
        case Mode::FM:
            return "FM";
        case Mode::WFM:
            return "WFM";
        case Mode::CWR:
            return "CWR";
        case Mode::RTTYR:
            return "RTTYR";
        case Mode::AMS:
            return "AMS";
        case Mode::PKTLSB:
            return "PKTLSB";
        case Mode::PKTUSB:
            return "PKTUSB";
        case Mode::PKTFM:
            return "PKTFM";
        case Mode::ECSSUSB:
            return "ECSSUSB";
        case Mode::ECSSLSB:
            return "ECSSLSB";
        case Mode::FAX:
            return "FAX";
        case Mode::SAM:
            return "SAM";
        case Mode::SAL:
            return "SAL";
        case Mode::SAH:
            return "SAH";
        case Mode::DSB:
            return "DSB";
        case Mode::FMN:
            return "FMN";
        case Mode::PKTAM:
            return "PKTAM";
        case Mode::P25:
            return "P25";
        case Mode::DSTAR:
            return "DSTAR";
        case Mode::DPMR:
            return "DPMR";
        case Mode::NXDNVN:
            return "NXDNVN";
        case Mode::NXDN_N:
            return "NXDN_N";
        case Mode::DCR:
            return "DCR";
        case Mode::AMN:
            return "AMN";
        case Mode::PSK:
            return "PSK";
        case Mode::PSKR:
            return "PSKR";
        case Mode::DD:
            return "DD";
        case Mode::C4FM:
            return "C4FM";
        case Mode::PKTFMN:
            return "PKTFMN";
        case Mode::SPEC:
            return "SPEC";
        case Mode::CWN:
            return "CWN";
    }
    return "";
}

QDebug ulog::rigctl::operator<<(QDebug debug, const Mode &mode) {
    QDebugStateSaver saver(debug);
    debug.nospace() << modeToString(mode);
    return debug;
}

void ulog::rigctl::registerMetaTypeMode() {
    qRegisterMetaType<ulog::rigctl::Mode>("ulog::rigctl::Mode");
}
