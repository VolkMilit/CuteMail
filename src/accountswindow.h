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

#ifndef ACCOUNTSWINDOW_H
#define ACCOUNTSWINDOW_H

#include <QDialog>

#include "settings.h"
#include "generate.h"

namespace Ui {
class accountsWindow;
}

class accountsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit accountsWindow(QWidget *parent = 0);
    ~accountsWindow();

private slots:
    void on_btn_new_clicked();
    void on_lw_accounts_clicked(const QModelIndex &index);
    void on_buttonBox_rejected();

private:
    void populateAccountsList();
    void writeAccount();
    void readAccount();

    Ui::accountsWindow *ui;

    settings *setting;
    cmgenerate *gen;

    QString prevAccount;
};

#endif // ACCOUNTSWINDOW_H
