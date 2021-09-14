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


#ifndef __ULOG__RIGCTL__MODE__H
#define __ULOG__RIGCTL__MODE__H

#include <QtCore/QtGlobal>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>

#include <hamlib/rig.h>

namespace ulog::rigctl {

    enum class Mode : quint64 {
        NONE = RIG_MODE_NONE,
        AM = RIG_MODE_AM,
        CW = RIG_MODE_CW,
        USB = RIG_MODE_USB,
        LSB = RIG_MODE_LSB,
        RTTY = RIG_MODE_RTTY,
        FM = RIG_MODE_FM,
        WFM = RIG_MODE_WFM,
        CWR = RIG_MODE_CWR,
        RTTYR = RIG_MODE_RTTYR,
        AMS = RIG_MODE_AMS,
        PKTLSB = RIG_MODE_PKTLSB,
        PKTUSB = RIG_MODE_PKTUSB,
        PKTFM = RIG_MODE_PKTFM,
        ECSSUSB = RIG_MODE_ECSSUSB,
        ECSSLSB = RIG_MODE_ECSSLSB,
        FAX = RIG_MODE_FAX,
        SAM = RIG_MODE_SAM,
        SAL = RIG_MODE_SAL,
        SAH = RIG_MODE_SAH,
        DSB = RIG_MODE_DSB,
        FMN = RIG_MODE_FMN,
        PKTAM = RIG_MODE_PKTAM,
        P25 = RIG_MODE_P25,
        DSTAR = RIG_MODE_DSTAR,
        DPMR = RIG_MODE_DPMR,
        NXDNVN = RIG_MODE_NXDNVN,
        NXDN_N = RIG_MODE_NXDN_N,
        DCR = RIG_MODE_DCR,
        AMN = RIG_MODE_AMN,
        PSK = RIG_MODE_PSK,
        PSKR = RIG_MODE_PSKR,
        DD = RIG_MODE_DD,
        C4FM = RIG_MODE_C4FM,
        PKTFMN = RIG_MODE_PKTFMN,
#ifdef ULOG_HAMLIB_VERSION_4_1
        SPEC = RIG_MODE_SPEC,
#ifdef ULOG_HAMLIB_VERSION_4_3
        CWN = RIG_MODE_CWN
#endif
#endif
    };

    QString modeToString(const Mode &mode);

    QDebug operator<<(QDebug debug, const Mode &mode);

    void registerMetaTypeMode();
}

Q_DECLARE_METATYPE(ulog::rigctl::Mode)

#endif
