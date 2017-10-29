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
    tmp.clear(); // just in case

    delete fetch_proc;

    delete gen;
    delete setting;
    delete maild;
    delete ui;
}

QStringList MainWindow::getCurrentAccount()
{
    const QModelIndex index = ui->treeView->currentIndex();
    QStringList list;

    if (index.parent().row() > 0) // we're on child item
    {
        list.push_back(index.parent().data().toString());
        list.push_back(index.data().toString());
        return list;
    }
    else                          // we're on parent item
    {
        list.push_back(index.data().toString());
        return list;
    }
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
    setting->setWindowDemention(QString::number(MainWindow::width()) + "x" + QString::number(MainWindow::height()));
    setting->setWindowFullscreen(QString::number(this->isMaximized()));
    setting->setLastAccount(getCurrentAccount().at(0));
}

void MainWindow::populateTable()
{
    ui->tb_mails->setRowCount(0); // clear table

    tmp.clear();
    if (getCurrentAccount().count() > 1) // we're in specific folder (i.e. /incoming)
        tmp.append(maild->scanDir(gen->getMailFolderPath() + getCurrentAccount().at(0)
                                  + "/" + getCurrentAccount().at(1)));
    else                                 // we're just focuse on account name, so fallback to default
        tmp.append(maild->scanDir(gen->getMailFolderPath() + getCurrentAccount().at(0) + "/incoming"));

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

    ui->actionDelete->setEnabled(true);
    ui->actionRestore->setEnabled(true);
}

void MainWindow::on_actionFetch_mail_triggered()
{
    QString account = getCurrentAccount().at(0);
    QStringList account_parse = account.split("@");
    connect(fetch_proc, SIGNAL(finished(int)), this, SLOT(refresh())); // doesn't work? wtf?!
    askForPassword(account_parse.at(1), "pop3", account_parse.at(0));
    fetch_proc->startDetached(setting->getSettingsPath() + "start.sh");
}

void MainWindow::askForPassword(QString server, QString protocol, QString username)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("CuteMail"), \
                                            tr("Enter password for ") + getCurrentAccount().at(0), \
                                         QLineEdit::Password, "", &ok);

    if (ok && !text.isEmpty())
    {
        gen->fetchmailConfig(server, protocol, username, text);
        gen->procmailConfig(getCurrentAccount().at(0)); // need to get parent, when focuse on incoming or trash or junk
        gen->mhaExecutable(getCurrentAccount().at(0));
    }
    else
    {
        QMessageBox::critical(this, "CuteMail", tr("Couldn't connect to ") + getCurrentAccount().at(0), QMessageBox::Ok);
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    if (getCurrentAccount().size() > 1)
    {
        if (getCurrentAccount().at(1) == "trash")
            ui->actionRestore->setVisible(true);
        else
            ui->actionRestore->setVisible(false);
    }

    populateTable();
    setWindowTitle(getCurrentAccount().at(0) + " - CuteMail");
    ui->actionDelete->setEnabled(false);
    ui->actionRestore->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event = nullptr; // shut up the compiller, whithout parametr function just not working
    writeSettings();
}

void MainWindow::on_actionDelete_triggered()
{
    const int cr = ui->tb_mails->currentItem()->row();
    maild->move(tmp.at(cr), getCurrentAccount().at(0), "trash");
    tmp.remove(cr);
    ui->tb_mails->removeRow(cr);
}

void MainWindow::on_actionRestore_triggered()
{
    const int cr = ui->tb_mails->currentItem()->row();
    maild->move(tmp.at(cr), getCurrentAccount().at(0), "incoming");
    tmp.remove(cr);
    ui->tb_mails->removeRow(cr);
}
