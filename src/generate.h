#ifndef GENERATE_H
#define GENERATE_H

#include <QString>
#include <QFile>
#include <QDir>

#include "settings.h"

class cmgenerate
{
public:
    cmgenerate();
    ~cmgenerate();
    void accauntsFolders(QString account_name);
    QString getMailFolderPath();

private:
    QString mailFolderPath;

    settings *setting;
};

#endif // GENERATE_H
