/*
    Copyright (c) 2011 - Tőkés Attila (tokes_atti@yahoo.com)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    See the LICENSE file for more details.
*/

#ifndef QUOTEDPRINTABLE_H
#define QUOTEDPRINTABLE_H

#include <QObject>
#include <QByteArray>

class QuotedPrintable : public QObject
{
    Q_OBJECT
public:

    static QByteArray encode(const QString &input);
    static QString decode(const QByteArray &input);

private:
    QuotedPrintable();
};

#endif // QUOTEDPRINTABLE_H
