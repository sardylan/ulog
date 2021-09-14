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


#ifndef __ULOG__RIGCTL__UPDATE__H
#define __ULOG__RIGCTL__UPDATE__H

#include <QtCore/QtGlobal>

#include "mode.hpp"

namespace ulog::rigctl {

    class Update {

    public:

        Update();

        ~Update();

        [[nodiscard]] int getVfoNum() const;

        void setVfoNum(int newValue);

        [[nodiscard]] quint64 getFrequency() const;

        void setFrequency(quint64 newValue);

        [[nodiscard]] Mode getMode() const;

        void setMode(Mode newValue);

        [[nodiscard]] qint32 getBandwidth() const;

        void setBandwidth(qint32 newValue);

        [[nodiscard]] bool isPtt() const;

        void setPtt(bool newValue);

        [[nodiscard]] float getPower() const;

        void setPower(float newValue);

        [[nodiscard]] int getStrength() const;

        void setStrength(int newValue);

        bool isValid();

        static void registerMetaTypes();

    private:

        int vfoNum;
        quint64 frequency;
        Mode mode;
        qint32 bandwidth;
        bool ptt;
        float power;
        int strength;

    };

    QDebug operator<<(QDebug debug, const Update &update);

}

Q_DECLARE_METATYPE(ulog::rigctl::Update)

#endif
