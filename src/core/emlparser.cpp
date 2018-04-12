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
}

emlparser::~emlparser()
{
    // todo: store in memory
    QFile file(QDir::homePath() + "/.cache/cutemail-tmp.html");
    file.remove();
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

void emlparser::decode()
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

        // this is temporary duck-tape for QP
        // becouse mimetic's decodeQP is broken
        // for cyrilic
        tmp3.replace("==D0", "о");
        tmp3.replace("==D1", "у");
        tmp3.replace("==B0", "а");
        tmp3.replace("==B5", "е");
        tmp3.replace("==81", "с");
        tmp3.replace("==8F", "я");
        tmp3.replace("�", "");

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

    decode();
}

/*
   This function is dedicated to all fuckers, who doesn't follow standarts!
*/
bool emlparser::isNoncompliantMail()
{
    return isnoncompliant;
}

QString emlparser::getFrom()
{
    // there is a "bug", only first part parsing
    return getHeaderValue("From");
}

QString emlparser::getTo()
{
    return getHeaderValue("To");
}

QString emlparser::getDate()
{
    return getHeaderValue("Date");
}

QString emlparser::getSubject()
{
    return getHeaderValue("Subject");
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
