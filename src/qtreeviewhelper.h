#ifndef QTREEVIEWHELPER_H
#define QTREEVIEWHELPER_H

#include <QTreeView>
#include <QAbstractItemView>
#include <QAbstractItemModel>

class QTreeViewHelper
{
public:
    QTreeViewHelper();
    ~QTreeViewHelper();

    int topLevelItemCount(QTreeView *tree);
    int belowLevelItemCount();
    QString currentItem();
    QString currentParentFromChild();
    void addItem(QString item);
    void addChildItem(/*parent*/);
};

#endif // QTREEVIEWHELPER_H
