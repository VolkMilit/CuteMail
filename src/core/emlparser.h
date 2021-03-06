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

#ifndef EMLPARSER_H
#define EMLPARSER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <QMessageBox>
#include <QTextCodec>
#include <QPair>

#include "utils/utils.h"
#include "third-party/Qt-Quoted-Printable/quotedprintable.h"

class emlparser : private utils
{
public:
    emlparser(QString path);
    virtual ~emlparser();

    QString getTo();
    QString getFrom();
    QString getReturnPath();
    QString getDate();
    QString getSubject();
    QString getContentType();
    QString getUsubscribelist();
    bool isNoncompliantMail();
    QPair<QString, QString> getBody();
    QStringList getAttachmenstNames();

private:
    QString msgfile;
    QString enconding;
    bool isnoncompliant;

    void parseHeader();
    void parseBody();

    int lastpos;

    QString decodeall(const QString &str);
    QString findNextStr(QTextStream &in, QString str);

    void mixedBound();

    struct header
    {
        QString to;
        QString subject;
        QString from;
        QString returnpath;
        QString cte;
        QString ct;
        QString date;
        QString usubscribelist;
        QString boundary;
        QStringList attachments;
        QStringList attachmentsnames;
    } header;

    struct body
    {
        QString first;
        QString second;
        QString attachments;
    } body;
};

#endif // EMLPARSER_H
