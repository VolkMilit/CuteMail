#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gen(new generate()),
    setting(new settings()),
    maild(new mailDir())
{
    ui->setupUi(this);

    populateTreeView();
    setupWebView();
    readSettings();

    fetch_proc = new QProcess(this);
}

MainWindow::~MainWindow()
{
    delete gen;
    delete fetch_proc;
    delete setting;
    delete maild;
    delete ui;
}

QString MainWindow::getCurrentAccount()
{
    QModelIndex index = ui->treeView->currentIndex();
    return index.data().toString();
}

// this will populate tree view with model, but I still can't off extra fields
void MainWindow::populateTreeView()
{
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(gen->getMailFolderPath());
    model->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(gen->getMailFolderPath()));

    // hide unnecessary column
    for (int i = 1; i < 4; i++)
        ui->treeView->setColumnHidden(i, true);
}

// there will be all webview-related settings
// todo: probably javascript still may be executing, need to off it down
void MainWindow::setupWebView()
{
    ui->webView->settings()->setMaximumPagesInCache(0);
    ui->webView->settings()->setOfflineStorageDefaultQuota(0);
    ui->webView->settings()->setObjectCacheCapacities(0, 0, 0);
    ui->webView->settings()->setOfflineWebApplicationCacheQuota(0);
}

void MainWindow::readSettings()
{
    const QString dem = setting->getWindowDemention();
    const QString max = setting->getWindowFullscreen();
    const QString column = setting->getTableHeadersWight();
    const QStringList column_list = column.split(",");

    this->resize(dem.split("x")[0].toInt(), dem.split("x")[1].toInt());
    if (max.toInt() == 1) this->showMaximized();

    for (int i = 0; i < column_list.length(); i++)
        ui->tb_mails->setColumnWidth(i, column_list.at(i).toInt());
}

void MainWindow::writeSettings()
{
    QString column_str;

    for (int i = 0; i < ui->tb_mails->columnCount(); i++)
        column_str += QString::number(ui->tb_mails->columnWidth(i)) + ",";

    setting->setTableHeadersWight(column_str);
    //setting->setSplitterSizes(ui->splitter->w);
    setting->setWindowDemention(QString::number(MainWindow::width()) + "x" + QString::number(MainWindow::height()));
    setting->setWindowFullscreen(QString::number(this->isMaximized()));
}

void MainWindow::populateTable()
{
    ui->tb_mails->setRowCount(0); // clear table

    tmp.clear();
    tmp.append(maild->scanDir(gen->getMailFolderPath() + getCurrentAccount() + "/incoming"));

    for (int i = 0; i < tmp.size(); i++)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem;
        item1->setText(readmailbox->readSubject(tmp.at(i)));

        QTableWidgetItem *item2 = new QTableWidgetItem;
        item2->setText(readmailbox->readFrom(tmp.at(i)));

        QTableWidgetItem *item3 = new QTableWidgetItem;
        item3->setText(readmailbox->readDate(tmp.at(i)));

        ui->tb_mails->insertRow(i);

        ui->tb_mails->setItem(i, 0, item1);
        ui->tb_mails->setItem(i, 1, item2);
        ui->tb_mails->setItem(i, 2, item3);
    }
}

void MainWindow::refresh()
{
    gen->fetchmailDelete();
    gen->procmailDelete();
    gen->deleteExecutable();
    populateTable();
}

void MainWindow::on_tb_mails_itemClicked(QTableWidgetItem *item)
{
    // temporary solution for a couple of version,
    // in in the next version it will be replaced by
    // someting else, but no browser
    ui->webView->setUrl("file://" + tmp.at(item->row()));
}

void MainWindow::on_actionFetch_mail_triggered()
{
    QStringList account = getCurrentAccount().split("@");
    connect(fetch_proc, SIGNAL(finished(int)), this, SLOT(refresh())); // doesn't work? wtf?!
    askForPassword(account.at(1), "pop3", account.at(0));
    fetch_proc->startDetached(setting->getSettingsPath() + "start.sh");
}

void MainWindow::askForPassword(QString server, QString protocol, QString username)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("CuteMail"),
                                            tr("Enter password for "), QLineEdit::Password,
                                            "", &ok);

    if (ok && !text.isEmpty())
    {
        gen->fetchmailConfig(server, protocol, username, text);
        gen->procmailConfig(getCurrentAccount()); // need to get parent, when focuse on incoming or trash or junk
        gen->mhaExecutable(getCurrentAccount());
    }
    else
    {
        QMessageBox::critical(this, "CuteMail", "Can't connect to " + getCurrentAccount(), QMessageBox::Ok);
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{    
    populateTable();
    setWindowTitle(getCurrentAccount() + " - CuteMail");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event = nullptr; // shut up the compiller, whithout parametr function just not working
    writeSettings();
}
