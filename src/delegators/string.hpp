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

#ifndef __ULOG__DELEGATORS__STRING__H
#define __ULOG__DELEGATORS__STRING__H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>

#include <QtWidgets/QWidget>
#include <QtWidgets/QStyleOptionViewItem>
#include <QtWidgets/QStyledItemDelegate>

namespace ulog::delegators {

    class StringDelegate : public QStyledItemDelegate {
    Q_OBJECT

    public:

        explicit StringDelegate(QObject *parent = nullptr);

        ~StringDelegate() override;

        QWidget *createEditor(QWidget *parent,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

        void setEditorData(QWidget *editor, const QModelIndex &index) const override;

        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

        void updateEditorGeometry(QWidget *editor,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const override;



        [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    };

}

#endif
