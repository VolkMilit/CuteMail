#ifndef EMLPARSER_H
#define EMLPARSER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <QMessageBox>
#include <QTextCodec>

#include <mimetic/mimetic.h>
#include <iostream>
#include <vector>

#include "utils.h"

class emlparser : private utils
{
public:
    emlparser(QString path);
    virtual ~emlparser();

    QString getTo();
    QString getFrom();
    QString getDate();
    QString getSubject();
    void generateTmpHtml();
    bool isNoncompliantMail();

private:
    QString msgfile;
    QString enconding;
    bool isnoncompliant;

    bool isMultipart();
    QString getBoundary();
    QString findBody();
    void decode();
    QString getHeaderValue(const std::string &field);
};

#endif // EMLPARSER_H
