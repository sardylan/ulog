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


#include <QtCore/QDebug>

#include "rigctl.hpp"

#define RIGCTL_BUFLEN 8192

#ifndef HAMLIB_FILPATHLEN
#define HAMLIB_FILPATHLEN FILPATHLEN
#endif

using namespace ulog::rigctl;

RigCtl::RigCtl(QObject *parent) : Service(parent) {
    params = new Params();

    mutex = new QMutex();

    pollingTimer = new QTimer();
    pollingTimer->setSingleShot(false);

    rig = nullptr;
    vfoCount = 0;

    vfoA = RIG_VFO_A;
    vfoB = RIG_VFO_B;
    vfoC = RIG_VFO_C;

    hasRFPower = false;
    hasMeter = false;
    hasSWR = false;
    hasStrength = false;
}

RigCtl::~RigCtl() {
    delete pollingTimer;
    delete mutex;
    delete params;
}

Params *RigCtl::getParams() const {
    return params;
}

bool RigCtl::isOpen() {
    return rig != nullptr;
}

void RigCtl::start() {
    mutex->lock();

    if (rig == nullptr)
        hamlibStart();

    mutex->unlock();
}

void RigCtl::stop() {
    mutex->lock();

    if (rig != nullptr)
        hamlibStop();

    mutex->unlock();
}

void RigCtl::hamlibStart() {
    qInfo() << "Starting HamLib";

    qDebug() << "Initializing HamLib";
    rig_set_debug(RIG_DEBUG_ERR);

    quint32 model = params->getRigModel();
    rig = rig_init(model);
    if (rig == nullptr) {
        qCritical() << "Unable to initialize HamLib";
        return;
    }

    qDebug() << "Setting device";
    const std::string &portNameString = params->getPortName().toStdString();
    const char *portName = portNameString.c_str();
    strncpy(rig->state.rigport.pathname, portName, HAMLIB_FILPATHLEN - 1);

    int rate;

    switch (params->getPortBaudRate()) {

        case QSerialPort::Baud1200:
            rate = 1200;
            break;

        case QSerialPort::Baud2400:
            rate = 2400;
            break;

        case QSerialPort::Baud4800:
            rate = 4800;
            break;

        case QSerialPort::Baud9600:
            rate = 9600;
            break;

        case QSerialPort::Baud19200:
            rate = 19200;
            break;

        case QSerialPort::Baud38400:
            rate = 38400;
            break;

        case QSerialPort::Baud57600:
            rate = 57600;
            break;

        case QSerialPort::Baud115200:
            rate = 115200;
            break;

        case QSerialPort::UnknownBaud:
            rate = 0;
            break;
    }

    rig->state.rigport.parm.serial.rate = rate;

    int dataBits;

    switch (params->getPortDataBits()) {

        case QSerialPort::Data5:
            dataBits = 5;
            break;

        case QSerialPort::Data6:
            dataBits = 6;
            break;

        case QSerialPort::Data7:
            dataBits = 7;
            break;

        case QSerialPort::Data8:
            dataBits = 8;
            break;

        case QSerialPort::UnknownDataBits:
            dataBits = 0;
            break;
    }

    rig->state.rigport.parm.serial.data_bits = dataBits;

    enum serial_parity_e parity;

    switch (params->getPortParity()) {

        case QSerialPort::NoParity:
            parity = RIG_PARITY_NONE;
            break;

        case QSerialPort::EvenParity:
            parity = RIG_PARITY_EVEN;
            break;

        case QSerialPort::OddParity:
            parity = RIG_PARITY_ODD;
            break;

        case QSerialPort::SpaceParity:
            parity = RIG_PARITY_SPACE;
            break;

        case QSerialPort::MarkParity:
            parity = RIG_PARITY_MARK;
            break;

        case QSerialPort::UnknownParity:
            parity = RIG_PARITY_NONE;
            break;
    }

    rig->state.rigport.parm.serial.parity = parity;

    int stopBits;

    switch (params->getPortStopBits()) {

        case QSerialPort::OneStop:
        case QSerialPort::UnknownStopBits:
            stopBits = 1;
            break;

        case QSerialPort::OneAndHalfStop:
        case QSerialPort::TwoStop:
            stopBits = 2;
            break;

    }

    rig->state.rigport.parm.serial.stop_bits = stopBits;

    enum serial_handshake_e handshake;

    switch (params->getPortFlowControl()) {

        case QSerialPort::NoFlowControl:
            handshake = RIG_HANDSHAKE_NONE;
            break;

        case QSerialPort::HardwareControl:
            handshake = RIG_HANDSHAKE_HARDWARE;
            break;

        case QSerialPort::SoftwareControl:
            handshake = RIG_HANDSHAKE_XONXOFF;
            break;

        case QSerialPort::UnknownFlowControl:
            handshake = RIG_HANDSHAKE_NONE;
            break;

    }

    rig->state.rigport.parm.serial.handshake = handshake;

    enum serial_control_state_e rtsState;

    switch (params->getPortRtsState()) {

        case TriState::Undefined:
            rtsState = RIG_SIGNAL_UNSET;
            break;

        case TriState::False:
            rtsState = RIG_SIGNAL_OFF;
            break;

        case TriState::True:
            rtsState = RIG_SIGNAL_ON;
            break;
    }

    rig->state.rigport.parm.serial.rts_state = rtsState;

    enum serial_control_state_e dtrState;

    switch (params->getPortDtrState()) {

        case TriState::Undefined:
            dtrState = RIG_SIGNAL_UNSET;
            break;

        case TriState::False:
            dtrState = RIG_SIGNAL_OFF;
            break;

        case TriState::True:
            dtrState = RIG_SIGNAL_ON;
            break;
    }

    rig->state.rigport.parm.serial.dtr_state = dtrState;

    qDebug() << "Opening device";
    int returnCode = rig_open(rig);
    if (returnCode != RIG_OK) {
        qCritical() << "Unable to open rig port" << rigerror(returnCode);
        hamlibStop();
        return;
    }

    qDebug() << "Getting rig informations";
    const char *infoBuffer = rig_get_info(rig);
    qInfo() << "Rig:" << infoBuffer;

#ifdef ULOG_HAMLIB_VERSION_4_2
    qDebug() << "Getting VFO list";
    char rawBuf[RIGCTL_BUFLEN];
    returnCode = rig_get_vfo_list(rig, rawBuf, RIGCTL_BUFLEN);
    if (returnCode != RIG_OK) {
        qCritical() << "Unable to get VFO list" << rigerror(returnCode);
        hamlibStop();
        return;
    }
    QString buf(rawBuf);
    QStringList availableVFONames = buf.trimmed().split(" ");
    vfoCount = availableVFONames.size();
#else
    vfoCount = 1;
#endif

    qDebug() << "Checking levels";
    if (rig_has_get_level(rig, RIG_LEVEL_RFPOWER))
        hasRFPower = true;
    if (rig_has_get_level(rig, RIG_LEVEL_METER))
        hasMeter = true;
    if (rig_has_get_level(rig, RIG_LEVEL_SWR))
        hasSWR = true;
    if (rig_has_get_level(rig, RIG_LEVEL_STRENGTH))
        hasStrength = true;

    configAdjust();

    qDebug() << "Connecting timer, setting polling interval and start";
    connect(pollingTimer, &QTimer::timeout, this, &RigCtl::polling);
    pollingTimer->setInterval(params->getPollingInterval());

    qDebug() << "Starting polling timer";
    QMetaObject::invokeMethod(pollingTimer, qOverload<>(&QTimer::start), Qt::QueuedConnection);

    qDebug() << "Sending connected signal";
    QMetaObject::invokeMethod(this, "connected", Qt::QueuedConnection, Q_ARG(int, vfoCount));
}

void RigCtl::hamlibStop() {
    qInfo() << "Stopping HamLib";

    qDebug() << "Stopping and disconnecting timer";
    disconnect(pollingTimer, &QTimer::timeout, this, &RigCtl::polling);
    QMetaObject::invokeMethod(pollingTimer, &QTimer::stop, Qt::BlockingQueuedConnection);

    qDebug() << "Closing HamLib";
    rig_close(rig);

    qDebug() << "Setting rig to nullptr";
    rig = nullptr;

    qDebug() << "Sending disconnected signal";
    QMetaObject::invokeMethod(this, &RigCtl::disconnected, Qt::QueuedConnection);
}

void RigCtl::polling() {
    qInfo() << "HamLib Polling Rig";

    if (!isOpen())
        return;

    if (vfoCount >= 1)
        readVfo(1, vfoA);

    if (vfoCount >= 2)
        readVfo(2, vfoB);

    if (vfoCount >= 3)
        readVfo(3, vfoC);
}

void RigCtl::configAdjust() {
    qDebug() << "Adjusting configuration based on model";
    if (params->getRigModel() == RIG_MODEL_FT817) {
        vfoA = RIG_VFO_CURR;
        vfoCount = 1;
        hasRFPower = false;
    }
}

void RigCtl::readVfo(int vfoNum, vfo_t vfo) {
    if (!isOpen())
        return;

    Update update;
    value_t value;

    update.setVfoNum(vfoNum);

    update.setFrequency(readVfoFrequency(vfo));

    QPair<Mode, qint32> modeBandwidth = readVfoModeBandwidth(vfo);
    update.setMode(modeBandwidth.first);
    update.setBandwidth(modeBandwidth.second);

    update.setPtt(readVfoPTT(vfo));

    if (hasRFPower) {
        value = readLevel(vfo, RIG_LEVEL_RFPOWER_METER_WATTS);
        update.setPower(value.f);
    }

    if (hasStrength) {
        value = readLevel(vfo, RIG_LEVEL_STRENGTH);
        update.setStrength(value.i);
    }

    qDebug() << "VFO" << update;

    if (!update.isValid())
        return;

    QMetaObject::invokeMethod(this, "vfoUpdate", Qt::QueuedConnection, Q_ARG(ulog::rigctl::Update, update));
}

quint64 RigCtl::readVfoFrequency(vfo_t vfo) {
    if (!isOpen())
        return 0;

    int status;
    freq_t freq;

    status = rig_get_freq(rig, vfo, &freq);
    if (status != RIG_OK) {
        qWarning() << "Error getting frequency" << rigerror(status);
        QMetaObject::invokeMethod(this, &RigCtl::stop, Qt::QueuedConnection);
        return 0;
    }

    return static_cast<quint64>(freq);
}

QPair<Mode, qint32> RigCtl::readVfoModeBandwidth(vfo_t vfo) {
    if (!isOpen())
        return {Mode::NONE, 0};

    int status;
    rmode_t rawMode;
    pbwidth_t rawWidth;

    status = rig_get_mode(rig, vfo, &rawMode, &rawWidth);
    if (status != RIG_OK) {
        qWarning() << "Error getting frequency" << rigerror(status);
        QMetaObject::invokeMethod(this, &RigCtl::stop, Qt::QueuedConnection);
        return {Mode::NONE, 0};
    }

    auto mode = static_cast<Mode>(rawMode);
    auto bandwidth = static_cast<qint32>(rawWidth);

    return {mode, bandwidth};
}

bool RigCtl::readVfoPTT(vfo_t vfo) {
    if (!isOpen())
        return false;

    int status;
    ptt_t ptt;

    status = rig_get_ptt(rig, vfo, &ptt);
    if (status != RIG_OK) {
        qWarning() << "Error getting PTT" << rigerror(status);
        QMetaObject::invokeMethod(this, &RigCtl::stop, Qt::QueuedConnection);
        return false;
    }

    return ptt != RIG_PTT_OFF;
}

value_t RigCtl::readLevel(vfo_t vfo, setting_t level) {
    if (!isOpen())
        return {};

    int status;
    value_t value;

    status = rig_get_level(rig, vfo, level, &value);
    if (status != RIG_OK) {
        qWarning() << "Error getting level" << level << " - " << rigerror(status);
        QMetaObject::invokeMethod(this, &RigCtl::stop, Qt::QueuedConnection);
        return {};
    }

    return value;
}
