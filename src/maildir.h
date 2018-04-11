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

#ifndef MAILDIR_H
#define MAILDIR_H

#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QVector>
#include <QDirIterator>

//std headers
#include <algorithm>

#include "generate.h"

class mailDir
{
public:
    mailDir();
    ~mailDir();

    QVector<QString> scanDir(QString dir);
    void move(QString file_name, QString account_name, QString destenation_folder);

private:
    cmgenerate *gen;
};

#endif // MAILDIR_H
