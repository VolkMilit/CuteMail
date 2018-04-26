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

/*QString utils::decodeQP(QString str)
{
    std::string str_std;
    mimetic::QP::Decoder qp;

    mimetic::code(str.toStdString().begin(), \
                  str.toStdString().end(), \
                  qp, \
                  std::back_inserter<std::string>(str_std));

    return QString::fromStdString(str_std);
}

std::string utils::decodeQP(const std::string &str)
{
    std::string str_std;
    mimetic::QP::Decoder qp;

    mimetic::code(str.begin(), \
                  str.end(), \
                  qp, \
                  std::back_inserter<std::string>(str_std));

    return str_std;
}

QString utils::decodeB64(QString str)
{
    std::string str_std;
    mimetic::Base64::Decoder base64;

    mimetic::code(str.toStdString().begin(), \
                  str.toStdString().end(), \
                  base64, \
                  std::back_inserter<std::string>(str_std));

    return QString::fromStdString(str_std);
}

std::string utils::decodeB64(const std::string &str)
{
    std::string str_std;
    mimetic::Base64::Decoder base64;

    mimetic::code(str.begin(), \
                  str.end(), \
                  base64, \
                  std::back_inserter<std::string>(str_std));

    return str_std;
}*/

/*
    Note that it is necessary to store the bytearray
    before you call data() on it,  a call like the following:
    const char *c_str2 = str2.toLatin1().data();
    will make the application crash as the QByteArray has not
    been stored and hence no longer exists

    from: https://stackoverflow.com/questions/5505221/converting-qstring-to-char
*/
const char *utils::toCChar(QString qstr)
{
    QByteArray qbraw = qstr.toLatin1();
    const char *cc_str = qbraw.data();

    return cc_str;
}

std::vector<std::string> utils::stdSplit(const std::string &str, char delim)
{
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;

    while (std::getline(ss, item, delim))
        elems.push_back(item);

    return elems;
}

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
