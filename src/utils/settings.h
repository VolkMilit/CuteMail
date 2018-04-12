/*
    CuteMail - simple mail client for GNU/Linux.
    Copyright (C) 2018 Volk_Milit (aka Ja'Virr-Dar)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
    int readInt(QString file, QString group, QString value);
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

    // MailDir
    void setMailDir(QString value);
    QString getMailDir();

    // AlwaysUseWebview
    void setAlwaysUseWebview(int value);
    int getAlwaysUseWebview();

    // UseWebviewAutomatically
    void setUseWebviewAutomatically(int value);
    int getUseWebviewAutomatically();

    // CheckForNewMail
    void setCheckForNewMail(int value);
    int getCheckForNewMail();

    // CheckAfter
    void setCheckAfter(int value);
    int getCheckAfter();

    // DisplayTray
    void setDisplayTray(int value);
    int getDisplayTray();

    // DisplayNotify
    void setDisplayNotify(int value);
    int getDisplayNotify();

    // DisplayMessageOnce
    void setDisplayMessageOnce(int value);
    int getDisplayMessageOnce();

    bool settingsFile();

private:
    QString settingsPath;
    QString accauntsBase;
    QString settingsBase;
};

#endif // SETTINGS_H
