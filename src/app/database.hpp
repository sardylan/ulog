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


#ifndef __ULOG__APP__DATABASE__H
#define __ULOG__APP__DATABASE__H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtCore/QMap>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>

#define DATABASE_FILE_NAME "/home/sardylan/develop/sardylan/ulog/ulog.sqlite"

#define DATABASE_TABLE_NAME_QSO "qso"
#define DATABASE_TABLE_NAME_FIELDS "fields"

#define DATABASE_DATA_ROLE_VISIBLE Qt::UserRole
#define DATABASE_DATA_ROLE_TYPE Qt::UserRole + 1
#define DATABASE_DATA_ROLE_FOCUS Qt::UserRole + 2

namespace ulog::app {

    class Database : public QObject {
    Q_OBJECT

    public:

        explicit Database(QObject *parent = nullptr);

        ~Database() override;

    public slots:

        void open();

        void close();

        void readAll();

        void qsoAdd();

        void qsoRemove(int rowNumber);

    private:

        QMutex *mutex;
        bool opened;
        QSqlDatabase sqlDatabase;
        QSqlTableModel *qsoTableModel;

        QMap<QString, QString> qsoFieldTypes;
        QMap<QString, QString> qsoFieldDescriptions;
        QMap<QString, bool> qsoFieldFocus;

        void openDB();

        void closeDB();

        void prepareQsoTableModel();

    signals:

        void connected();

        void disconnected();

        void newQsoTableModel(QSqlTableModel *qsoTableModel);

        void dataRead();

    };

}
#endif
