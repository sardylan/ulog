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

#define DATABASE_CONNECTION_NAME "ulog"

#define DATABASE_TABLE_NAME_QSO "qso"
#define DATABASE_TABLE_NAME_FIELDS "fields"

#define DATABASE_DATA_ROLE_VISIBLE Qt::UserRole
#define DATABASE_DATA_ROLE_TYPE Qt::UserRole + 1
#define DATABASE_DATA_ROLE_POSITION Qt::UserRole + 2

#define DATABASE_VERSION 1

namespace ulog::app {

    class Database : public QObject {
    Q_OBJECT

    public:

        explicit Database(QObject *parent = nullptr);

        ~Database() override;

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

    public slots:

        void open();

        void close();

        void readAll();

        void qsoAdd();

        void qsoRemove(int rowNumber);

    private:

        QString type;
        QString file;
        QString host;
        quint16 port;
        QString username;
        QString password;
        QString name;

        QMutex *mutex;
        bool opened;
        QSqlDatabase sqlDatabase;
        QSqlTableModel *qsoTableModel;

        QMap<QString, bool> qsoFieldEnabled;
        QMap<QString, QString> qsoFieldDescriptions;
        QMap<QString, QString> qsoFieldTypes;
        QMap<QString, int> qsoFieldPositions;

        void openDB();

        void closeDB();

        void checkDDL();

        void prepareQsoTableModel();

    signals:

        void error(QString message);

        void connected();

        void disconnected();

        void newQsoTableModel(QSqlTableModel *qsoTableModel);

        void dataRead();

    };

}
#endif
