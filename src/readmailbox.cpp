#include "readmailbox.h"
#include <QDebug>

readMailbox::readMailbox()
{
}

readMailbox::~readMailbox()
{

}

// read full html file and return vectors of string
QVector<QString> readMailbox::readFull(QString doucument)
{
    QVector<QString> documentvector;

    if (documentvector.size() > 0) // it's empty actually, just in case
        documentvector.clear();

    QFile file(doucument);

    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    while (!in.atEnd())
    {
        documentvector.push_back(in.readLine());
    }

    file.close();

    return documentvector;
}

// read specific tag from html file and return string without tags
QString readMailbox::readParam(QString file, QString param)
{
    QVector<QString> tmp = readFull(file);

    QString ret;

    for (int i = 0; i < tmp.size(); i++)
    {
        if (tmp.at(i).contains(param))
        {
            ret = tmp.at(i);
            break;
        }
    }

    return removeJunk(ret.remove(param)).remove(":");
}

// delete all html tags from string
QString readMailbox::removeJunk(QString string)
{
    QXmlStreamReader xml(string);
    QString textString;
    while (!xml.atEnd())
    {
        if ( xml.readNext() == QXmlStreamReader::Characters )
        {
            textString += xml.text();
        }
    }

    return textString;
}

QString readMailbox::readTo(QString file)
{
    return readParam(file, "Delivered-To");
}

QString readMailbox::readSubject(QString file)
{
    return readParam(file, "Subject</em>");
}

QString readMailbox::readFrom(QString file)
{
    return readParam(file, "From");
}

QString readMailbox::readDate(QString file)
{
    return readParam(file, "Date");
}
