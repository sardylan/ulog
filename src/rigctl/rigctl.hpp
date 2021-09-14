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


#ifndef __ULOG__RIGCTL__RIGCTL__H
#define __ULOG__RIGCTL__RIGCTL__H

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QTimer>
#include <QtCore/QPair>

#include <hamlib/rig.h>

#include <utilities/service.hpp>

#include "params.hpp"
#include "update.hpp"
#include "mode.hpp"

namespace ulog::rigctl {

    class RigCtl : public utilities::Service {
    Q_OBJECT

    public:

        explicit RigCtl(QObject *parent = nullptr);

        ~RigCtl() override;

        [[nodiscard]] Params *getParams() const;

        bool isOpen();

    public slots:

        void start() override;

        void stop() override;

    private:

        QMutex *mutex;
        Params *params;
        QTimer *pollingTimer;

        RIG *rig;
        int vfoCount;

        vfo_t vfoA;
        vfo_t vfoB;
        vfo_t vfoC;

        bool hasRFPower;
        bool hasMeter;
        bool hasSWR;
        bool hasStrength;

        void hamlibStart();

        void hamlibStop();

        void configAdjust();

        void readVfo(int vfoNum, vfo_t vfo);

        quint64 readVfoFrequency(vfo_t vfo);

        QPair<Mode, qint32> readVfoModeBandwidth(vfo_t vfo);

        bool readVfoPTT(vfo_t vfo);

        value_t readLevel(vfo_t vfo, setting_t level);

    private slots:

        void polling();

    signals:

        void connected(int vfoCount);

        void disconnected();

        void vfoUpdate(ulog::rigctl::Update update);

    };

}

#endif
