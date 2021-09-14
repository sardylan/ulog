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

#include "database.hpp"

using namespace ulog::app;

Database::Database(QObject *parent) : QObject(parent) {
    mutex = new QMutex();
    opened = false;
    qsoTableModel = nullptr;
}

Database::~Database() {
    delete qsoTableModel;
    delete mutex;
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
            newRecord.setValue("tx_frequnecy", 123456);
        else if (columnName == "rx_frequnecy")
            newRecord.setValue("rx_frequnecy", 123456);
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

    qDebug() << "Opening sqlite DB" << DATABASE_FILE_NAME;
    sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    sqlDatabase.setDatabaseName(DATABASE_FILE_NAME);
    if (!sqlDatabase.open()) {
        qCritical() << "Unable to Open database";
        closeDB();
        return;
    }

    qsoFieldTypes.clear();
    qsoFieldDescriptions.clear();
    qsoFieldFocus.clear();

    prepareQsoTableModel();

    opened = true;

    QMetaObject::invokeMethod(this, &Database::connected, Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, "newQsoTableModel", Qt::QueuedConnection, Q_ARG(QSqlTableModel*, qsoTableModel));
}

void Database::closeDB() {
    qInfo() << "Closing Database";

    qDebug() << "Deleting table model";
    qsoTableModel->deleteLater();

    qDebug() << "Closing database connection";
    sqlDatabase.close();

    qsoFieldTypes.clear();
    qsoFieldDescriptions.clear();
    qsoFieldFocus.clear();

    opened = false;

    QMetaObject::invokeMethod(this, &Database::disconnected, Qt::QueuedConnection);
}

void Database::prepareQsoTableModel() {
    qInfo() << "Preparing QSO table model";

    qDebug() << "Creating Table Model";
    qsoTableModel = new QSqlTableModel(nullptr, sqlDatabase);
    qsoTableModel->setTable(DATABASE_TABLE_NAME_QSO);

    qDebug() << "Reading fields";
    QSqlQuery sqlQuery;
    QString query = QString("SELECT f.field_name  AS field_name, "
                            "       f.field_type  AS field_type, "
                            "       f.description AS description, "
                            "       f.position AS position "
                            "FROM %1 f "
                            "WHERE f.table_name = :table_name "
                            "  AND f.enabled = TRUE "
                            "ORDER BY f.position")
            .arg(DATABASE_TABLE_NAME_FIELDS);

    sqlQuery.prepare(query);
    sqlQuery.bindValue(":table_name", DATABASE_TABLE_NAME_QSO);
    sqlQuery.exec();

    sqlQuery.numRowsAffected();

    while (sqlQuery.next()) {
        QSqlRecord sqlRecord = sqlQuery.record();
        QString fieldName = sqlRecord.value("field_name").toString();
        QString fieldType = sqlRecord.value("field_type").toString();
        QString fieldDescription = sqlRecord.value("description").toString();
        bool fieldFocus = static_cast<bool>(sqlRecord.value("position").toInt() == 0);

        qsoFieldTypes.insert(fieldName, fieldType);
        qsoFieldDescriptions.insert(fieldName, fieldDescription);
        qsoFieldFocus.insert(fieldName, fieldFocus);
    }

    qDebug() << qsoFieldFocus;

    qDebug() << "Setting headers";
    QSqlRecord sqlRecord = qsoTableModel->record();
    int columnCount = qsoTableModel->columnCount();

    for (int i = 0; i < columnCount; i++) {
        QString columnName = sqlRecord.fieldName(i);
        bool isVisible = qsoFieldDescriptions.contains(columnName);

        if (isVisible) {
            QString columnType = qsoFieldTypes.value(columnName);
            QString columnDescription = qsoFieldDescriptions.value(columnName);
            bool isFocus = qsoFieldFocus.value(columnName);
            qsoTableModel->setHeaderData(i, Qt::Horizontal, columnDescription);
            qsoTableModel->setHeaderData(i, Qt::Horizontal, columnType, DATABASE_DATA_ROLE_TYPE);
            qsoTableModel->setHeaderData(i, Qt::Horizontal, isFocus, DATABASE_DATA_ROLE_FOCUS);
        }

        qsoTableModel->setHeaderData(i, Qt::Horizontal, isVisible, DATABASE_DATA_ROLE_VISIBLE);
    }

    qDebug() << "Selecting editing model";
    qsoTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
}
