#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("About CuteMail");
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::on_buttonBox_accepted()
{
    this->close();
}
