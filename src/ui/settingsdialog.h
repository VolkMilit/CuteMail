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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDir>
#include <QTableWidgetItem>

#include "utils/settings.h"

namespace Ui { class settingsDialog; }

class settingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingsDialog(QWidget *parent = 0);
    ~settingsDialog();

private slots:
    void on_cb_alwayswebview_clicked();
    void on_cb_automaticallywebview_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_lw_tabs_itemClicked();

private:
    Ui::settingsDialog *ui;
    settings *setting;

    void readSettings();
    void writeSettings();

    void setupWindow();

    bool displayonce;
};

#endif // SETTINGSDIALOG_H
