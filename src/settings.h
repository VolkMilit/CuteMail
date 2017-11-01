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

    void write(QString file, QString group, QString value, QVariant var);
    QString read(QString file, QString group, QString value);
    QString getSettingsPath();

    /*accounts.ini*/
    // ProxyString
    void setProxyString(QString value, QString account);
    QString getProxyString(QString account);

    // Port
    void setPort(QString value, QString account);
    QString getPort(QString account);

    // UseSSL
    void setUseSSL(QString value, QString account);
    QString getUseSSL(QString account);

    /*settings.ini*/
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

    // LastAccount
    void setLastAccount(QString value);
    QString getLastAccount();

    bool settingsFile();

private:
    QString settingsPath;
    QString accauntsBase;
    QString settingsBase;
};

#endif // SETTINGS_H
