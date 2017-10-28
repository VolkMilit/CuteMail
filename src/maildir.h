#ifndef MAILDIR_H
#define MAILDIR_H

#include <QString>
#include <QDir>
#include <QVector>
#include <QDirIterator>

//std headers
#include <algorithm>

class mailDir
{
public:
    mailDir();
    ~mailDir();

    QVector<QString> scanDir(QString dir);
};

#endif // MAILDIR_H
