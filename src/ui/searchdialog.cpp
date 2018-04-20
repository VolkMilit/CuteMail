#include "searchdialog.h"
#include "ui_searchdialog.h"

searchDialog::searchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Search...");
}

searchDialog::~searchDialog()
{
    delete ui;
}

int searchDialog::getCase()
{
    return ui->cb_case->isChecked() ? 1 : 0;
}

int searchDialog::getSearchby()
{
    return ui->comb_searchfor->currentIndex();
}

QString searchDialog::getString()
{
    return ui->le_searchfor->text();
}

int searchDialog::getFirstCoins()
{
    return ui->cb_firstcoinc->isChecked() ? 1 : 0;
}
