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


#ifndef __ULOG__APP__WINDOWS__LICENSE__H
#define __ULOG__APP__WINDOWS__LICENSE__H

#include <QtWidgets/QDialog>

namespace Ui {
    class License;
}

namespace ulog::app::windows {

    class License : public QDialog {
    Q_OBJECT

    public:

        explicit License(QWidget *parent = nullptr);

        ~License() override;

    private:

        Ui::License *ui;

        void connectSignals();

        void initUi();

        static QByteArray readFile(const QString &path);

    };

}

#endif
