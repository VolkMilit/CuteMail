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

#ifndef GENERATE_H
#define GENERATE_H

#include <QString>
#include <QFile>
#include <QDir>

#include "settings.h"

class cmgenerate
{
public:
    cmgenerate();
    ~cmgenerate();
    void accauntsFolders(QString account_name);
    QString getMailFolderPath();

private:
    QString mailFolderPath;

    settings *setting;
};

#endif // GENERATE_H
