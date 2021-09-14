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

#ifndef __ULOG__APP__CONFIG__CONFIG__H
#define __ULOG__APP__CONFIG__CONFIG__H

#include <QtCore/QString>

#include "rigctl.hpp"
#include "database.hpp"

namespace ulog::app::config {

    class Config {

    public:

        Config();

        ~Config();

        [[nodiscard]] RigCtl *getRigCtl() const;

        [[nodiscard]] Database *getDatabase() const;

    private:

        RigCtl *rigCtl;
        Database *database;

    };

}
#endif
