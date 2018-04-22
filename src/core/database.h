#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>

class database
{
public:
    database(QString file, QString name);

    QString getValue(const int uid, const QString &field);
    void addValue(const QString &value, const QString &field);
    void overrideValue(const int uid, const QString &value, const QString &field);

private:
    QString m_file;
    QString m_name;
};

#endif // DATABASE_H
