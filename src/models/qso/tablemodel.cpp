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
#include <QtCore/QDateTime>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlRecord>

#include "tablemodel.hpp"

#define SQL_QUERY_FIELDS "SELECT f.field_name AS name, f.field_type AS type, f.description AS description FROM fields f WHERE f.table_name = :table_name ORDER BY f.position"
#define SQL_QUERY_RECORD_COUNT "SELECT count(id) FROM qso"

using namespace ulog::models;

QSOTableModel::QSOTableModel(QObject *parent) : QAbstractTableModel(parent) {

}

QSOTableModel::QSOTableModel(const QSOTableModel &other) : QSOTableModel(nullptr) {
    QSOTableModel::fieldNames = other.fieldNames;
    QSOTableModel::fieldDescriptions = other.fieldDescriptions;
}

QSOTableModel::~QSOTableModel() = default;

int QSOTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)

    QSqlQuery sqlQuery;

    if (!sqlQuery.exec(SQL_QUERY_RECORD_COUNT))
        return 0;

    if (!sqlQuery.first())
        return 0;

    int records = sqlQuery.value(0).toInt();

    return records;
}

int QSOTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    int size = fieldDescriptions.size();
    return size;
}

QVariant QSOTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return {};

    int column = index.column();
    int row = index.row();

    const QString &columnName = fieldNames.value(column);
    const QString &fieldType = fieldTypes.value(column);

    QString query = QString("SELECT %1 FROM %2 t WHERE t.id = :id")
            .arg(columnName, MODEL_QSO_TABLE_NAME);

    QSqlQuery sqlQuery;
    sqlQuery.prepare(query);
    sqlQuery.bindValue(":id", row + 1);
    sqlQuery.exec();
    if (!sqlQuery.first())
        return {};

    const QSqlRecord &record = sqlQuery.record();
    const QVariant &recordValue = record.value(columnName);


    if (fieldType == "DateTime") {
        qlonglong msecs = recordValue.toLongLong();
        const QDateTime &dateTime = QDateTime::fromMSecsSinceEpoch(msecs);

        if (role == Qt::EditRole)
            return dateTime;

        return dateTime.toString();

    } else if (fieldType == "String") {
        return recordValue.toString();

    } else {
        return recordValue;
    }
}

bool QSOTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;

    if (role != Qt::EditRole)
        return false;

    int column = index.column();
    int row = index.row();

    const QString &columnName = fieldNames.value(column);
//    const QString &fieldType = fieldTypes.value(column);

    QString query = QString("SELECT count(id) AS count FROM %2 t WHERE t.id = :id").arg(MODEL_QSO_TABLE_NAME);

    QSqlQuery sqlQuery;
    sqlQuery.prepare(query);
    sqlQuery.bindValue(":id", row + 1);
    sqlQuery.exec();
    if (!sqlQuery.first())
        return {};

    const QSqlRecord &record = sqlQuery.record();
    const QVariant &recordValue = record.value("count");
    const bool alreadyExists = recordValue.toInt() > 0;

    if (alreadyExists)
        query = QString("UPDATE %1 SET %2 = :value").arg(MODEL_QSO_TABLE_NAME, columnName);
    else
        query = QString("INSERT INTO %1 (%2) VALUES (:value)").arg(MODEL_QSO_TABLE_NAME, columnName);

    sqlQuery.prepare(query);
    sqlQuery.bindValue(":value", value);
    bool result = sqlQuery.exec();

    qDebug() << result << alreadyExists;

    return result;
}

QVariant QSOTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    switch (role) {
        case Qt::DisplayRole:
            if (orientation == Qt::Vertical)
                return section;

            return fieldDescriptions.value(section);

        case Qt::UserRole:
            if (orientation == Qt::Vertical)
                return section;

            return fieldNames.value(section);

        case Qt::TextAlignmentRole:
            return Qt::AlignCenter;

        default:
            return QAbstractItemModel::headerData(section, orientation, role);
    }
}

Qt::ItemFlags QSOTableModel::flags(const QModelIndex &index) const {
    return QAbstractTableModel::flags(index);
}

void QSOTableModel::initFields() {
    QSqlQuery sqlQuery;

    sqlQuery.prepare(SQL_QUERY_FIELDS);
    sqlQuery.bindValue(":table_name", MODEL_QSO_TABLE_NAME);
    sqlQuery.exec();

    int i = 0;
    while (sqlQuery.next()) {
        const QSqlRecord &record = sqlQuery.record();
        fieldNames.insert(i, record.value("name").toString());
        fieldTypes.insert(i, record.value("type").toString());
        fieldDescriptions.insert(i, record.value("description").toString());
        i++;
    }
}

QList<QString> QSOTableModel::columnTypes() {
    return QList<QString>(fieldTypes.values());
}

void QSOTableModel::registerMetaTypes() {
    qRegisterMetaType<ulog::models::QSOTableModel>("ulog::models::QSOTableModel");
    qRegisterMetaType<ulog::models::QSOTableModel *>("ulog::models::QSOTableModel *");
}
