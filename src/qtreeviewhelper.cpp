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

#include "qtreeviewhelper.h"
#include <QDebug>

QTreeViewHelper::QTreeViewHelper(){}
QTreeViewHelper::~QTreeViewHelper(){}

int QTreeViewHelper::topLevelItemCount(QTreeView *tree)
{
    int idx = 0;
    const QModelIndex index = tree->currentIndex();

    while(index.sibling(idx, 0).isValid())
    {
        idx += 1;
        qDebug() << index.sibling(idx, 0);
    }

    return idx;
}

QString QTreeViewHelper::currentItem()
{

}

void QTreeViewHelper::addItem(QString item)
{

}
