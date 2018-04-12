#include "settingsdialog.h"
#include "ui_settingsdialog.h"

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog),
    setting(new settings)
{
    ui->setupUi(this);

    if (setting->getDisplayMessageOnce() == 0)
        displayonce = false;
    else
        displayonce = true;

    ui->le_path_mailbox->setText(QDir::homePath() + "/mail");
    readSettings();
}

settingsDialog::~settingsDialog()
{
    delete setting;
    delete ui;
}

void settingsDialog::readSettings()
{
    ui->le_path_mailbox->setText(setting->getMailDir());
    ui->cb_alwayswebview->setChecked(setting->getAlwaysUseWebview() ? true : false);
    ui->cb_automaticallywebview->setChecked(setting->getUseWebviewAutomatically() ? true : false);
    ui->gb_checknew->setChecked(setting->getCheckForNewMail() ? true : false);
    ui->sp_seconds_tocheck->setValue(setting->getCheckAfter());
    ui->cb_trayicon->setChecked(setting->getDisplayTray() ? true : false);
    ui->cb_notify->setChecked(setting->getDisplayNotify() ? true : false);
}

void settingsDialog::writeSettings()
{
    setting->setMailDir(ui->le_path_mailbox->text());
    setting->setAlwaysUseWebview(ui->cb_alwayswebview->isChecked() ? 1 : 0);
    setting->setUseWebviewAutomatically(ui->cb_automaticallywebview->isChecked() ? 1 : 0);
    setting->setCheckForNewMail(ui->gb_checknew->isChecked() ? 1 : 0);
    setting->setCheckAfter(ui->sp_seconds_tocheck->value());
    setting->setDisplayTray(ui->cb_trayicon->isChecked() ? 1 : 0);
    setting->setDisplayNotify(ui->cb_notify->isChecked() ? 1 : 0);
}

void settingsDialog::on_cb_alwayswebview_clicked()
{
    if (!displayonce)
    {
        QMessageBox::warning(this,\
                         "CuteMail - warning",\
                         "Note that webview might be 'no go' for privacy concerns."
                         "Still many mail used html to display content. This message appears only once.",\
                         QMessageBox::Ok
                         );

        setting->setDisplayMessageOnce(1);
    }
}

void settingsDialog::on_cb_automaticallywebview_clicked()
{
    if (!displayonce)
    {
        QMessageBox::warning(this,\
                         "CuteMail - warning",\
                         "Note that webview might be 'no go' for privacy concerns."
                         "Still many mail used html to display content. This message appears only once.",\
                         QMessageBox::Ok
                         );

        setting->setDisplayMessageOnce(1);
    }
}

void settingsDialog::on_buttonBox_accepted()
{
    writeSettings();
}

void settingsDialog::on_buttonBox_rejected()
{
    this->close();
}
