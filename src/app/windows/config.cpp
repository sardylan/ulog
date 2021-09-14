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

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QComboBox>

#include <QtSql/QSqlDatabase>

#include <hamlib/riglist.h>

#include <rigctl/models.hpp>

#include "config.hpp"
#include "ui_config.h"

using namespace ulog::app;
using namespace ulog::app::windows;

Config::Config(config::Config *config, QWidget *parent) : QDialog(parent), ui(new Ui::Config) {
    ui->setupUi(this);

    Config::config = config;
    models = rigctl::models();

    connectSignals();
    initUi();
    valuesLoad();
}

Config::~Config() {
    delete ui;
}

void Config::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Config::valuesSave);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Config::close);

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &Config::valuesSave);
    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &Config::close);
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &Config::valuesLoad);

    connect(ui->dbTypeComboBox, &QComboBox::currentIndexChanged, this, &Config::updateDbFieldsVisibility);
}

void Config::initUi() {
    qInfo() << "Initalizing UI";

    qDebug() << "Initializing Rig Model Combo Box";
    initDbTypeComboBox();

    initRigModelComboBox();
    initRigSerialPortComboBoxes();
}

void Config::initDbTypeComboBox() {
    ui->dbTypeComboBox->clear();

    const QStringList &sqlDrivers = QSqlDatabase::drivers();

    for (const QString &sqlDriver: sqlDrivers) {
        QString text;

        if (sqlDriver == "QDB2")
            text = "IBM DB2";
        else if (sqlDriver == "QIBASE")
            text = "Borland InterBase";
        else if (sqlDriver == "QMYSQL")
            text = "MySQL";
        else if (sqlDriver == "MARIADB")
            text = "MariaDB";
        else if (sqlDriver == "QOCI")
            text = "Oracle Call Interface Driver";
        else if (sqlDriver == "QODBC")
            text = "Open Database Connectivity (ODBC)";
        else if (sqlDriver == "QPSQL")
            text = "PostgreSQL";
        else if (sqlDriver == "QSQLITE2")
            text = "SQLite 2";
        else if (sqlDriver == "QSQLITE")
            text = "SQLite";
        else if (sqlDriver == "QTDS")
            text = "Sybase Adaptive Server";

        if (text.isEmpty())
            continue;

        ui->dbTypeComboBox->addItem(text, sqlDriver);
    }
}

void Config::updateDbFieldsVisibility() {
    bool dbIsFile = false;

    if (ui->dbTypeComboBox->currentData().toString() == "QSQLITE" ||
        ui->dbTypeComboBox->currentData().toString() == "QSQLITE2")
        dbIsFile = true;

    ui->dbFileLayout->setEnabled(dbIsFile);
    ui->dbFileLabel->setEnabled(dbIsFile);
    ui->dbFileValue->setEnabled(dbIsFile);
    ui->dbFileButton->setEnabled(dbIsFile);

    ui->dbHostLabel->setEnabled(!dbIsFile);
    ui->dbHostValue->setEnabled(!dbIsFile);
    ui->dbPortLabel->setEnabled(!dbIsFile);
    ui->dbPortValue->setEnabled(!dbIsFile);
    ui->dbUsernameLabel->setEnabled(!dbIsFile);
    ui->dbUsernameValue->setEnabled(!dbIsFile);
    ui->dbPasswordLabel->setEnabled(!dbIsFile);
    ui->dbPasswordValue->setEnabled(!dbIsFile);
    ui->dbNameLabel->setEnabled(!dbIsFile);
    ui->dbNameValue->setEnabled(!dbIsFile);
    ui->dbTestButton->setEnabled(!dbIsFile);
}

void Config::initRigModelComboBox() {
    ui->rigctlSerialPortModelComboBox->clear();

    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DUMMY);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_NETRIGCTL);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ARMSTRONG);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FLRIG);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT847);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT1000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT1000D);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT1000MPMKV);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT747);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT757);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT757GXII);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT575);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT767);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT736R);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT840);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT820);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT900);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT920);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT890);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT990);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FRG100);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FRG9600);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FRG8800);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT817);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT100);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT857);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT897);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT1000MP);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT1000MPMKVFLD);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_VR5000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT450);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT950);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT2000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT9000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT980);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FTDX5000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_VX1700);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FTDX1200);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT991);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT891);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FTDX3000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT847UNI);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT600);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FTDX101D);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT818);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FTDX10);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FT897D);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FTDX101MP);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS50);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS440);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS450S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS570D);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS690S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS711);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS790);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS811);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS850);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS870S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS940);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS950S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS950SDX);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS2000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_R5000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS570S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_THD7A);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_THD7AG);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_THF6A);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_THF7E);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_K2);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS930);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_THG71);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS680S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS140S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TMD700);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TMV7);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS480);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_K3);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TRC80);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS590S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TRANSFOX);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_THD72A);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TMD710);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TMV71);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_F6K);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS590SG);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_XG3);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS990S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_HPSDR);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TS890S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_THD74);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_K3S);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_KX2);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_KX3);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_PT8000A);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_K4);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_POWERSDR);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_MALACHITE);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC1271);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC1275);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC271);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC275);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC375);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC471);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC475);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC575);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC706);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC706MKII);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC706MKIIG);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC707);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC718);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC725);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC726);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC728);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC729);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC731);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC735);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC736);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC737);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC738);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC746);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC751);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC751A);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC756);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC756PRO);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC761);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC765);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC775);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC781);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC820);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC821H);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC970);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR10);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR71);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR72);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR75);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR7000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR7100);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR8500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR9000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC910);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC78);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC746PRO);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC756PROII);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICID1);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC703);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC7800);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC756PROIII);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR20);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC7000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC7200);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC7700);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC7600);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC92D);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR9500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC7410);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC9100);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICRX7);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC7100);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ID5100);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC2730);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC7300);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_PERSEUS);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC785x);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_X108G);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR6);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC7610);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR8600);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ICR30);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC9700);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ID4100);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ID31);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ID51);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_IC705);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_MINISCOUT);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_XPLORER);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_OS535);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_OS456);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_OMNIVI);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_OMNIVIP);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_PARAGON2);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DELTAII);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_PCR1000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_PCR100);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_PCR1500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_PCR2500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR8200);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR8000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR7030);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR5000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR3030);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR3000A);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR3000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR2700);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR2500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR16);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_SDU5500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_SDU5000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR8600);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR5000A);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_AR7030P);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_SR2200);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_JST145);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_JST245);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_CMH530);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_NRD345);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_NRD525);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_NRD535);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_NRD545);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RS64);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RS2005);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RS2006);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RS2035);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RS2042);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RS2041);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BC780);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BC245);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BC895);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_PRO2052);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BC235);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BC250);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BC785);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BC786);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BCT8);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BCD396T);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BCD996T);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_BC898);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DKR8);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DKR8A);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DKR8B);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_HF150);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_HF225);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_HF250);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_HF235);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RA3790);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RA3720);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RA6790);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RA3710);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RA3702);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_HF1000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_HF1000A);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_WJ8711);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_WJ8888);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ESM500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_EK890);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_EK891);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_EK895);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_EK070);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TRP7000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TRP8000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TRP9000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TRP8255);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_WR1000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_WR1500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_WR1550);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_WR3100);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_WR3150);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_WR3500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_WR3700);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_G303);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_G313);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_G305);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_G315);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TT550);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TT538);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RX320);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RX340);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RX350);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TT526);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TT516);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TT565);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TT585);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TT588);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RX331);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_TT599);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DX77);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DXSR8);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_505DSP);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_GNURADIO);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_MC4020);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_GRAUDIO);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_GRAUDIOIQ);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DSP10);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_SDR1000);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_SDR1000RFE);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DTTSP);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_EKD500);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ELEKTOR304);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DRT1);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DWT);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_USRP0);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_USRP);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_DDS60);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ELEKTOR507);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_MINIVNA);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_SI570AVRUSB);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_PMSDR);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_SI570PICUSB);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FIFISDR);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FUNCUBEDONGLE);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_HIQSDR);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_SI570PEABERRY1);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_SI570PEABERRY2);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_FUNCUBEDONGLEPLUS);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_RSHFIQ);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_V4L);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_V4L2);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_ESMC);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_EB200);
    addRigModel(ui->rigctlSerialPortModelComboBox, RIG_MODEL_XK2100);
}

void Config::initRigSerialPortComboBoxes() {
    qInfo() << "Initializing serial port widgets";

    qDebug() << "initializing portName ComboBox";
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator portIterator;
    ui->rigctlSerialPortNameComboBox->clear();
    for (portIterator = portList.begin(); portIterator != portList.end(); portIterator++) {
        QSerialPortInfo serialPortInfo = *portIterator;
        const QString &systemLocation = serialPortInfo.systemLocation();
        ui->rigctlSerialPortNameComboBox->addItem(systemLocation, systemLocation);
    }

    qDebug() << "initializing baudRate ComboBox";
    ui->rigctlSerialPortBaudRateComboBox->clear();
    ui->rigctlSerialPortBaudRateComboBox->addItem("1200 baud", QSerialPort::Baud1200);
    ui->rigctlSerialPortBaudRateComboBox->addItem("2400 baud", QSerialPort::Baud2400);
    ui->rigctlSerialPortBaudRateComboBox->addItem("4800 baud", QSerialPort::Baud4800);
    ui->rigctlSerialPortBaudRateComboBox->addItem("9600 baud", QSerialPort::Baud9600);
    ui->rigctlSerialPortBaudRateComboBox->addItem("19200 baud", QSerialPort::Baud19200);
    ui->rigctlSerialPortBaudRateComboBox->addItem("38400 baud", QSerialPort::Baud38400);
    ui->rigctlSerialPortBaudRateComboBox->addItem("57600 baud", QSerialPort::Baud57600);
    ui->rigctlSerialPortBaudRateComboBox->addItem("115200 baud", QSerialPort::Baud115200);

    qDebug() << "initializing dataBits ComboBox";
    ui->rigctlSerialPortDataBitsComboBox->clear();
    ui->rigctlSerialPortDataBitsComboBox->addItem("5 bits", QSerialPort::Data5);
    ui->rigctlSerialPortDataBitsComboBox->addItem("6 bits", QSerialPort::Data6);
    ui->rigctlSerialPortDataBitsComboBox->addItem("7 bits", QSerialPort::Data7);
    ui->rigctlSerialPortDataBitsComboBox->addItem("8 bits", QSerialPort::Data8);

    qDebug() << "initializing parity ComboBox";
    ui->rigctlSerialPortParityComboBox->clear();
    ui->rigctlSerialPortParityComboBox->addItem("None", QSerialPort::NoParity);
    ui->rigctlSerialPortParityComboBox->addItem("Even", QSerialPort::EvenParity);
    ui->rigctlSerialPortParityComboBox->addItem("Odd", QSerialPort::OddParity);
    ui->rigctlSerialPortParityComboBox->addItem("Space", QSerialPort::SpaceParity);
    ui->rigctlSerialPortParityComboBox->addItem("Mark", QSerialPort::MarkParity);

    qDebug() << "initializing stopBits ComboBox";
    ui->rigctlSerialPortStopBitsComboBox->clear();
    ui->rigctlSerialPortStopBitsComboBox->addItem("1 bit", QSerialPort::OneStop);
    ui->rigctlSerialPortStopBitsComboBox->addItem("1.5 bits", QSerialPort::OneAndHalfStop);
    ui->rigctlSerialPortStopBitsComboBox->addItem("2 bits", QSerialPort::TwoStop);

    qDebug() << "initializing flowControl ComboBox";
    ui->rigctlSerialPortFlowControlComboBox->clear();
    ui->rigctlSerialPortFlowControlComboBox->addItem("None", QSerialPort::NoFlowControl);
    ui->rigctlSerialPortFlowControlComboBox->addItem("Hardware (RTS-CTS)", QSerialPort::HardwareControl);
    ui->rigctlSerialPortFlowControlComboBox->addItem("Software (XON-XOFF)", QSerialPort::SoftwareControl);
}

void Config::addRigModel(QComboBox *comboBox, qint32 number) {
    QString name = models.value(number);

    if (name.length() > 0) {
        QString text = QString("%2 [%3]").arg(name).arg(number);
        comboBox->addItem(text, number);
    }
}

void Config::valuesLoad() {
    qInfo() << "Loading values";

    qDebug() << "Loading configuration";
    QMetaObject::invokeMethod(this, &Config::configLoad);

    qDebug() << "Setting initial values for DB";
    auto dbType = config->getDatabase()->getType();
    for (int i = 0; i < ui->dbTypeComboBox->count(); i++)
        if (ui->dbTypeComboBox->itemData(i).toString() == dbType) {
            ui->dbTypeComboBox->setCurrentIndex(i);
            break;
        }

    ui->dbFileValue->setText(config->getDatabase()->getFile());
    ui->dbHostValue->setText(config->getDatabase()->getHost());
    ui->dbPortValue->setText(QString::number(config->getDatabase()->getPort()));
    ui->dbUsernameValue->setText(config->getDatabase()->getUsername());
    ui->dbPasswordValue->setText(config->getDatabase()->getPassword());
    ui->dbNameValue->setText(config->getDatabase()->getFile());

    qDebug() << "Updating db fields";
    QMetaObject::invokeMethod(this, &Config::updateDbFieldsVisibility, Qt::QueuedConnection);

    qDebug() << "Selecting initial value for HamLib";
    auto rigCtlModel = config->getRigCtl()->getModel();
    for (int i = 0; i < ui->rigctlSerialPortModelComboBox->count(); i++)
        if (ui->rigctlSerialPortModelComboBox->itemData(i).value<QSerialPort::BaudRate>() == rigCtlModel) {
            ui->rigctlSerialPortModelComboBox->setCurrentIndex(i);
            break;
        }

    ui->rigctlSerialPortPollingIntervalSpinBox->setValue(config->getRigCtl()->getPollingInterval());
    ui->rigctlSerialPortCIVAddressValue->setText(QString::number(config->getRigCtl()->getIcomCiv(), 16));

    auto &rigCtlSerialPortName = const_cast<QString &>(config->getRigCtl()->getSerialPort()->getName());
    int index = -1;
    for (int i = 0; i < ui->rigctlSerialPortNameComboBox->count(); i++)
        if (ui->rigctlSerialPortNameComboBox->itemData(i).toString() == rigCtlSerialPortName) {
            index = i;
            break;
        }
    if (index != -1) {
        ui->rigctlSerialPortNameComboBox->setCurrentIndex(index);
    } else {
        ui->rigctlSerialPortNameComboBox->setItemText(0, rigCtlSerialPortName);
        ui->rigctlSerialPortNameComboBox->setItemData(0, rigCtlSerialPortName);
        ui->rigctlSerialPortNameComboBox->setCurrentIndex(0);
    }

    auto rigCtlSerialPortBaudRate = config->getRigCtl()->getSerialPort()->getBaudRate();
    for (int i = 0; i < ui->rigctlSerialPortBaudRateComboBox->count(); i++)
        if (ui->rigctlSerialPortBaudRateComboBox->itemData(i).value<QSerialPort::BaudRate>() ==
            rigCtlSerialPortBaudRate) {
            ui->rigctlSerialPortBaudRateComboBox->setCurrentIndex(i);
            break;
        }

    auto rigCtlSerialPortDataBits = config->getRigCtl()->getSerialPort()->getDataBits();
    for (int i = 0; i < ui->rigctlSerialPortDataBitsComboBox->count(); i++)
        if (ui->rigctlSerialPortDataBitsComboBox->itemData(i).value<QSerialPort::DataBits>() ==
            rigCtlSerialPortDataBits) {
            ui->rigctlSerialPortDataBitsComboBox->setCurrentIndex(i);
            break;
        }

    auto rigCtlSerialPortParity = config->getRigCtl()->getSerialPort()->getParity();
    for (int i = 0; i < ui->rigctlSerialPortParityComboBox->count(); i++)
        if (ui->rigctlSerialPortParityComboBox->itemData(i).value<QSerialPort::Parity>() == rigCtlSerialPortParity) {
            ui->rigctlSerialPortParityComboBox->setCurrentIndex(i);
            break;
        }

    auto rigCtlSerialPortStopBits = config->getRigCtl()->getSerialPort()->getStopBits();
    for (int i = 0; i < ui->rigctlSerialPortStopBitsComboBox->count(); i++)
        if (ui->rigctlSerialPortStopBitsComboBox->itemData(i).value<QSerialPort::StopBits>() ==
            rigCtlSerialPortStopBits) {
            ui->rigctlSerialPortStopBitsComboBox->setCurrentIndex(i);
            break;
        }

    auto rigCtlSerialPortFlowControl = config->getRigCtl()->getSerialPort()->getFlowControl();
    for (int i = 0; i < ui->rigctlSerialPortFlowControlComboBox->count(); i++)
        if (ui->rigctlSerialPortFlowControlComboBox->itemData(i).value<QSerialPort::FlowControl>() ==
            rigCtlSerialPortFlowControl) {
            ui->rigctlSerialPortFlowControlComboBox->setCurrentIndex(i);
            break;
        }
}

void Config::valuesSave() {
    qInfo() << "Saving values";

    qDebug() << "Saving DB params";
    config->getDatabase()->setType(ui->dbTypeComboBox->currentData().toString());
    config->getDatabase()->setFile(ui->dbFileValue->text());
    config->getDatabase()->setHost(ui->dbHostValue->text());
    config->getDatabase()->setPort(ui->dbPortValue->text().toUShort());
    config->getDatabase()->setUsername(ui->dbUsernameValue->text());
    config->getDatabase()->setPassword(ui->dbPasswordValue->text());
    config->getDatabase()->setName(ui->dbNameValue->text());

    qDebug() << "Saving HamLib params";
    config->getRigCtl()->setModel(ui->rigctlSerialPortModelComboBox->currentData().value<qint32>());
    config->getRigCtl()->setPollingInterval(ui->rigctlSerialPortPollingIntervalSpinBox->value());
    config->getRigCtl()->setIcomCiv(ui->rigctlSerialPortCIVAddressValue->text().toUShort(nullptr, 16));
    config->getRigCtl()->getSerialPort()->setName(
            ui->rigctlSerialPortNameComboBox->currentData().value<QString>());
    config->getRigCtl()->getSerialPort()->setBaudRate(
            ui->rigctlSerialPortBaudRateComboBox->currentData().value<QSerialPort::BaudRate>());
    config->getRigCtl()->getSerialPort()->setDataBits(
            ui->rigctlSerialPortDataBitsComboBox->currentData().value<QSerialPort::DataBits>());
    config->getRigCtl()->getSerialPort()->setParity(
            ui->rigctlSerialPortParityComboBox->currentData().value<QSerialPort::Parity>());
    config->getRigCtl()->getSerialPort()->setStopBits(
            ui->rigctlSerialPortStopBitsComboBox->currentData().value<QSerialPort::StopBits>());
    config->getRigCtl()->getSerialPort()->setFlowControl(
            ui->rigctlSerialPortFlowControlComboBox->currentData().value<QSerialPort::FlowControl>());

    qDebug() << "Saving configuration";
    QMetaObject::invokeMethod(this, &Config::configSave, Qt::QueuedConnection);
}
