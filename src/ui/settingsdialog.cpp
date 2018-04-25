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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog),
    setting(new settings)
{
    ui->setupUi(this);

    setupWindow();
    readSettings();
}

settingsDialog::~settingsDialog()
{
    delete setting;
    delete ui;
}

void settingsDialog::setupWindow()
{
    this->setWindowTitle("CuteMail settings");

    for (auto i = 0; i < ui->tb_shortcuts->rowCount(); i++)
        ui->tb_shortcuts->setCellWidget(i, 1, new QKeySequenceEdit);

    if (setting->getDisplayMessageOnce() == 0)
        displayonce = false;
    else
        displayonce = true;

    // default settings
    ui->le_path_mailbox->setText(QDir::homePath() + "/mail");
    ui->cb_usexdgbrowser->setChecked(true);

    // center align icons in listwidget
    for (auto i = 0; i < ui->lw_tabs->count(); i++)
    {
        ui->lw_tabs->item(i)->setSizeHint(QSize(100, 52));
        ui->lw_tabs->item(i)->setTextAlignment(Qt::AlignCenter);
    }
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
    ui->cb_usexdgbrowser->setChecked(setting->getUseXDGBrowser() ? true : false);


    QKeySequenceEdit *tmpks;
    QStringList sc = setting->readGroup(setting->getSettingsFilePath(), "Shortcuts");

    for (auto i = 0; i < ui->tb_shortcuts->rowCount(); i++)
    {
        QString item = toCamelString(ui->tb_shortcuts->item(i, 0)->text());
        tmpks = qobject_cast<QKeySequenceEdit*>(ui->tb_shortcuts->cellWidget(i, 1));

        for (auto s : sc)
        {
            if (s == item)
            {
                QString shortcut = setting->read(setting->getSettingsFilePath(), "Shortcuts", s);

                if (!shortcut.isEmpty())
                    tmpks->setKeySequence(QKeySequence(shortcut));
                else
                    break;
            }
        }
    }

    tmpks->deleteLater();
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
    setting->setUseXDGBrowser(ui->cb_usexdgbrowser->isChecked() ? 1 : 0);

    QKeySequenceEdit *tmpks;

    for (auto i = 0; i < ui->tb_shortcuts->rowCount(); i++)
    {
        QString item = toCamelString(ui->tb_shortcuts->item(i, 0)->text());
        tmpks = qobject_cast<QKeySequenceEdit*>(ui->tb_shortcuts->cellWidget(i, 1));
        QString value = tmpks->keySequence().toString();
        setting->write(setting->getSettingsPath() + "settings.ini", "Shortcuts", item, value);
    }

    tmpks->deleteLater();
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

void settingsDialog::on_lw_tabs_itemClicked()
{
     QModelIndex item = ui->lw_tabs->currentIndex();
     ui->stackedWidget->setCurrentIndex(item.row());
}
