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

#ifndef __ULOG__APP__WINDOWS__MAIN__H
#define __ULOG__APP__WINDOWS__MAIN__H

#include <QtCore/QtGlobal>
#include <QtCore/QList>
#include <QtCore/QTimer>
#include <QtCore/QMap>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>

#include <QtSerialPort/QSerialPort>

#include <QtSql/QSqlTableModel>

#include <rigctl/update.hpp>

#include "config/config.hpp"

#define DATETIME_FORMAT "HH:mm:ss"

namespace Ui {
    class Main;
}

namespace ulog::app::windows {

    class Main : public QMainWindow {
    Q_OBJECT

    public:

        explicit Main(config::Config *config, QWidget *parent = nullptr);

        ~Main() override;

    public slots:

        void rigCtlConnecting();

        void rigCtlConnected(int vfoCount);

        void rigCtlDisconnecting();

        void rigCtlDisconnected();

        void setQsoTableModel(QSqlTableModel *qsoTableModel);

        void vfoUpdate(const ulog::rigctl::Update &update);

        void updateTableView();

    protected:

        void closeEvent(QCloseEvent *event) override;

    private:

        Ui::Main *ui;

        config::Config *config;

        QTimer *clockTimer;

        QLabel *statusBarRigCtlStatus;
        QLabel *statusBarRigCtlPollingInterval;
        QLabel *statusBarRigCtlModel;
        QLabel *statusBarRigCtlSerialPort;

        QMap<int, bool> pttStatus;

        int focusColumn;

        void connectSignals();

        void initUi();

        void initStatusBar();

        void startUi();

        void addStatusBarPermanentWidget(QLabel *widget);

        void setRigCtlFieldsVisibility(bool visible, int vfoCount = 0);

        void updateStrength(int vfoNum, int strength);

        static QString formatFrequency(quint64 frequency);

        static void setWidgetSizePolicy(QWidget *widget);

        static void updateModeLabel(QLabel *label, ulog::rigctl::Mode mode);

    private slots:

        void updateWindowsTitle();

        void updateClocks();

        void updateCatWidgets();

        void qsoRemoveConfirm();

        void focusOnLastQso();

    signals:

        void closeApplication();

        void displayConfig();

        void displayAbout();

        void displayLicense();

        void displayQtInfo();

        void rigCtlStart();

        void rigCtlStop();

        void modelReady();

        void qsoAdd();

        void qsoRemove(int id);

    };

}

#endif
