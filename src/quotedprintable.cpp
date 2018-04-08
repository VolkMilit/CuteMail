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

#include "quotedprintable.h"

QByteArray QuotedPrintable::encode(const QString &input)
{
    QByteArray output;

    char byte;
    const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    QByteArray latin1 = input.toUtf8();
    for (int i = 0; i < latin1.length() ; ++i)
    {
        byte = latin1[i];

        if ((byte == ' ') || ((byte >= 33) && (byte <= 126)  && (byte != '=')))
        {
            output.append(byte);
        }
        else
        {
            output.append('=');
            output.append(hex[((byte >> 4) & 0x0F)]);
            output.append(hex[(byte & 0x0F)]);
        }
    }

    return output;
}


QString QuotedPrintable::decode(const QByteArray &input)
{
    QByteArray output;

    for (int i = 0; i < input.length(); ++i)
    {
        if (input.at(i) == '=' && i+2<input.length())
        {
            QString strValue = input.mid((++i)++, 2);
            bool converted;
            char character = strValue.toUInt(&converted, 16);
            if( converted )
                output.append(character);
            else
                output.append( "=" + strValue);
        }
        else
        {
            output.append(input.at(i));
        }
    }

    return QString::fromUtf8(output);
}
