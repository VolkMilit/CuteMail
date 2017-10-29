#include "maildir.h"
#include <QDebug>

mailDir::mailDir()
    : gen(new generate())
{

}

mailDir::~mailDir()
{
    delete gen;
}

QVector<QString> mailDir::scanDir(QString dir)
{
    QVector<QString> tmp;

    QDirIterator it(dir, QStringList() << "*.html", QDir::Files);
    while (it.hasNext())
        tmp.push_back(it.next());

    std::sort(tmp.begin(), tmp.end());

    return tmp;
}

void mailDir::move(QString file_name, QString account_name, QString destenation_folder)
{
    const QString dir = gen->getMailFolderPath() + account_name + "/";
    QFile file(file_name);
    QFileInfo file_info(file_name);
    file.copy(file_name, dir + destenation_folder + "/" + file_info.baseName() + ".html");
    file.remove();
}
