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
#include <QtCore/QCoreApplication>

#include <QtWidgets/QApplication>

#include <rigctl/update.hpp>
#include <rigctl/mode.hpp>

#include "main.hpp"
#include "ulog.hpp"
#include "version.hpp"

ulog::app::ULog *mainApplication;

#ifdef Q_OS_LINUX

void signalHandler(int signal) {
    switch (signal) {
        case SIGINT:
        case SIGTERM:
            mainApplication->stop();
            break;

        default:
            return;
    }
}

#endif

#ifdef Q_OS_WINDOWS

BOOL WINAPI ctrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
            case CTRL_CLOSE_EVENT:
                mainApplication->stop();
                return TRUE;

                default:
                    return FALSE;
    }
}

#endif


int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

    qSetMessagePattern("\x1b[94;1m[\x1b[96;1m%{time yyyy-MM-dd hh:mm:ss.zzz}\x1b[94;1m]\x1b[39;0m "
                       "PID:\x1b[31m%{pid}\x1b[39m "
                       "TID:\x1b[91m%{threadid}\x1b[39m "
                       "["
                       "%{if-debug}\x1b[37m DEBUG  \x1b[39m%{endif}"
                       "%{if-info}\x1b[92m INFO   \x1b[39m%{endif}"
                       "%{if-warning}\x1b[93mWARNING \x1b[39m%{endif}"
                       "%{if-critical}\x1b[91mCRITICAL\x1b[39m%{endif}"
                       "%{if-fatal}\x1b[91;5m FATAL  \x1b[39;25m%{endif}"
                       "]: "
                       "%{file}:%{line} "
                       "[\x1b[97m%{function}()\x1b[39m] "
                       "%{message}");

    declareMetatypes();

    QApplication application(argc, argv);

    mainApplication = new ulog::app::ULog();
    QApplication::connect(mainApplication, &ulog::app::ULog::finished, []() { QCoreApplication::exit(); });
    mainApplication->entryPoint();

#ifdef Q_OS_LINUX
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
#endif

#ifdef Q_OS_WINDOWS
    if (SetConsoleCtrlHandler(ctrlHandler, TRUE) != TRUE)
        qWarning() << "Unable to set Control Handler";
#endif

    int result = QApplication::exec();

    delete mainApplication;

    return result;
}

void declareMetatypes(){
    ulog::rigctl::registerMetaTypeMode();
    ulog::rigctl::Update::registerMetaTypes();
}
