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

#include "utils.h"

utils::utils(){}
utils::~utils(){}

QTextStream &utils::QStdErr()
{
    static QTextStream ts(stderr);
    return ts;
}

// https://wiki.qt.io/Converting_Strings_from_and_to_Camel_Case
QString utils::toCamelString(const QString &s)
{
    QStringList parts = s.split(' ', QString::SkipEmptyParts);
    for (int i=1; i<parts.size(); ++i)
        parts[i].replace(0, 1, parts[i][0].toUpper());

    return parts.join("");
}

QString utils::removeLastStrSymbol(const QString &str, const char s)
{
    QString tmp = str;

    if (tmp.at(tmp.length()-1) == s)
        tmp.remove(tmp.length()-1, tmp.length()-1);

    return tmp;
}

QString utils::replaceLastStrSymbol(const QString &str, const char s, const char ss)
{
    QString tmp = str;

    if (tmp.at(tmp.length()-1) == s)
        tmp.replace(tmp.length()-1, tmp.length()-1, ss);

    return tmp;
}
