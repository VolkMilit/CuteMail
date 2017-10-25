#ifndef READMAILBOX_H
#define READMAILBOX_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QXmlStreamReader>

class readMailbox
{
public:
    readMailbox();
    ~readMailbox();

    QString readFrom(QString file);
    QString readTo(QString file);
    QString readSubject(QString file);
    QString readDate(QString file);

private:
    QVector<QString> readFull(QString document);
    QString readParam(QString file, QString param);
    QString removeJunk(QString string);
};

#endif // READMAILBOX_H
