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

    void write(QString group, QString value, QVariant var);
    QString read(QString group, QString value);
    QString getSettingsPath();

    // [general]
    // TableHeadersWight
    void setTableHeadersWight(QString value);
    QString getTableHeadersWight();

    // SplitterSizes
    void setSplitterSizes(QByteArray value);
    QString getSplitterSizes();

    // WindowDemention
    void setWindowDemention(QString value);
    QString getWindowDemention();

    // WindowFullscreen
    void setWindowFullscreen(QString value);
    QString getWindowFullscreen();

private:
    QString settingsPath;
    QString accauntsBase;
    QString settingsBase;
};

#endif // SETTINGS_H
