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


#ifndef __ULOG__RIGCTL__PARAMS__H
#define __ULOG__RIGCTL__PARAMS__H

#include <QtCore/QString>

#include <QtSerialPort/QSerialPort>

#include <hamlib/rig.h>
#include <hamlib/riglist.h>

#include <utilities/enums.hpp>

using namespace ulog::utilities::enums;
using namespace ulog::utilities;

#define RIGCTL_RIG_MODEL_DEFAULT RIG_MODEL_FT818
#define RIGCTL_ICOM_ICV_DEFAULT 0
#define RIGCTL_POLLING_INTERVAL_DEFAULT 500

#define RIGCTL_PORT_NAME_DEFAULT "/dev/ttyACM0"
#define RIGCTL_PORT_BAUD_RATE_DEFAULT QSerialPort::Baud19200
#define RIGCTL_PORT_DATA_BITS_DEFAULT QSerialPort::Data8
#define RIGCTL_PORT_PARITY_DEFAULT QSerialPort::NoParity
#define RIGCTL_PORT_STOP_BITS_DEFAULT QSerialPort::OneStop
#define RIGCTL_PORT_FLOW_CONTROL_DEFAULT QSerialPort::NoFlowControl
#define RIGCTL_RTS_STATE_DEFAULT TriState::Undefined
#define RIGCTL_DTR_STATE_DEFAULT TriState::Undefined

namespace ulog::rigctl {

    class Params {

    public:

        Params();

        ~Params();

        [[nodiscard]] rig_model_t getRigModel() const;

        void setRigModel(rig_model_t newValue);

        [[nodiscard]] quint8 getIcomIcv() const;

        void setIcomIcv(quint8 newValue);

        [[nodiscard]] int getPollingInterval() const;

        void setPollingInterval(int newValue);

        [[nodiscard]] const QString &getPortName() const;

        void setPortName(const QString &newValue);

        [[nodiscard]] QSerialPort::BaudRate getPortBaudRate() const;

        void setPortBaudRate(QSerialPort::BaudRate newValue);

        [[nodiscard]] QSerialPort::DataBits getPortDataBits() const;

        void setPortDataBits(QSerialPort::DataBits newValue);

        [[nodiscard]] QSerialPort::Parity getPortParity() const;

        void setPortParity(QSerialPort::Parity newValue);

        [[nodiscard]] QSerialPort::StopBits getPortStopBits() const;

        void setPortStopBits(QSerialPort::StopBits newValue);

        [[nodiscard]] QSerialPort::FlowControl getPortFlowControl() const;

        void setPortFlowControl(QSerialPort::FlowControl newValue);

        [[nodiscard]] enums::TriState getPortRtsState() const;

        void setPortRtsState(enums::TriState newValue);

        [[nodiscard]] enums::TriState getPortDtrState() const;

        void setPortDtrState(enums::TriState newValue);

    private:

        rig_model_t rigModel;
        quint8 icomIcv;
        int pollingInterval;

        QString portName;
        QSerialPort::BaudRate portBaudRate;
        QSerialPort::DataBits portDataBits;
        QSerialPort::Parity portParity;
        QSerialPort::StopBits portStopBits;
        QSerialPort::FlowControl portFlowControl;
        enums::TriState portRtsState;
        enums::TriState portDtrState;

    };

}

#endif
