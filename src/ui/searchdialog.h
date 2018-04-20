#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class searchDialog;
}

class searchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchDialog(QWidget *parent = 0);
    ~searchDialog();

    enum searchby
    {
        ALL = 0,
        SUBJECT,
        SENDER
    } searchby;

    QString getString();
    int getSearchby();
    int getCase();
    int getFirstCoins();

private:
    Ui::searchDialog *ui;
};

#endif // SEARCHDIALOG_H
