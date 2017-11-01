#include "accountswindow.h"
#include "ui_accountswindow.h"
#include <QDebug>

accountsWindow::accountsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::accountsWindow),
    setting(new settings()),
    gen(new generate())
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
    if (!ui->le_login->text().isEmpty() && !ui->le_server->text().isEmpty())
        gen->accauntsFolders(ui->le_login->text() + "@" + ui->le_server->text());

    ui->lw_accounts->clear();
    populateAccountsList();
}

void accountsWindow::populateAccountsList()
{
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
    this->hide();
}
