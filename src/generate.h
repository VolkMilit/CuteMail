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
    void procmailConfig(QString account_name);
    void fetchmailConfig(QString server, QString protocol, QString username, QString password);
    void fetchmailDelete();
    void procmailDelete();
    void mhaExecutable(QString account_name);
    void deleteExecutable();
    void accauntsFolders(QString account_name);
    QString getMailFolderPath();

private:
    void createFetchFile(QString server, QString protocol, QString username, QString password);
    void createProcmailConfig(QString path);

    QString fetchmailConfigPath;
    QString procmailConfigPath;
    QString mailFolderPath;

    settings *setting;
};

#endif // GENERATE_H
