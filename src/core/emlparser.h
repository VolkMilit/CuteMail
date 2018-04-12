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

#ifndef EMLPARSER_H
#define EMLPARSER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <QMessageBox>
#include <QTextCodec>

#include <mimetic/mimetic.h>
#include <iostream>
#include <vector>

#include "utils/utils.h"

class emlparser : private utils
{
public:
    emlparser(QString path);
    virtual ~emlparser();

    QString getTo();
    QString getFrom();
    QString getDate();
    QString getSubject();
    void generateTmpHtml();
    bool isNoncompliantMail();

private:
    QString msgfile;
    QString enconding;
    bool isnoncompliant;

    bool isMultipart();
    QString getBoundary();
    QString findBody();
    void decode();
    QString getHeaderValue(const std::string &field);
};

#endif // EMLPARSER_H
