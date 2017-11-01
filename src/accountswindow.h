#ifndef ACCOUNTSWINDOW_H
#define ACCOUNTSWINDOW_H

#include <QDialog>

#include "settings.h"
#include "generate.h"

namespace Ui {
class accountsWindow;
}

class accountsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit accountsWindow(QWidget *parent = 0);
    ~accountsWindow();

private slots:
    void on_btn_new_clicked();
    void on_lw_accounts_clicked(const QModelIndex &index);
    void on_buttonBox_rejected();

private:
    void populateAccountsList();
    void writeAccount();
    void readAccount();

    Ui::accountsWindow *ui;

    settings *setting;
    generate *gen;

    QString prevAccount;
};

#endif // ACCOUNTSWINDOW_H
