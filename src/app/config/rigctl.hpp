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

#ifndef __ULOG__APP__CONFIG__RIGCTL__H
#define __ULOG__APP__CONFIG__RIGCTL__H

#include <QtCore/QtGlobal>

#include "serialport.hpp"

#define RIGCTL_MODEL_DEFAULT 0
#define RIGCTL_ICOM_ICV_DEFAULT 0
#define RIGCTL_POLLING_INTERVAL_DEFAULT 500

namespace ulog::app::config {

    class RigCtl {

    public:

        RigCtl();

        virtual ~RigCtl();

        [[nodiscard]] qint32 getModel() const;

        void setModel(qint32 newValue);

        [[nodiscard]] quint8 getIcomCiv() const;

        void setIcomCiv(quint8 newValue);

        [[nodiscard]] int getPollingInterval() const;

        void setPollingInterval(int newValue);

        [[nodiscard]] SerialPort *getSerialPort() const;

    private:

        qint32 model;
        quint8 icomCIV;
        int pollingInterval;

        SerialPort *serialPort;

    };

}
#endif
