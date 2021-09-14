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


#include "service.hpp"

using namespace ulog::utilities;

Service::Service(QObject *parent) : QObject(parent) {
    thread = new QThread();

    connect(this, &Service::terminated, thread, &QThread::terminate);
    connect(this, &Service::terminated, thread, &QThread::deleteLater);

    moveToThread(thread);
    thread->start();
}

Service::~Service() {
    QMetaObject::invokeMethod(this, &Service::terminated, Qt::QueuedConnection);
}
