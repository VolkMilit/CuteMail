#ifndef GENERATE_H
#define GENERATE_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include "settings.h"

class generate
{
public:
    generate();
    ~generate();
    void procmailConfig(QString accaunt_name);
    void fetchmailConfig(QString server, QString protocol, QString username, QString password);
    void fetchmailDelete();
    void procmailDelete();
    void mhaExecutable(QString accaunt_name);
    void deleteExecutable();

private:
    void createFetchFile(QString server, QString protocol, QString username, QString password);
    void createProcmailConfig(QString path);

    QString fetchmailConfigPath;
    QString procmailConfigPath;

    settings *setting;
};

#endif // GENERATE_H
