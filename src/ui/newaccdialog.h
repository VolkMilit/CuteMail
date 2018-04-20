#ifndef NEWACCDIALOG_H
#define NEWACCDIALOG_H

#include <QDialog>
#include <QDir>
#include <QUrl>

#include "core/generate.h"

namespace Ui {
class newaccdialog;
}

class newaccdialog : public QDialog
{
    Q_OBJECT

public:
    explicit newaccdialog(QWidget *parent = 0);
    ~newaccdialog();

protected slots:
    void accept();

private:
    Ui::newaccdialog *ui;
    cmgenerate *generate;

    void wrongStyle(QWidget *widget);
    void standartStyle(QWidget *widget);
};

#endif // NEWACCDIALOG_H
