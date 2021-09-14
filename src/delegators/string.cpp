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


#include <QtWidgets/QLineEdit>

#include "string.hpp"

using namespace ulog::delegators;

StringDelegate::StringDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

StringDelegate::~StringDelegate() = default;

QWidget *StringDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
    Q_UNUSED(option)
    Q_UNUSED(index)

    auto *lineEdit = new QLineEdit(parent);
    return lineEdit;
}

void StringDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *lineEdit = dynamic_cast<QLineEdit *>(editor);
    auto value = index.model()->data(index, Qt::EditRole).value<QString>();
    lineEdit->setText(value);
}

void StringDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto lineEdit = dynamic_cast<QLineEdit *>(editor);
    QString data = lineEdit->text();
    model->setData(index, data);
}

void StringDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

QSize StringDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    const QSize &size = QStyledItemDelegate::sizeHint(option, index);
    return {size.width() + 20, size.height()};
}
