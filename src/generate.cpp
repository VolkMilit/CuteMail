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

#include "generate.h"

cmgenerate::cmgenerate() :
    setting(new settings())
{
    mailFolderPath = QDir::homePath() + "/mail/"; // will be configurable
}

cmgenerate::~cmgenerate()
{
    delete setting;
}

QString cmgenerate::getMailFolderPath()
{
    return mailFolderPath;
}

void cmgenerate::accauntsFolders(QString account_name)
{
    QDir account(mailFolderPath + account_name);
    QDir account_incoming(mailFolderPath + account_name + "/incoming");
    QDir account_junk(mailFolderPath + account_name + "/junk");
    QDir account_trash(mailFolderPath + account_name + "/trash");

    if (!account.exists())
        account.mkdir(mailFolderPath + account_name);

    if (!account_incoming.exists())
        account.mkdir(mailFolderPath + account_name + "/incoming");

    if (!account_junk.exists())
        account.mkdir(mailFolderPath + account_name + "/junk");

    if (!account_trash.exists())
        account.mkdir(mailFolderPath + account_name + "/trash");
}
