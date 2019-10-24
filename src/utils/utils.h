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

#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QTextStream>

//#include <mimetic/mimetic.h>
#include <iostream>
#include <vector>

class utils
{
public:
    utils();
    virtual ~utils();

protected:
    /*QString decodeQP(QString str);
    std::string decodeQP(const std::string &str);

    QString decodeB64(QString str);
    std::string decodeB64(const std::string &str);*/

    const char *toCChar(QString qstr);
    std::vector<std::string> stdSplit(const std::string &str, char delim);

    QTextStream &QStdErr();

    QString toCamelString(const QString &s);
    QString removeLastStrSymbol(const QString &str, const char s);
    QString replaceLastStrSymbol(const QString &str, const char s, const char ss);
};

#endif // UTILS_H
