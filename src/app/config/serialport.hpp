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


#ifndef __ULOG__APP__CONFIG__SERIALPORT__H
#define __ULOG__APP__CONFIG__SERIALPORT__H

#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtSerialPort/QSerialPort>

#include <utilities/enums.hpp>

using namespace ulog::utilities;
using namespace ulog::utilities::enums;

#define SERIALPORT_NAME_DEFAULT ""
#define SERIALPORT_BAUD_RATE_DEFAULT QSerialPort::Baud9600
#define SERIALPORT_DATA_BITS_DEFAULT QSerialPort::Data8
#define SERIALPORT_PARITY_DEFAULT QSerialPort::NoParity
#define SERIALPORT_STOP_BITS_DEFAULT QSerialPort::OneStop
#define SERIALPORT_FLOW_CONTROL_DEFAULT QSerialPort::NoFlowControl
#define SERIALPORT_RTS_STATE_DEFAULT TriState::Undefined
#define SERIALPORT_DTR_STATE_DEFAULT TriState::Undefined

namespace ulog::app::config {

    class SerialPort {

    public:

        SerialPort();

        ~SerialPort();

        [[nodiscard]] const QString &getName() const;

        void setName(const QString &newValue);

        [[nodiscard]] QSerialPort::BaudRate getBaudRate() const;

        void setBaudRate(QSerialPort::BaudRate newValue);

        [[nodiscard]] QSerialPort::DataBits getDataBits() const;

        void setDataBits(QSerialPort::DataBits newValue);

        [[nodiscard]] QSerialPort::Parity getParity() const;

        void setParity(QSerialPort::Parity newValue);

        [[nodiscard]] QSerialPort::StopBits getStopBits() const;

        void setStopBits(QSerialPort::StopBits newValue);

        [[nodiscard]] QSerialPort::FlowControl getFlowControl() const;

        void setFlowControl(QSerialPort::FlowControl newValue);

        [[nodiscard]] enums::TriState getRtsState() const;

        void setRtsState(enums::TriState newValue);

        [[nodiscard]] enums::TriState getDtrState() const;

        void setDtrState(enums::TriState newValue);

    private:

        QString name;
        QSerialPort::BaudRate baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
        enums::TriState rtsState;
        enums::TriState dtrState;

    };

}

#endif
