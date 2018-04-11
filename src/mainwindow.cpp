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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gen(new cmgenerate()),
    setting(new settings()),
    maild(new mailDir()),
    accountswindow(new accountsWindow(parent)),
    qtreeviewhelper(new QTreeViewHelper())
{
    ui->setupUi(this);

    ui->fr_warning->hide();

    populateTreeView();
    setupWebView();
    readSettings();
    populateTable();
}

MainWindow::~MainWindow()
{
    tmp.clear(); // just in case

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

// this will populate tree view with model
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
    ui->webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
    ui->webView->settings()->setAttribute(QWebSettings::JavaEnabled, false);
    ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled, false);
    ui->webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, false);
    ui->webView->settings()->setMaximumPagesInCache(0);
    ui->webView->settings()->setOfflineStorageDefaultQuota(0);
    ui->webView->settings()->setObjectCacheCapacities(0, 0, 0);
    ui->webView->settings()->setOfflineWebApplicationCacheQuota(0);
    ui->webView->settings()->setDefaultTextEncoding("utf-8");

    ui->webView->hide();
}

void MainWindow::readSettings()
{
    if (!setting->settingsFile())
    {
        qDebug() << "Cannot read settings, create new one.";
        writeSettings();
    }

    const QString dem = setting->getWindowDemention();
    const QString max = setting->getWindowFullscreen();
    const QString column = setting->getTableHeadersWight();
    const QStringList column_list = column.split(",");

    this->resize(dem.split("x")[0].toInt(), dem.split("x")[1].toInt());
    if (max.toInt() == 1) this->showMaximized();

    for (int i = 0; i < column_list.length(); i++)
        ui->tb_mails->setColumnWidth(i, column_list.at(i).toInt());

    QModelIndex idx = 0;
    ui->treeView->setCurrentIndex(idx);
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

    for (auto i = 0; i < tmp.size(); i++)
    {
        emlparser eml(tmp.at(i));

        QTableWidgetItem *item1 = new QTableWidgetItem;
        item1->setText(eml.getSubject());

        QTableWidgetItem *item2 = new QTableWidgetItem;
        item2->setText(eml.getFrom());

        QTableWidgetItem *item3 = new QTableWidgetItem;
        item3->setText(eml.getDate());

        ui->tb_mails->insertRow(i);

        ui->tb_mails->setItem(i, 0, item1);
        ui->tb_mails->setItem(i, 1, item2);
        ui->tb_mails->setItem(i, 2, item3);
    }
}

void MainWindow::refresh()
{
    populateTable();
}

void MainWindow::showTextMessage(QTableWidgetItem *item)
{
    emlparser eml(this->tmp.at(item->row()));
    eml.generateTmpHtml();

    if (eml.isNoncompliantMail())
        ui->fr_warning->show();

    QFile ff(QDir::homePath() + "/.cache/cutemail-tmp.html");
    ff.open(QFile::ReadOnly | QFile::Text);
    QTextStream readFile(&ff);

    ui->textBrowser->clear();
    ui->textBrowser->append(readFile.readAll());
}

void MainWindow::showFullMessage(QTableWidgetItem *item)
{
    ui->fr_warning->hide();

    emlparser eml(this->tmp.at(item->row()));
    eml.generateTmpHtml();

    ui->webView->setUrl(QUrl("file://" + QDir::homePath() + "/.cache/cutemail-tmp.html"));

    ui->actionDelete->setEnabled(true);
    ui->actionRestore->setEnabled(true);
}

void MainWindow::on_tb_mails_itemClicked(QTableWidgetItem *item)
{
    ui->fr_warning->hide();
    ui->webView->hide();
    ui->textBrowser->show();
    showTextMessage(item);

    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.setPosition(0);
    ui->textBrowser->setTextCursor(cursor);
}

void MainWindow::on_actionFetch_mail_triggered()
{
    QString account = getCurrentAccount().at(0);
    QStringList account_parse = account.split("@");

    this->askForPassword(account_parse.at(1), "imap", account_parse.at(0));
}

void MainWindow::askForPassword(QString server, QString protocol, QString username)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("CuteMail"), \
                                            tr("Enter password for ") + getCurrentAccount().at(0), \
                                         QLineEdit::Password, "", &ok);

    if (ok && !text.isEmpty())
    {
        mailfetch mf(username + "@" + server, protocol + "." + server, text);
        //return;
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

void MainWindow::on_actionManage_accounts_triggered()
{
    accountswindow->show();
}

void MainWindow::on_bt_chngview_clicked()
{
    QTableWidgetItem *item = ui->tb_mails->currentItem();

    if (fullshowing == false)
    {
        ui->bt_chngview->setText("Hide full");
        fullshowing = true;

        ui->textBrowser->hide();
        ui->webView->show();
        showFullMessage(item);
    }
    else
    {
        ui->bt_chngview->setText("Show full");
        fullshowing = false;

        ui->webView->hide();
        ui->textBrowser->show();
        showTextMessage(item);
    }
}
