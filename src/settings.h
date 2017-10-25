#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>
#include <QDir>

class settings
{
public:
    settings();
    ~settings();

    void write(QString group, QString value, QString var);
    QString read(QString group, QString value);
    QString getSettingsPath();

    // [general]
    // TableHeadersWight
    void setTableHeadersWight(QString value);
    QString getTableHeadersWight();

    // SplitterSizes
    void setSplitterSizes(QString value);
    QString getSplitterSizes();

    // WindowDemention
    void setWindowDemention(QString value);
    QString getWindowDemention();

private:
    QString settingsPath;
    QString accauntsBase;
    QString settingsBase;
};

#endif // SETTINGS_H
