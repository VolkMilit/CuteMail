#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gen(new generate()),
    setting(new settings())
{
    ui->setupUi(this);

    setupWindow();
    populateTreeView();
    setupWebView();

    fetch_proc = new QProcess(this);
}

MainWindow::~MainWindow()
{
    delete gen;
    delete fetch_proc;
    delete ui;
}

// this will populate tree view with model, but I still can't off extra fields
void MainWindow::populateTreeView()
{
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::homePath() + "/mail/");
    model->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(QDir::homePath() + "/mail/"));
}

void MainWindow::setupWindow()
{
    // read settings
    // todo

    // I dunno, maybe it can be disabled throught gui in qtcreator?
    ui->tb_mails->setEditTriggers(QAbstractItemView::NoEditTriggers); // disable edit mode

    fetchDir(QDir::homePath() + "/mail/out"); // will be configurated
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

void MainWindow::fetchDir(QString dir)
{
    ui->tb_mails->setRowCount(0); // clear table

    QVector<QString> tmp;

    QDirIterator it(dir, QStringList() << "*.html", QDir::Files);
    while (it.hasNext())
        tmp.push_back(it.next());

    std::sort(tmp.begin(), tmp.end());

    for (int i = 0; i < tmp.size(); i++)
    {
        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setText(tmp.at(i));

        QTableWidgetItem *item1 = new QTableWidgetItem;
        item1->setText(readmailbox->readSubject(tmp.at(i)));

        QTableWidgetItem *item2 = new QTableWidgetItem;
        item2->setText(readmailbox->readFrom(tmp.at(i)));

        QTableWidgetItem *item3 = new QTableWidgetItem;
        item3->setText(readmailbox->readDate(tmp.at(i)));

        ui->tb_mails->insertRow(i);

        ui->tb_mails->setItem(i, 0, item0);
        ui->tb_mails->setItem(i, 1, item1);
        ui->tb_mails->setItem(i, 2, item2);
        ui->tb_mails->setItem(i, 3, item3);
    }
}

void MainWindow::refresh()
{
    gen->fetchmailDelete();
    gen->procmailDelete();
    gen->deleteExecutable();
    fetchDir(QDir::homePath() + "/mail/out");
}

void MainWindow::on_tb_mails_itemClicked(QTableWidgetItem *item)
{
    // temporary solution for a couple of version,
    // in in the next version it will be replaced by
    // someting else, but no browser
    ui->webView->setUrl("file://" + item->text());
}

void MainWindow::on_actionFetch_mail_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();

    const QString accaunt = index.data().toString();
    const QStringList tmp2 = accaunt.split("@");

    connect(fetch_proc, SIGNAL(finished(int)), this, SLOT(refresh())); // don't working? wtf?!
    askForPassword(tmp2.at(1), "pop3", tmp2.at(0));
    fetch_proc->startDetached(setting->getSettingsPath() + "start.sh");
}

void MainWindow::askForPassword(QString server, QString protocol, QString username)
{
    QModelIndex index = ui->treeView->currentIndex();

    bool ok;
    QString text = QInputDialog::getText(this, tr("CuteMail"),
                                            tr("Enter password for "), QLineEdit::Password,
                                            "", &ok);

    if (ok && !text.isEmpty())
    {
        gen->fetchmailConfig(server, protocol, username, text);
        gen->procmailConfig(index.data().toString()); // need to get parent, when focuse on incoming or trash or junk
        gen->mhaExecutable(index.data().toString());
    }
    else
    {
        QMessageBox::critical(this, "CuteMail", "Can't connect to " + username + "@" + server, QMessageBox::Ok);
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    fetchDir(QDir::homePath() + "/mail/" + index.data().toString() + "/incoming");
    setWindowTitle(index.data().toString() + " - CuteMail");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QVector<int> column;
    QVector<int> splitters;
    QVector<int> windowDem;

    // getting value
    for (int i = 0; i < ui->tb_mails->columnCount(); i++)
        column.push_back(ui->tb_mails->columnWidth(i));

    splitters.push_back(ui->splitter->height());
    splitters.push_back(ui->splitter_2->width());

    windowDem.push_back(MainWindow::width());
    windowDem.push_back(MainWindow::height());

    // setting value
    for (int i = 0; i < column.size(); i++)
        setting->setTableHeadersWight(QString::number(column.at(i)) + ",");

    setting->setSplitterSizes(QString::number(splitters.at(0)) + "," + QString::number(splitters.at(1)));
    setting->setWindowDemention(QString::number(windowDem.at(0)) + "x" + QString::number(windowDem.at(1)));
}
