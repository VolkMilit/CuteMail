#include "maildir.h"

mailDir::mailDir(){}
mailDir::~mailDir(){}

QVector<QString> mailDir::scanDir(QString dir)
{
    QVector<QString> tmp;

    QDirIterator it(dir, QStringList() << "*.html", QDir::Files);
    while (it.hasNext())
        tmp.push_back(it.next());

    std::sort(tmp.begin(), tmp.end());

    return tmp;
}
