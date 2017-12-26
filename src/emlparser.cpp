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
        std::string ret = split(h.contentType().str(), '=').at(1);
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

    QFile file(QDir::homePath() + "/.cache/cutemail-tmp.html");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    while (!out.atEnd())
    {
        QString line = out.readLine();

        if ("--" + getBoundary() == line && idx < 2)
            idx += 1;

        if (idx >= 2)
            tmp += line;
    }

    if (file.exists())
        file.remove();

    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    int idxOf = tmp.indexOf("<");
    tmp.remove(0, idxOf);

    out << tmp;

    file.close();
}

void emlparser::generateTmpHtml()
{
    //std::string ttt;

    int ignoreMask = 0;
    ignoreMask += mimetic::imHeader;
    ignoreMask += mimetic::imChildParts;

    mimetic::File test(this->msg.toStdString());
    mimetic::MimeEntity me;
    me.load(test.begin(), test.end(), ignoreMask);

    //mimetic::Base64::Decoder b64;
    //mimetic::code(test.begin(), test.end(), b64, ttt);

    QFile file(QDir::homePath() + "/.cache/cutemail-tmp.html");

    if (file.exists())
        file.remove();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << QString::fromUtf8(me.body().data()); // need to get document encoding, but...
                                                // well, nobody using anything exept uft8 nowadays

    file.close();

    if (isMultipart())
    {
        splitMultipartMsg();
    }

    //std::cout << ttt << std::endl;
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

std::vector<std::string> emlparser::split(const std::string &s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;

    while (std::getline(ss, item, delim))
        elems.push_back(item);

    return elems;
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
            std::string s = split(src, '?').at(3);

            if (!s.empty())
            {
                std::string got;
                mimetic::Base64::Decoder base64;
                mimetic::code(s.begin(), s.end(), base64, std::back_inserter<std::string>(got));

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
