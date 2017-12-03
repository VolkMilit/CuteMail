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
