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


#ifndef __ULOG__APP__CONFIG__DATABASE__H
#define __ULOG__APP__CONFIG__DATABASE__H

#include <QtCore/QtGlobal>
#include <QtCore/QString>

#define DATABASE_TYPE_DEFAULT "QSQLITE"
#define DATABASE_FILE_DEFAULT ""
#define DATABASE_HOST_DEFAULT "127.0.0.1"
#define DATABASE_PORT_DEFAULT 5432
#define DATABASE_USERNAME_DEFAULT "ulog"
#define DATABASE_PASSWORD_DEFAULT "ulog"
#define DATABASE_NAME_DEFAULT "ulog"

namespace ulog::app::config {

    class Database {

    public:

        Database();

        ~Database();

        [[nodiscard]] const QString &getType() const;

        void setType(const QString &newValue);

        [[nodiscard]] const QString &getFile() const;

        void setFile(const QString &newValue);

        [[nodiscard]] const QString &getHost() const;

        void setHost(const QString &newValue);

        [[nodiscard]] quint16 getPort() const;

        void setPort(quint16 newValue);

        [[nodiscard]] const QString &getUsername() const;

        void setUsername(const QString &newValue);

        [[nodiscard]] const QString &getPassword() const;

        void setPassword(const QString &newValue);

        [[nodiscard]] const QString &getName() const;

        void setName(const QString &newValue);

    private:

        QString type;
        QString file;
        QString host;
        quint16 port;
        QString username;
        QString password;
        QString name;

    };

}

#endif
