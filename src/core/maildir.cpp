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

#include "maildir.h"
#include <QDebug>

mailDir::mailDir()
    : gen(new cmgenerate())
{

}

mailDir::~mailDir()
{
    delete gen;
}

QVector<QString> mailDir::scanDir(QString dir)
{
    QVector<QString> tmp;

    QDirIterator it(dir, QStringList() << "*.eml", QDir::Files);
    while (it.hasNext())
        tmp.push_back(it.next());

    std::sort(tmp.begin(), tmp.end());

    return tmp;
}

void mailDir::move(QString file_name, QString account_name, QString destenation_folder)
{
    const QString dir = gen->getMailFolderPath() + account_name + "/";
    QFile file(file_name);
    QFileInfo file_info(file_name);
    file.copy(file_name, dir + destenation_folder + "/" + file_info.baseName() + ".eml");
    file.remove();
}
