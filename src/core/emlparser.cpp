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

#include "emlparser.h"
#include <QDebug>

emlparser::emlparser(QString path)
{
    this->msgfile = path;
    isnoncompliant = false;
    lastpos = 0;

    parseHeader();
}

emlparser::~emlparser(){}

QPair<QString, QString> emlparser::getBody()
{
    parseBody();

    QPair<QString, QString> ret;
    ret.first = this->body.first;
    ret.second = this->body.second;

    return ret;
}

QString emlparser::decodeall(const QString &str)
{
    if (!str.contains("=?UTF-8?", Qt::CaseInsensitive))
        return str;

    QString tmp = str;
    QString code = tmp.split('?').at(2);
    QString decoded;
    QStringList tmplist = tmp.split(' ');

    for (QString item : tmplist)
    {
        if (item.at(0) == '=' || (item.at(0) == '"' && item.at(1) == '='))
        {
            item.remove("=?UTF-8?" + code + "?", Qt::CaseInsensitive);
            item.remove("?=");
            item.remove("\"");

            // problems in some strings = symbol at the end
            item = removeLastStrSymbol(item, '=');

            if (code == "Q" || code == "q")
            {
                decoded += QuotedPrintable::decode(QVariant(item).toByteArray());
            }
            else
            {
                QByteArray btmp = QByteArray::fromBase64(item.toLatin1());
                decoded += QTextCodec::codecForMib(106)->toUnicode(btmp);
            }
        }
        else
        {
            decoded += item;
        }
    }

    return decoded;
}

void emlparser::parseHeader()
{
    QString line;

    QFile file(this->msgfile);

    if (!file.open(QFile::ReadOnly | QFile::Text) || !file.exists())
        return;

    QTextStream in(&file);

    while(!in.atEnd())
    {
        line = in.readLine();
        QStringList field = line.split(": ");

        if (field.at(0) == "Date")
            this->header.date = field.at(1);

        if (field.at(0) == "Delivered-To")
            this->header.to = decodeall(field.at(1));

        if (field.at(0) == "Subject")
        {
            this->header.subject = decodeall(field.at(1));
            int lastseek = in.pos();

            while (!in.atEnd())
            {
                line = in.readLine();

                if (line.at(0) == ' ')
                    this->header.subject.append(decodeall(line));
                else
                    break;
            }

            in.seek(lastseek);
        }

        if (field.at(0) == "From")
        {
            this->header.from = decodeall(field.at(1));
            int lastseek = in.pos();

            while (!in.atEnd())
            {
                line = in.readLine();

                if (line.at(0) == ' ')
                    this->header.from.append(decodeall(line));
                else
                    break;
            }

            in.seek(lastseek);
        }

        if (field.at(0) == "Return-path")
            this->header.returnpath = decodeall(field.at(1));

        if (field.at(0) == "Content-Transfer-Encoding")
            this->header.cte = field.at(1);

        if (field.at(0) == "Content-Type")
        {
            QString ct = field.at(1);
            this->header.ct = ct.split(";").at(0);
        }

        if (field.at(0) == "List-Unsubscribe")
        {
            QString uslst = field.at(1);
            uslst.remove("<");
            uslst.remove(">");

            QStringList lst = uslst.split(",");
            for (QString l : lst)
            {
                if (l.split(":").at(0) == "http" || l.split(":").at(0) == "https")
                {
                    uslst = l;
                    break;
                }
                else
                    uslst = l;
            }

            this->header.usubscribelist = uslst;
        }

        if (line.isEmpty())
        {
            lastpos = in.pos();
            break;
        }
    }

    file.close();
}

void emlparser::parseBody()
{
    int part = 0;
    QString enconding = this->header.cte;
    QString str_body1;
    QString str_body2;

    QString eof;
    QString line;

    QFile file(this->msgfile);

    if (!file.open(QFile::ReadOnly | QFile::Text) || !file.exists())
        return;

    QTextStream in(&file);

    in.seek(lastpos);

    while (!in.atEnd())
    {
        line = in.readLine();
        eof = "\n";

        if (line.at(0) == '-' && line.at(1) == '-')
        {
            part++;
            continue;
        }

        if (line.contains("Content-Type:") || line.contains("charset") && !line.contains("meta"))
            continue;

        if (line.contains("Content-Transfer-Encoding"))
        {
            if (enconding.isEmpty())
                enconding = line.split(":").at(1);

            continue;
        }

        if (line.at(line.length()-1) == '=')
        {
            line = removeLastStrSymbol(line, '=');
            eof = "";
        }

        if (part < 2)
            str_body1 += line + eof;
        else
            str_body2 += line + eof;
    }

    file.close();

    QString decoded1;
    QString decoded2;

    if (enconding.contains("quoted-printable"))
    {
        decoded1 = QuotedPrintable::decode(QVariant(str_body1).toByteArray());
        decoded2 = QuotedPrintable::decode(QVariant(str_body2).toByteArray());
    }
    else if (enconding.contains("base64"))
    {
        QByteArray btmp1 = QByteArray::fromBase64(str_body1.toLatin1());
        decoded1 = QTextCodec::codecForMib(106)->toUnicode(btmp1);

        QByteArray btmp2 = QByteArray::fromBase64(str_body2.toLatin1());
        decoded2 = QTextCodec::codecForMib(106)->toUnicode(btmp2);
    }
    else
    {
        decoded1 = str_body1;
        decoded2 = str_body2;
    }

    this->body.first = decoded1;
    this->body.second = decoded2;  
}

bool emlparser::isNoncompliantMail()
{
    if (this->header.ct == "text/html" || this->header.cte.isEmpty())
        return true;

    return false;
}

QString emlparser::getFrom()
{
    return this->header.from;
}

QString emlparser::getTo()
{
    return this->header.to;
}

QString emlparser::getDate()
{
    return this->header.date;
}

QString emlparser::getSubject()
{
    return this->header.subject;
}

QString emlparser::getContentType()
{
    return this->header.ct;
}

QString emlparser::getUsubscribelist()
{
    return this->header.usubscribelist;
}

QString emlparser::getReturnPath()
{
    return this->header.returnpath;
}
