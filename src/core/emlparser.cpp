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
    //parseBody();
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
        if (item.at(0) == '=')
        {
            item.remove("=?UTF-8?" + code + "?", Qt::CaseInsensitive);
            item.remove("?=");

            if (code == "Q" || code == "q")
            {
                std::string s_tmp = item.toStdString();
                decoded += QString::fromStdString(decodeQP(s_tmp));
            }
            else
            {
                std::string s_tmp = item.toStdString();
                decoded += QString::fromStdString(decodeB64(s_tmp));
                //QByteArray tmp = QByteArray::fromBase64(str.toLatin1());
                //decoded = QTextCodec::codecForMib(106)->toUnicode(tmp);
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
    QFile file(this->msgfile);

    if (!file.open(QFile::ReadOnly | QFile::Text) || !file.exists())
        return;

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
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
}

void emlparser::parseBody()
{
    int part = 0;
    QString enconding = this->header.cte;
    QString str_body1;
    QString str_body2;

    QFile file(this->msgfile);

    if (!file.open(QFile::ReadOnly | QFile::Text) || !file.exists())
        return;

    QTextStream in(&file);

    in.seek(lastpos);

    while (!in.atEnd())
    {
        QString line = in.readLine();

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

        if (part < 2)
            str_body1 += line + "\n";
        else
            str_body2 += line + "\n";
    }

    std::string l1 = str_body1.toStdString();
    std::string l2 = str_body2.toStdString();
    std::string tmp1;
    std::string tmp2;

    if (enconding.contains("quoted-printable"))
    {
        tmp1 = decodeQP(l1);
        tmp2 = decodeQP(l2);
    }
    else if (enconding.contains("base64"))
    {
        tmp1 = decodeB64(l1);
        tmp2 = decodeB64(l2);
    }
    else
    {
        tmp1 = l1;
        tmp2 = l2;
    }

    QString decoded1 = QString::fromStdString(tmp1);
    QString decoded2 = QString::fromStdString(tmp2);

    this->body.first = decoded1;
    this->body.second = decoded2;
}

QString emlparser::getBoundary()
{
    mimetic::File in(this->msgfile.toStdString());

    mimetic::MimeEntity me(in.begin(), in.end());
    mimetic::MimeEntity* pMe = &me;

    mimetic::Header& h = pMe->header();

    if (h.contentType().isMultipart()) // \/ yeah, this sucks, thanks to qt
    {
        std::string ret = stdSplit(h.contentType().str(), '=').at(1);
        return QString::fromStdString(ret).remove("\"");
    }

    return "";
}

bool emlparser::isMultipart()
{
    mimetic::File in(this->msgfile.toStdString());

    mimetic::MimeEntity me(in.begin(), in.end());
    mimetic::MimeEntity* pMe = &me;

    mimetic::Header& h = pMe->header();

    if (h.contentType().isMultipart())
        return true;

    return false;
}

QString emlparser::findBody()
{
    QString tmp;
    int idx = 0;

    QString bound = getBoundary();

    QFile file(QDir::homePath() + "/.cache/cutemail-tmp.html");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QTextStream out(&file);

    while (!out.atEnd())
    {
        if (idx >= 2)
            break;

        QString line = out.readLine();

        if ("--" + bound == line || "--" + bound + "--" == line) // we're met start of body
        {
            idx += 1;
            continue;
        }

        if (line.contains("Content-Type"))
            continue;

        if (line.contains("Content-Transfer-Encoding"))
        {
            this->enconding = line.split(":").at(1);
            continue;
        }

        tmp += line + "\n";
    }

    if (file.exists())
        file.remove();

    file.close();

    return tmp;
}

void emlparser::trydecode()
{
    QString tmp = findBody();

    // FUCK EML!!!111 FUCK EVERYBODY, WHO DOESN'T FOLLOW STANDART!!!11
    // For beginig there is been just getHeaderValue, but some mails
    // does't have this field. And sometimes even don't have boundary!
    // I hate this!
    if (enconding.isEmpty())
    {
        isnoncompliant = true;
        enconding = getHeaderValue("Content-Transfer-Encoding");
    }

    if (tmp.isEmpty())
        return;

    QFile file(QDir::homePath() + "/.cache/cutemail-tmp.html");
    QTextStream out(&file);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    if (enconding.contains("base64"))
    {
        std::string tmp1 = tmp.toLatin1().toStdString();
        std::string tmp2 = decodeB64(tmp1);
        out << QString::fromStdString(tmp2);
    }
    else if (enconding.contains("quoted-printable"))
    {
        std::string tmp1 = tmp.toLatin1().toStdString();
        std::string tmp2 = decodeQP(tmp1);
        QString tmp3 = QString::fromStdString(tmp2);
        out << tmp3;
    }
    else
    {
        out << tmp.toLatin1();
    }

    file.close();
}

void emlparser::generateTmpHtml()
{
    int ignoreMask = 0;
    ignoreMask += mimetic::imHeader;
    ignoreMask += mimetic::imChildParts;

    mimetic::File test(this->msgfile.toStdString());
    mimetic::MimeEntity me;
    me.load(test.begin(), test.end(), ignoreMask);

    QFile file(QDir::homePath() + "/.cache/cutemail-tmp.html");

    if (file.exists())
        file.remove();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << me.body().data();

    file.close();

    trydecode();
}

/*
   This function is dedicated to all fuckers, who doesn't follow standarts!
*/
bool emlparser::isNoncompliantMail()
{
    if (this->header.ct == "text/html" || this->header.cte.isEmpty())
        return true;

    return false;
}

QString emlparser::getFrom()
{
    //return getHeaderValue("From");
    return this->header.from;
}

QString emlparser::getTo()
{
    //return getHeaderValue("To");
    return this->header.to;
}

QString emlparser::getDate()
{
    //return getHeaderValue("Date");
    return this->header.date;
}

QString emlparser::getSubject()
{
    //return getHeaderValue("Subject");
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

QString emlparser::getHeaderValue(const std::string &field)
{
    mimetic::File in(this->msgfile.toStdString());

    mimetic::MimeEntity me(in.begin(), in.end());
    mimetic::MimeEntity* pMe = &me;

    mimetic::Header& h = pMe->header();

    std::string src = h.field(field).value();

    if (!src.empty() && src.at(0) == '=')
    {
        try
        {
            std::string headenconding = stdSplit(src, '?').at(2);
            std::string s = stdSplit(src, '?').at(3);

            if (!s.empty())
            {
                std::string got;

                if (headenconding == "q" || headenconding == "Q")
                    got = decodeQP(s);
                else
                    got = decodeB64(s);

                return QString::fromStdString(got);
            }
        }
        catch(...)
        {
            qDebug() << "Failed to encode";
        }
    }

    return QString::fromStdString(src);
}
