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

class emlparser : protected utils
{
public:
    emlparser(QString path);
    virtual ~emlparser();

    QString getTo();
    QString getFrom();
    QString getDate();
    QString getSubject();
    void generateTmpHtml();

private:
    QString msg;

    bool isMultipart();
    QString getBoundary();
    void splitMultipartMsg();
    QString getHeaderValue(const std::string &field);
};

#endif // EMLPARSER_H
