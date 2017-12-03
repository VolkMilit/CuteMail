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

class emlparser
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

    std::vector<std::string> split(const std::string &s, char delim);
    bool isMultipart();
    QString getBoundary();
    void splitMultipartMsg();
};

#endif // EMLPARSER_H
