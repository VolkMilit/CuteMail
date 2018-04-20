#include "newaccdialog.h"
#include "ui_newaccdialog.h"
#include <QDebug>

newaccdialog::newaccdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newaccdialog),
    generate(new cmgenerate)
{
    ui->setupUi(this);

    this->setWindowTitle("Add new account");
    ui->l_warn->hide();
}

newaccdialog::~newaccdialog()
{
    delete generate;
    delete ui;
}

void newaccdialog::accept()
{
    QString server = ui->le_server->text();

    QString acc = ui->le_login->text() + "@" + ui->le_server->text();
    QDir test(generate->getMailFolderPath() + acc);

    bool somethingwrong = false;
    standartStyle(ui->le_server);
    standartStyle(ui->le_login);
    standartStyle(ui->le_password);
    ui->l_warn->hide();

    if (test.exists())
    {
        ui->l_warn->show();
        somethingwrong = true;
    }

    if (server.contains("."))
    {
        QString t = server.split(".").at(1);
        if (t.isEmpty())
        {
            wrongStyle(ui->le_server);
            somethingwrong = true;
        }
    }
    else
    {
        wrongStyle(ui->le_server);
        somethingwrong = true;
    }

    if (ui->le_login->text().isEmpty())
    {
        wrongStyle(ui->le_login);
        somethingwrong = true;
    }

    /*if (ui->le_password->text().isEmpty())
    {
        wrongStyle(ui->le_password);
        somethingwrong = true;
    }*/

    if (somethingwrong)
        return;

    generate->accauntsFolders(ui->le_login->text() + "@" + ui->le_server->text());
    this->close();
}

void newaccdialog::wrongStyle(QWidget *widget)
{
    widget->style()->unpolish(widget);
    widget->setStyleSheet("QLineEdit{background-color: yellow;}");
}

void newaccdialog::standartStyle(QWidget *widget)
{
    widget->style()->unpolish(widget);
    widget->setStyleSheet("");
}
