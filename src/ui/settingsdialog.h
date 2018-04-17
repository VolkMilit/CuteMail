#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDir>

#include "utils/settings.h"

namespace Ui { class settingsDialog; }

class settingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingsDialog(QWidget *parent = 0);
    ~settingsDialog();

private slots:
    void on_cb_alwayswebview_clicked();
    void on_cb_automaticallywebview_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::settingsDialog *ui;
    settings *setting;

    void readSettings();
    void writeSettings();

    bool displayonce;
};

#endif // SETTINGSDIALOG_H
