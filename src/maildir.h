#ifndef MAILDIR_H
#define MAILDIR_H

#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QVector>
#include <QDirIterator>

//std headers
#include <algorithm>

#include "generate.h"

class mailDir
{
public:
    mailDir();
    ~mailDir();

    QVector<QString> scanDir(QString dir);
    void move(QString file_name, QString account_name, QString destenation_folder);

private:
    generate *gen;
};

#endif // MAILDIR_H
