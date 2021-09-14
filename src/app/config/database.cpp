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


#include "database.hpp"

using namespace ulog::app::config;

Database::Database() {
    type = DATABASE_TYPE_DEFAULT;
    file = DATABASE_FILE_DEFAULT;
    host = DATABASE_HOST_DEFAULT;
    port = DATABASE_PORT_DEFAULT;
    username = DATABASE_USERNAME_DEFAULT;
    password = DATABASE_PASSWORD_DEFAULT;
    name = DATABASE_NAME_DEFAULT;
}

Database::~Database() = default;

const QString &Database::getType() const {
    return type;
}

void Database::setType(const QString &newValue) {
    Database::type = newValue;
}

const QString &Database::getFile() const {
    return file;
}

void Database::setFile(const QString &newValue) {
    Database::file = newValue;
}

const QString &Database::getHost() const {
    return host;
}

void Database::setHost(const QString &newValue) {
    Database::host = newValue;
}

quint16 Database::getPort() const {
    return port;
}

void Database::setPort(quint16 newValue) {
    Database::port = newValue;
}

const QString &Database::getUsername() const {
    return username;
}

void Database::setUsername(const QString &newValue) {
    Database::username = newValue;
}

const QString &Database::getPassword() const {
    return password;
}

void Database::setPassword(const QString &newValue) {
    Database::password = newValue;
}

const QString &Database::getName() const {
    return name;
}

void Database::setName(const QString &newValue) {
    Database::name = newValue;
}
