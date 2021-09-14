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
#include <QtCore/QVariant>
#include <QtCore/QDateTime>

#include <QtWidgets/QDateTimeEdit>

#include "datetime.hpp"

using namespace ulog::delegators;

DateTimeDelegate::DateTimeDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

DateTimeDelegate::~DateTimeDelegate() = default;

QWidget *DateTimeDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
    Q_UNUSED(option)

    auto *dateTimeEdit = new QDateTimeEdit(parent);
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDisplayFormat(DELEGATOR_DATETIME_DATETIME_FORMAT);

    QVariant data = index.data();
    setDateTimeToWidget(dateTimeEdit, data);

    return dateTimeEdit;
}

void DateTimeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QVariant data = index.data();
    qDebug() << "setEditorData" << index.row() << index.column() << data;

    setDateTimeToWidget(editor, data);
}

void DateTimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto dateTimeEdit = dynamic_cast<QDateTimeEdit *>(editor);
    QDateTime data = dateTimeEdit->dateTime();

    qDebug() << "setModelData" << index.row() << index.column() << data;
    model->setData(index, data);
}

void DateTimeDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

QSize DateTimeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    const QSize &size = QStyledItemDelegate::sizeHint(option, index);
    return {size.width() + 20, size.height()};
}

QString DateTimeDelegate::displayText(const QVariant &value, const QLocale &locale) const {
    Q_UNUSED(locale)

    const QDateTime &dateTime = convertDataToDateTime(value);
    return dateTime.toString(DELEGATOR_DATETIME_DATETIME_FORMAT);
}

QDateTime DateTimeDelegate::convertDataToDateTime(const QVariant &value) {
    QDateTime dateTime;
    qlonglong rawTimestamp;
    QString isoDateTime;

    switch (value.type()) {
        case QVariant::DateTime:
            dateTime = value.toDateTime();
            break;

        case QVariant::String:
            isoDateTime = value.toString();
            dateTime = QDateTime::fromString(isoDateTime, Qt::ISODateWithMs);
            break;

        case QVariant::Int:
        case QVariant::LongLong:
            rawTimestamp = value.value<qlonglong>();
            dateTime = QDateTime::fromMSecsSinceEpoch(rawTimestamp, Qt::UTC);
            break;

        default:
            dateTime = QDateTime::currentDateTimeUtc();
            break;
    }

    return dateTime;
}

void DateTimeDelegate::setDateTimeToWidget(QWidget *widget, const QVariant &data) {
    const QDateTime &value = convertDataToDateTime(data);
    auto dateTimeWidget = qobject_cast<QDateTimeEdit *>(widget);
    dateTimeWidget->setDateTime(value);
}
