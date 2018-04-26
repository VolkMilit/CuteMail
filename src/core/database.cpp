/*
    CuteMail - simple mail client for GNU/Linux.
    Copyright (C) 2018 Volk_Milit (aka Ja'Virr-Dar)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "database.h"
#include <QDebug>

database::database(QString file, QString name)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(file);
    db.open();

    QSqlQuery query("CREATE TABLE '" + name + "' (id INTEGER PRIMARY KEY, body TEXT, bodyfull TEXT, status TEXT)");
    db.commit();

    m_file = file; // segfault without this, WTF?
    m_name = name;
}

database::~database()
{
    db.close();
    db.removeDatabase("QSQLITE");
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
    if (!query.exec("INSERT INTO '" + m_name + "'('" + field + "') VALUES('" + value + "')"))
        qDebug() << "Add value db error: " << query.lastError();
}

void database::overrideValue(const int uid, const QString &value, const QString &field)
{
    QSqlQuery query;
    if (!query.exec("UPDATE '" + m_name + "' SET '" + field + "' = '" + value + "' WHERE id = " + QString::number(uid)))
        qDebug() << "Override db error: " << query.lastError();
}
