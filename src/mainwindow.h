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

//std headers
#include <algorithm>

#include "readmailbox.h"
#include "generate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tb_mails_itemClicked(QTableWidgetItem *item);
    void on_actionFetch_mail_triggered();
    void fetchDir(QString dir);
    void refresh();
    void on_treeView_clicked(const QModelIndex &index);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void askForPassword(QString server, QString protocol, QString username);
    void setupWindow();
    void setupWebView();
    void populateTreeView();
    void readSettings();
    void writeSettings();

    Ui::MainWindow *ui;
    readMailbox *readmailbox;
    generate *gen;
    settings *setting;

    QProcess *fetch_proc;
};

#endif // MAINWINDOW_H
