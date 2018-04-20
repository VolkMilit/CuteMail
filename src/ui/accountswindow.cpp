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

#include "accountswindow.h"
#include "ui_accountswindow.h"
#include <QDebug>

accountsWindow::accountsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::accountsWindow),
    setting(new settings()),
    gen(new cmgenerate())
{
    ui->setupUi(this);

    this->setWindowTitle("Manage accounts");
    populateAccountsList();
}

accountsWindow::~accountsWindow()
{
    prevAccount.clear();

    delete setting;
    delete gen;
    delete ui;
}

void accountsWindow::writeAccount()
{
    if (prevAccount.isEmpty()) return;

    setting->setProxyString(ui->le_proxy->text(), prevAccount);
    setting->setPort(ui->le_port->text(), prevAccount);
    setting->setUseSSL(ui->cb_usessl->isChecked() ? "1" : "0", prevAccount);
}

void accountsWindow::readAccount()
{
    const QString currentAccount = ui->lw_accounts->currentItem()->text();
    ui->le_proxy->setText(setting->getProxyString(currentAccount));
    ui->le_port->setText(setting->getPort(currentAccount));
    ui->cb_usessl->setChecked(setting->getUseSSL(currentAccount).toInt() ? true : false);
}

void accountsWindow::on_btn_new_clicked()
{
    newaccdialog dialog;
    dialog.setModal(true);
    dialog.exec();
    dialog.done(0);

    populateAccountsList();
}

void accountsWindow::populateAccountsList()
{
    ui->lw_accounts->clear();

    QDir dir(gen->getMailFolderPath());
    const QStringList dirs = dir.entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (QString item, dirs)
        ui->lw_accounts->addItem(item);
}

void accountsWindow::on_lw_accounts_clicked(const QModelIndex &index)
{
    writeAccount();
    readAccount();

    const QStringList account = index.data().toString().split("@");

    if (account.size() < 2)
    {
        qDebug() << "CuteMail error: incorrect account";
        return;
    }

    ui->le_login->setText(account.at(0));
    ui->le_server->setText(account.at(1));

    prevAccount = index.data().toString();
}

void accountsWindow::on_buttonBox_rejected()
{
    writeAccount();
    this->close();
}

void accountsWindow::on_btn_delete_clicked()
{
    QListWidgetItem *item = ui->lw_accounts->currentItem();

    QMessageBox msgBox;
    msgBox.setModal(true);
    msgBox.setText("Are you sure you want to delete " + item->text() + "? Warning! All data will be deleted too!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok)
    {
        QDir dir(gen->getMailFolderPath() + item->text());
        dir.removeRecursively();
    }
    else
    {
        return;
    }
}
