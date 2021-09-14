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


#ifndef __ULOG__MODELS__QSO__TABLEMODEL__H
#define __ULOG__MODELS__QSO__TABLEMODEL__H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QAbstractTableModel>
#include <QtCore/QMap>
#include <QtCore/QMetaType>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>

#define MODEL_QSO_TABLE_NAME "qso"

namespace ulog::models {

    class QSOTableModel : public QAbstractTableModel {
    Q_OBJECT

    public:

        explicit QSOTableModel(QObject *parent = nullptr);

        QSOTableModel(const QSOTableModel &other);

        ~QSOTableModel() override;

        [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

        [[nodiscard]] int columnCount(const QModelIndex &parent) const override;

        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

        bool setData(const QModelIndex &index, const QVariant &value, int role) override;

        [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

        QList<QString> columnTypes();

        static void registerMetaTypes();

    public slots:

        void initFields();

    private:

        QMap<int, QString> fieldNames;
        QMap<int, QString> fieldTypes;
        QMap<int, QString> fieldDescriptions;

    };

}

Q_DECLARE_METATYPE(ulog::models::QSOTableModel)

#endif
