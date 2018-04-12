/*
    CuteMail - simple mail client for GNU/Linux.
    Copyright (C) 2018 Volk_Milit (aka Ja'Virr-Dar)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//qt headers
#include <QMainWindow>
#include <QDir>
#include <QDirIterator>
#include <QTableWidgetItem>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QItemSelection>
#include <QStandardItemModel>

#include "core/generate.h"
#include "core/maildir.h"
#include "core/emlparser.h"
#include "core/mailfetch.h"
#include "ui/accountswindow.h"
#include "ui/settingsdialog.h"

namespace Ui {class MainWindow;}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tb_mails_itemClicked(QTableWidgetItem *item);
    void on_actionFetch_mail_triggered();
    void populateTable();
    void on_actionDelete_triggered();
    void on_actionRestore_triggered();
    void on_actionManage_accounts_triggered();
    void on_bt_chngview_clicked();
    void on_treeWidget_itemSelectionChanged();
    void on_actionSettings_triggered();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void askForPassword(QString server, QString protocol, QString username);
    void setupWebView();
    void populateTreeWidget();
    void readSettings();
    void writeSettings();

    void showTextMessage(QTableWidgetItem *item);
    void showFullMessage(QTableWidgetItem *item);

    QStringList getCurrentAccount();

    Ui::MainWindow *ui;
    cmgenerate *gen;
    settings *setting;
    mailDir *maild;
    accountsWindow *accountswindow;
    settingsDialog *settingsdialog;

    QVector<QString> tmp;
};

#endif // MAINWINDOW_H
