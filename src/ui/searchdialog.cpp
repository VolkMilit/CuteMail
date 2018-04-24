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

#include "searchdialog.h"
#include "ui_searchdialog.h"

searchDialog::searchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Search...");
}

searchDialog::~searchDialog()
{
    delete ui;
}

int searchDialog::getCase()
{
    return ui->cb_case->isChecked() ? 1 : 0;
}

int searchDialog::getSearchby()
{
    return ui->comb_searchfor->currentIndex();
}

QString searchDialog::getString()
{
    return ui->le_searchfor->text();
}

int searchDialog::getFirstCoins()
{
    return ui->cb_firstcoinc->isChecked() ? 1 : 0;
}
