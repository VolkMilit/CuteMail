#include "emlparser.h"
#include <QDebug>

emlparser::emlparser(QString path)
{
    this->msg = path;
}

emlparser::~emlparser()
{
    QFile file(QDir::homePath() + "/.cache/cutemail-tmp.html");
    file.remove();
}

QString emlparser::getBoundary()
{
    mimetic::File in(this->msg.toStdString());

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
    mimetic::File in(this->msg.toStdString());

    mimetic::MimeEntity me(in.begin(), in.end());
    mimetic::MimeEntity* pMe = &me;

    mimetic::Header& h = pMe->header();

    if (h.contentType().isMultipart())
        return true;

    return false;
}

void emlparser::splitMultipartMsg()
{
    QString tmp;
    int idx = 0;

    QString bound = getBoundary();

    QFile file(QDir::homePath() + "/.cache/cutemail-tmp.html");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

    QTextStream out(&file);

    while (!out.atEnd())
    {
        if (idx >= 2)
            break;

        QString line = out.readLine();

        if ("--" + bound == line || "--" + bound + "--" == line)
        {
            idx += 1;
            continue;
        }

        tmp += line + "\n";

        /*if ("--" + bound == line && idx < 2)
            idx += 1;

        if (idx >= 2)
            tmp += line;*/
    }

    if (file.exists())
        file.remove();

    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    //int idxOf = tmp.indexOf("<");
    //tmp.remove(0, idxOf);

    if (tmp.contains("base64"))
    {
        std::string tmp1 = tmp.toLatin1().toStdString();
        std::string tmp2 = decodeB64(tmp1);
        out << QString::fromStdString(tmp2);
    }
    else if (tmp.contains("quoted-printable"))
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

    mimetic::File test(this->msg.toStdString());
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

    //if (isMultipart())
    //{
        splitMultipartMsg();
    //}
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
    mimetic::File in(this->msg.toStdString());

    mimetic::MimeEntity me(in.begin(), in.end());
    mimetic::MimeEntity* pMe = &me;

    mimetic::Header& h = pMe->header();

    std::string src = h.field(field).value();

    if (src.at(0) == '=')
    {
        try
        {
            std::string enconding = stdSplit(src, '?').at(2);
            std::string s = stdSplit(src, '?').at(3);

            if (!s.empty())
            {
                std::string got;

                if (enconding == "q" || enconding == "Q")
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
