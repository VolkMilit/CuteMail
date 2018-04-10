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

#include "generate.h"
#include "maildir.h"
#include "accountswindow.h"
#include "qtreeviewhelper.h"
#include "emlparser.h"
#include "mailfetch.h"

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
    void refresh();
    void on_treeView_clicked(const QModelIndex &index);
    void on_actionDelete_triggered();
    void on_actionRestore_triggered();
    void on_actionManage_accounts_triggered();

    void on_bt_chngview_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void askForPassword(QString server, QString protocol, QString username);
    void setupWebView();
    void populateTreeView();
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
    QTreeViewHelper *qtreeviewhelper;

    QVector<QString> tmp;

    bool fullshowing = false;
};

#endif // MAINWINDOW_H
