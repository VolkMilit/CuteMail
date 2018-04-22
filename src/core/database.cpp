#include "database.h"
#include <QDebug>

database::database(QString file, QString name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(file);
    db.open();

    QSqlQuery query("CREATE TABLE '" + name + "' (id INTEGER PRIMARY KEY, status TEXT)");

    db.commit();
    //db.close();

    m_file = file; // segfault without this, WTF?
    m_name = name;
}

QString database::getValue(const int uid, const QString &field)
{
    QSqlQuery query;
    query.prepare("SELECT " + field + " FROM '" + m_name + "' WHERE id = ?");
    query.addBindValue(uid);

    if (!query.exec())
        qDebug() << "Get value db error: " << query.lastError();

    if (query.first())
        return query.value(0).toString();
    else
        return "";
}

void database::addValue(const QString &value, const QString &field)
{
    QSqlQuery query;
    if (!query.exec("INSERT INTO '" + m_name + "'(" + field + ") VALUES('" + value + "')"))
        qDebug() << "Add value db error: " << query.lastError();
}

void database::overrideValue(const int uid, const QString &value, const QString &field)
{
    QSqlQuery query;
    if (!query.exec("UPDATE '" + m_name + "' SET '" + field + "' = '" + value + "' WHERE id = " + QString::number(uid)))
        qDebug() << "Override db error: " << query.lastError();
}
