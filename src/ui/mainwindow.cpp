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
    maild(new mailDir())
{
    ui->setupUi(this);

    populateTreeWidget();
    setupView();
    readSettings();
    populateTable();
    setupMainwindow();
}

MainWindow::~MainWindow()
{
    tmp.clear(); // just in case

    writeSettings();

    delete maild;
    delete gen;
    delete setting;
    delete ui;
}

void MainWindow::setupMainwindow()
{
    ui->fr_warning->hide();

    // search widget in toolbar
    QLineEdit *le_search = new QLineEdit;
    QWidget *empty = new QWidget;

    QString s_search = setting->read(setting->getSettingsFilePath(), "Shortcuts", "Search");
    QShortcut *sc_search = new QShortcut(QKeySequence(s_search), this);
    connect(sc_search, &QShortcut::activated, le_search, QOverload<>::of(&QLineEdit::setFocus));

    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    le_search->setPlaceholderText("Search... (" + s_search + ")");
    le_search->setClearButtonEnabled(true);
    connect(le_search, &QLineEdit::textChanged, this, &MainWindow::search);

    ui->toolBar->addWidget(empty);
    ui->toolBar->addWidget(le_search);
}

void MainWindow::setupShortcuts()
{
}

QStringList MainWindow::getCurrentAccount()
{
    const QModelIndex index = ui->treeWidget->currentIndex();
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

void MainWindow::populateTreeWidget()
{
    ui->treeWidget->clear();

    QDir dirs(gen->getMailFolderPath());
    QStringList acclist = dirs.entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);

    for (auto acc : acclist)
    {
        QDir subdir(dirs.path() + "/" + acc);
        QStringList sublist = subdir.entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, acc);

        for (auto accdirs : sublist)
        {
            QTreeWidgetItem *subitem = new QTreeWidgetItem;
            subitem->setText(0, accdirs);
            item->addChild(subitem);
        }
    }
}

void MainWindow::populateTable()
{
    database db(setting->getSettingsPath() + "accounts.db", getCurrentAccount().at(0));

    int unread_count = 0;
    QFont font;
    font.setBold(true);

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

        if (db.getValue(i+1, "status").isEmpty())
            db.addValue("unseen", "status");

        if (db.getValue(i+1, "status") == "unseen")
        {
            unread_count++;
            item1->setFont(font);
            item2->setFont(font);
            item3->setFont(font);
        }

        ui->tb_mails->setItem(i, 0, item1);
        ui->tb_mails->setItem(i, 1, item2);
        ui->tb_mails->setItem(i, 2, item3);
    }

    ui->statusBar->showMessage("Unreaded: " + QString::number(unread_count));
}

void MainWindow::showTextMessage()
{
    QTableWidgetItem *item = ui->tb_mails->currentItem();

    emlparser eml(this->tmp.at(item->row()));
    //database db(setting->getSettingsPath() + "accounts.db", getCurrentAccount().at(0));

    /*QString dbbody = db.getValue(item->row()+1, "body");

    if (dbbody.isEmpty())
    {
        db.overrideValue(item->row()+1, eml.getBody().first, "body");
        dbbody = eml.getBody().first;
    }*/

    if (!eml.getUsubscribelist().isEmpty())
        ui->actionActionUnsubscribe->setEnabled(true);
    else
        ui->actionActionUnsubscribe->setEnabled(false);

    if (eml.isNoncompliantMail())
    {
        connect(ui->bt_chngview, &QPushButton::clicked, this, &MainWindow::showFullMessage);
        showSplash("CuteMail detected this message can display"
                   "incorrectly. Click this button to show full"
                   "(using webview).", "Show full");
    }

    ui->textBrowser->clear();
    //ui->textBrowser->append(dbbody);
    ui->textBrowser->append(eml.getBody().first);

    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.setPosition(0);
    ui->textBrowser->setTextCursor(cursor);
}

void MainWindow::showFullMessage()
{
    QTableWidgetItem *item = ui->tb_mails->currentItem();

    ui->webView->show();
    ui->textBrowser->hide();
    ui->fr_warning->hide();

    emlparser eml(this->tmp.at(item->row()));

    /*database db(setting->getSettingsPath() + "accounts.db", getCurrentAccount().at(0));
    QString dbbody = db.getValue(item->row()+1, "bodyfull");

    if (dbbody.isEmpty())
    {
        emlparser eml(this->tmp.at(item->row()));

        if (!eml.getBody().second.isEmpty())
        {
            db.overrideValue(item->row()+1, eml.getBody().second, "bodyfull");
            dbbody = eml.getBody().second;
        }
        else
        {
            db.overrideValue(item->row()+1, eml.getBody().first, "bodyfull");
            dbbody = eml.getBody().first;
        }
    }*/

    //ui->webView->setHtml(dbbody);

    if (!eml.getBody().second.isEmpty())
        ui->webView->setHtml(eml.getBody().second);
    else
        ui->webView->setHtml(eml.getBody().first);
}

void MainWindow::on_tb_mails_itemClicked(QTableWidgetItem *item)
{
    qDebug() << this->tmp.at(item->row());

    ui->fr_warning->hide();
    ui->webView->hide();
    ui->textBrowser->show();

    ui->statusBar->showMessage("Loading...");

    showTextMessage();

    ui->statusBar->showMessage("Done.");

    ui->actionDelete->setEnabled(true);
    ui->actionRestore->setEnabled(true);


    QFont font;
    font.setBold(false);

    database db(setting->getSettingsPath() + "accounts.db", getCurrentAccount().at(0));
    db.overrideValue(item->row()+1, "read", "status");

    item->setFont(font);
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
    accountsWindow accountswindow;
    accountswindow.setModal(true);
    accountswindow.exec();
    accountswindow.done(0);

    populateTreeWidget();
}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    ui->treeWidget->setItemExpanded(item, true);

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

// there will be all webview-related settings
void MainWindow::setupView()
{
    ui->textBrowser->setOpenExternalLinks(true);

    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView, &QWebView::linkClicked, this, &MainWindow::openExternal);

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

void MainWindow::openExternal(const QUrl &url)
{
    if (setting->getUseXDGBrowser() == 1)
    {
        QDesktopServices::openUrl(url.toString());
    }
    else
    {
        ui->webView->load(url);
        connect(ui->bt_chngview, &QPushButton::clicked, this, &MainWindow::showFullMessage);
        showSplash("Using external browser is disabled. You can go back, click this button.", "Go back");
    }
}

void MainWindow::readSettings()
{
    if (!setting->settingsFile())
    {
        qDebug() << "Cannot read settings, create new one.";
        writeSettings();
    }

    if (setting->getWindowFullscreen() == 1)
    {
        this->showMaximized();
    }
    else
    {
        const QVector<int> dem = setting->getWindowDemention();
        this->resize(dem.at(0), dem.at(1));
    }

    QVector<int> column = setting->getTableHeadersWight();
    for (auto i = 0; i < ui->tb_mails->columnCount(); i++)
        ui->tb_mails->setColumnWidth(i, column.at(i));

    const QString last_acc = setting->getLastAccount();
    for (auto acc = 0; acc < ui->treeWidget->topLevelItemCount(); acc++)
    {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(acc);
        if (item->text(0) == last_acc)
        {
            ui->treeWidget->topLevelItem(acc)->setSelected(true);
            ui->treeWidget->topLevelItem(acc)->setExpanded(true);
            ui->treeWidget->setCurrentItem(item);
            break;
        }
    }

    setWindowTitle(getCurrentAccount().at(0) + " - CuteMail");
}

void MainWindow::writeSettings()
{
    QString column_str;

    for (int i = 0; i < ui->tb_mails->columnCount(); i++)
        column_str += QString::number(ui->tb_mails->columnWidth(i)) + ",";

    setting->setTableHeadersWight(column_str);
    setting->setWindowDemention(QString::number(MainWindow::width()) + "x" + QString::number(MainWindow::height()));
    setting->setWindowFullscreen(isMaximized());
    setting->setLastAccount(getCurrentAccount().at(0));
}

void MainWindow::on_actionSettings_triggered()
{
    settingsDialog settingsdialog;
    settingsdialog.setModal(true);
    settingsdialog.exec();
    settingsdialog.done(0);
}

void MainWindow::on_actionActionUnsubscribe_triggered()
{
    QTableWidgetItem *item = ui->tb_mails->currentItem();
    emlparser eml(this->tmp.at(item->row()));

    if (setting->getUseXDGBrowser() == 1)
        QDesktopServices::openUrl(eml.getUsubscribelist());
    else
    {
       ui->webView->show();
       ui->textBrowser->hide();
       ui->webView->setUrl(eml.getUsubscribelist());
    }
}

void MainWindow::showSplash(const QString &str, const QString &btnstr)
{
    if (ui->fr_warning->isHidden())
        ui->fr_warning->show();

    ui->lb_warning->setText(str);
    ui->bt_chngview->setText(btnstr);
}

void MainWindow::on_actionAdd_new_account_triggered()
{
    newaccdialog acc;
    acc.setModal(true);
    acc.exec();
    acc.done(0);

    populateTreeWidget();
}

void MainWindow::on_actionAbout_CuteMail_triggered()
{
    aboutDialog about;
    about.setModal(true);
    about.exec();
    about.done(0);
}

void MainWindow::search(const QString &text)
{
    int rowcount = ui->tb_mails->rowCount();

    if (text.isEmpty())
    {
        for (int i = 0; i < rowcount; i++)
            ui->tb_mails->setRowHidden(i, false);

        return;
    }

    for (auto i = 0; i < ui->tb_mails->rowCount(); i++)
    {
        QString megastring;
        QString str1 = ui->tb_mails->item(i, 0)->text();
        QString str2 = ui->tb_mails->item(i, 1)->text();
        megastring = str1 + " " + str2;

        if (!megastring.contains(text, Qt::CaseInsensitive))
            ui->tb_mails->hideRow(i);
    }
}

void MainWindow::on_actionFind_triggered()
{
    QItemSelectionModel *selectionModel = ui->tb_mails->selectionModel();
    QItemSelection itemSelection = selectionModel->selection();

    searchDialog search;
    search.setModal(true);

    if (search.exec())
    {
        ui->tb_mails->clearSelection();

        QString searchstr = search.getString();

        if (searchstr.isEmpty())
            return;

        int casesense = search.getCase();
        int searchby = search.getSearchby();
        int firstcoins = search.getFirstCoins();

        for (auto i = 0; i < ui->tb_mails->rowCount(); i++)
        {
            QString str1 = ui->tb_mails->item(i, 0)->text();
            QString str2 = ui->tb_mails->item(i, 1)->text();
            QString megastring = str1 + " " + str2;
            QString str;

            if (searchby == search.searchby::ALL)
                str = megastring;
            else if (searchby == search.searchby::SUBJECT)
                str = str1;
            else
                str = str2;

            if (str.contains(searchstr, casesense ? Qt::CaseSensitive : Qt::CaseInsensitive))
            {
                ui->tb_mails->selectRow(i);
                itemSelection.merge(selectionModel->selection(), QItemSelectionModel::Select);
                selectionModel->clearSelection();
                selectionModel->select(itemSelection, QItemSelectionModel::Select);

                if (firstcoins)
                    break;
            }
        }
    }

    search.done(0);
}
