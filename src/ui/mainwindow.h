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
#include <QDesktopServices>
#include <QWebPage>
#include <QPointer>

#include "core/generate.h"
#include "core/maildir.h"
#include "core/emlparser.h"
#include "core/mailfetch.h"
#include "core/database.h"
#include "ui/accountswindow.h"
#include "ui/settingsdialog.h"
#include "ui/aboutdialog.h"
#include "ui/searchdialog.h"

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
    void on_actionDelete_triggered();
    void on_actionRestore_triggered();
    void on_actionManage_accounts_triggered();
    void on_treeWidget_itemSelectionChanged();
    void on_actionSettings_triggered();
    void on_actionActionUnsubscribe_triggered();
    void on_actionAdd_new_account_triggered();
    void on_actionAbout_CuteMail_triggered();
    void on_actionFind_triggered();

    void openExternal(const QUrl &url);
    void showTextMessage();
    void showFullMessage();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void populateTable();
    void askForPassword(QString server, QString protocol, QString username);
    void setupView();
    void populateTreeWidget();
    void readSettings();
    void writeSettings();
    void showSplash(const QString &str, const QString &btnstr);

    QStringList getCurrentAccount();

    Ui::MainWindow *ui;
    cmgenerate *gen;
    settings *setting;
    mailDir *maild;

    QVector<QString> tmp;

    int unseencount;
};

#endif // MAINWINDOW_H
