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

#include "readmailbox.h"
#include "generate.h"
#include "maildir.h"

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
    void populateTable();
    void refresh();
    void on_treeView_clicked(const QModelIndex &index);

    void on_actionDelete_triggered();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void askForPassword(QString server, QString protocol, QString username);
    void setupWebView();
    void populateTreeView();
    void readSettings();
    void writeSettings();
    QStringList getCurrentAccount();

    Ui::MainWindow *ui;
    readMailbox *readmailbox;
    generate *gen;
    settings *setting;
    mailDir *maild;

    QProcess *fetch_proc;

    QVector<QString> tmp;
};

#endif // MAINWINDOW_H
