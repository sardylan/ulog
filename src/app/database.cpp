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
#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

#include <utilities/resources.hpp>

#include "database.hpp"

using namespace ulog::app;

Database::Database(QObject *parent) : QObject(parent) {
    type = "";
    file = "";
    host = "";
    port = 0;
    username = "";
    password = "";
    name = "";

    mutex = new QMutex();
    opened = false;
    qsoTableModel = nullptr;
}

Database::~Database() {
    if (qsoTableModel != nullptr)
        qsoTableModel->deleteLater();

    delete mutex;
}

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

void Database::open() {
    mutex->lock();

    if (!opened)
        openDB();

    mutex->unlock();
}

void Database::close() {
    mutex->lock();

    if (opened)
        closeDB();

    mutex->unlock();
}

void Database::readAll() {
    mutex->lock();

    if (opened) {
        qsoTableModel->select();
        QMetaObject::invokeMethod(this, &Database::dataRead, Qt::QueuedConnection);
    }

    mutex->unlock();
}

void Database::qsoAdd() {
    qInfo() << "Adding QSO";

    qDebug() << "Preparing new record";
    QSqlRecord newRecord = qsoTableModel->record();

    int columnCount = qsoTableModel->columnCount();
    for (int i = 0; i < columnCount; i++) {
        const QSqlField &field = newRecord.field(i);
        const QString &columnName = field.name();

        if (columnName == "dt_start")
            newRecord.setValue("dt_start", QDateTime::currentDateTimeUtc());
        else if (columnName == "tx_frequnecy")
            newRecord.setValue("tx_frequnecy", 0);
        else if (columnName == "rx_frequnecy")
            newRecord.setValue("rx_frequnecy", 0);
    }

    qDebug() << "Opening a DB transaction";
    qsoTableModel->database().transaction();

    qDebug() << "Inserting record";
    bool result = qsoTableModel->insertRecord(-1, newRecord);
    if (!result) {
        qCritical() << "Unable to add new record in DB:" << newRecord;
        qDebug() << "Rollback transaction";
        qsoTableModel->database().rollback();
    }

    qDebug() << "Committing transaction";
    qsoTableModel->submitAll();
    qsoTableModel->database().commit();

    qsoTableModel->select();
}

void Database::qsoRemove(int rowNumber) {
    qInfo() << "Removing QSO in row" << rowNumber;
    qsoTableModel->database().transaction();
    qsoTableModel->removeRow(rowNumber);
    qsoTableModel->submitAll();
    qsoTableModel->database().commit();

    qsoTableModel->select();
}

void Database::openDB() {
    qInfo() << "Opening Database";

    qDebug() << "Verify driver" << type;
    const QStringList &sqlDrivers = QSqlDatabase::drivers();
    if (!sqlDrivers.contains(type)) {
        const QString &message = "Database driver not valid";
        qCritical() << message;
        QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection, Q_ARG(QString, message));
        closeDB();
        return;
    }

    qDebug() << "Configuring type" << type;
    sqlDatabase = QSqlDatabase::addDatabase(type, DATABASE_CONNECTION_NAME);
    if (type == "SQLITE" || type == "SQLITE2") {
        qDebug() << "File:" << file;
        sqlDatabase.setDatabaseName(file);
    } else {
        qDebug() << "Host:" << host << " - Port:" << port << " - Username:" << username << "DB name:" << name;
        sqlDatabase.setHostName(host);
        sqlDatabase.setPort(port);
        sqlDatabase.setUserName(username);
        sqlDatabase.setPassword(password);
        sqlDatabase.setDatabaseName(name);
    }

    qDebug() << "Opening";
    if (!sqlDatabase.open()) {
        qCritical() << "Unable to Open database";
        closeDB();
        return;
    }

    checkDDL();

    qsoFieldEnabled.clear();
    qsoFieldPositions.clear();
    qsoFieldTypes.clear();
    qsoFieldDescriptions.clear();

    prepareQsoTableModel();

    opened = true;

    QMetaObject::invokeMethod(this, &Database::connected, Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, "newQsoTableModel", Qt::QueuedConnection, Q_ARG(QSqlTableModel*, qsoTableModel));
}

void Database::closeDB() {
    qInfo() << "Closing Database";

    qDebug() << "Deleting table model";
    if (qsoTableModel != nullptr)
        qsoTableModel->deleteLater();

    qDebug() << "Closing database connection";
    sqlDatabase.close();
    sqlDatabase = QSqlDatabase();

    qsoFieldEnabled.clear();
    qsoFieldPositions.clear();
    qsoFieldTypes.clear();
    qsoFieldDescriptions.clear();

    opened = false;

    QMetaObject::invokeMethod(this, &Database::disconnected, Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, []() {
        QSqlDatabase::removeDatabase(DATABASE_CONNECTION_NAME);
    }, Qt::QueuedConnection);
}

void Database::checkDDL() {
    qInfo() << "Checking DDL";

    QString query;
    QSqlQuery sqlQuery(sqlDatabase);
    QSqlRecord sqlRecord;

    qDebug() << "Checking version table";
    if (!sqlDatabase.tables().contains("version")) {
        QByteArray rawQuery = utilities::Resources::readFile(":/sql/create_version");
        query = QString(rawQuery);
        sqlQuery.exec(query);
    }

    qDebug() << "Checking database version";
    int version;
    query = "SELECT ver FROM version;";
    sqlQuery.exec(query);
    if (!sqlQuery.next()) {
        version = 0;
    } else {
        sqlRecord = sqlQuery.record();
        version = sqlRecord.value("ver").toInt();
    }

    qDebug() << "Version is" << version;

    if (version == 0) {
        qDebug() << "Applying version 1";

        query = "INSERT INTO version (ver) VALUES(:version)";
        sqlQuery.prepare(query);
        sqlQuery.bindValue(":version", DATABASE_VERSION);
        sqlQuery.exec();

        QString filePath = QString(":/sql/versions/0-1-%1").arg(sqlDatabase.driverName());
        QByteArray rawQuery = utilities::Resources::readFile(filePath);
        QString stringQuery = QString(rawQuery);
        stringQuery = stringQuery.replace('\n', ' ');
        stringQuery = stringQuery.replace('\r', ' ');
        stringQuery = stringQuery.replace('\t', ' ');
        stringQuery = stringQuery.trimmed();

        QStringList queries = stringQuery.split(";");
        for (QString singleQuery: queries) {
            singleQuery = singleQuery.trimmed();
            if (singleQuery.length() == 0)
                return;

            qDebug() << singleQuery;
            if (!sqlQuery.exec(singleQuery))
                qWarning() << sqlQuery.lastError();
        }
    }
}

void Database::prepareQsoTableModel() {
    qInfo() << "Preparing QSO table model";

    qDebug() << "Creating Table Model";
    qsoTableModel = new QSqlTableModel(this, sqlDatabase);
    qsoTableModel->setTable(DATABASE_TABLE_NAME_QSO);

    qDebug() << "Reading fields";
    QSqlQuery sqlQuery(sqlDatabase);
    QString query = QString("SELECT f.field_name  AS field_name, "
                            "       f.field_type  AS field_type, "
                            "       f.description AS description, "
                            "       f.position AS position, "
                            "       f.enabled AS enabled "
                            "FROM %1 f "
                            "WHERE f.table_name = :table_name "
                            "ORDER BY position")
            .arg(DATABASE_TABLE_NAME_FIELDS);

    sqlQuery.prepare(query);
    sqlQuery.bindValue(":table_name", DATABASE_TABLE_NAME_QSO);
    sqlQuery.exec();

    while (sqlQuery.next()) {
        QSqlRecord sqlRecord = sqlQuery.record();
        QString fieldName = sqlRecord.value("field_name").toString();
        QString fieldType = sqlRecord.value("field_type").toString();
        QString fieldDescription = sqlRecord.value("description").toString();
        int position = sqlRecord.value("position").toInt();
        bool enabled = sqlRecord.value("enabled").toBool();

        qDebug() << "fieldName" << fieldName
                 << "-"
                 << "fieldType" << fieldType
                 << "-"
                 << "fieldDescription" << fieldDescription
                 << "-"
                 << "position" << position
                 << "-"
                 << "enabled" << enabled;

        qsoFieldEnabled.insert(fieldName, enabled);
        qsoFieldDescriptions.insert(fieldName, fieldDescription);
        qsoFieldTypes.insert(fieldName, fieldType);
        qsoFieldPositions.insert(fieldName, position);
    }

    qDebug() << "Setting headers";
    QSqlRecord sqlRecord = qsoTableModel->record();
    int columnCount = qsoTableModel->columnCount();

    for (int i = 0; i < columnCount; i++) {
        QString columnName = sqlRecord.fieldName(i);

        bool isVisible = qsoFieldEnabled.value(columnName);
        QString columnDescription = qsoFieldDescriptions.value(columnName);
        QString columnType = qsoFieldTypes.value(columnName);
        int position = qsoFieldPositions.value(columnName);

        qsoTableModel->setHeaderData(i, Qt::Horizontal, columnDescription);
        qsoTableModel->setHeaderData(i, Qt::Horizontal, isVisible, DATABASE_DATA_ROLE_VISIBLE);
        qsoTableModel->setHeaderData(i, Qt::Horizontal, columnType, DATABASE_DATA_ROLE_TYPE);
        qsoTableModel->setHeaderData(i, Qt::Horizontal, position, DATABASE_DATA_ROLE_POSITION);
    }

    qDebug() << "Selecting editing model";
    qsoTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
}
