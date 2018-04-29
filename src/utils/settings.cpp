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

#include "settings.h"

Settings::Settings()
{
    settingsPath = QDir::homePath() + "/.config/CuteMail/";
    accauntsBase = settingsPath + "accaunts.ini";
    settingsBase = settingsPath + "settings.ini";
}

Settings::~Settings()
{

}

bool Settings::settingsFile()
{
    QFile setting(settingsBase);
    if (!setting.exists())
        return false;
    else
        return true;
}

/*accounts.ini*/
// ProxyString
void Settings::setProxyString(QString value, QString account)
{
    write(accauntsBase, account, "ProxyString", value);
}

QString Settings::getProxyString(QString account)
{
    return read(accauntsBase, account, "ProxyString");
}

// Port
void Settings::setPort(QString value, QString account)
{
    write(accauntsBase, account, "Port", value);
}

QString Settings::getPort(QString account)
{
    return read(accauntsBase, account, "Port");
}

// UseSSL
void Settings::setUseSSL(QString value, QString account)
{
    write(accauntsBase, account, "UseSSL", value);
}

QString Settings::getUseSSL(QString account)
{
    return read(accauntsBase, account, "UseSSL");
}

/*settings.ini*/
// [general] for user related settings
// LastAccount
void Settings::setLastAccount(QString value)
{
    write(settingsBase, "General", "LastAccount", value);
}

QString Settings::getLastAccount()
{
    return read(settingsBase, "General", "LastAccount");
}

// MailDir
void Settings::setMailDir(QString value)
{
    write(settingsBase, "General", "MailDir", value);
}

QString Settings::getMailDir()
{
    return read(settingsBase, "General", "MailDir");
}

// AlwaysUseWebview
void Settings::setAlwaysUseWebview(int value)
{
    write(settingsBase, "General", "AlwaysUseWebview", value);
}

int Settings::getAlwaysUseWebview()
{
    return readInt(settingsBase, "General", "AlwaysUseWebview");
}

// UseWebviewAutomatically
void Settings::setUseWebviewAutomatically(int value)
{
    write(settingsBase, "General", "UseWebviewAutomatically", value);
}

int Settings::getUseWebviewAutomatically()
{
    return readInt(settingsBase, "General", "UseWebviewAutomatically");
}

// CheckForNewMail
void Settings::setCheckForNewMail(int value)
{
    write(settingsBase, "General", "CheckForNewMail", value);
}

int Settings::getCheckForNewMail()
{
    return readInt(settingsBase, "General", "CheckForNewMail");
}

// CheckAfter
void Settings::setCheckAfter(int value)
{
    write(settingsBase, "General", "CheckAfter", value);
}

int Settings::getCheckAfter()
{
    return readInt(settingsBase, "General", "CheckAfter");
}

// DisplayTray
void Settings::setDisplayTray(int value)
{
    write(settingsBase, "General", "DisplayTray", value);
}

int Settings::getDisplayTray()
{
    return readInt(settingsBase, "General", "DisplayTray");
}

// DisplayNotify
void Settings::setDisplayNotify(int value)
{
    write(settingsBase, "General", "DisplayNotify", value);
}

int Settings::getDisplayNotify()
{
    return readInt(settingsBase, "General", "DisplayNotify");
}

// UseXDGBrowser
void Settings::setUseXDGBrowser(int value)
{
    write(settingsBase, "General", "UseXDGBrowser", value);
}

int Settings::getUseXDGBrowser()
{
    return readInt(settingsBase, "General", "UseXDGBrowser");
}

// [readonly] for program-generated values
// DisplayMessageOnce
void Settings::setDisplayMessageOnce(int value)
{
    write(settingsBase, "readonly", "DisplayMessageOnce", value);
}

int Settings::getDisplayMessageOnce()
{
    return readInt(settingsBase, "readonly", "DisplayMessageOnce");
}

// TableHeadersWight
void Settings::setTableHeadersWight(QString value)
{
    write(settingsBase, "readonly", "TableHeadersWight", value);
}

QVector<int> Settings::getTableHeadersWight()
{
    QStringList tmp = read(settingsBase, "readonly", "TableHeadersWight").split(",");
    tmp.removeAt(tmp.length()-1);
    QVector<int> ret;

    for (QVariant i : tmp)
        ret.push_back(i.toInt());

    return ret;
}

// SplitterSizes
void Settings::setSplitterSizes(QString value)
{
    write(settingsBase, "readonly", "SplitterSizes", value);
}

QString Settings::getSplitterSizes()
{
    return read(settingsBase, "readonly", "SplitterSizes");
}

// WindowDemention
void Settings::setWindowDemention(QString value)
{
    write(settingsBase, "readonly", "WindowDemention", value);
}

QVector<int> Settings::getWindowDemention()
{
    QStringList tmp = read(settingsBase, "readonly", "WindowDemention").split("x");
    QVector<int> ret = {QVariant(tmp.at(0)).toInt(), QVariant(tmp.at(1)).toInt()};

    return ret;
}

// WindowFullscreen
void Settings::setWindowFullscreen(int value)
{
    write(settingsBase, "readonly", "WindowFullscreen", value);
}

int Settings::getWindowFullscreen()
{
    return readInt(settingsBase, "readonly", "WindowFullscreen");
}

QString Settings::getSettingsPath()
{
    return settingsPath;
}

QString Settings::getSettingsFilePath()
{
  return settingsBase;
}

void Settings::write(QString file, QString group, QString field, QVariant var)
{
    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(field, var);
    settings.endGroup();
}

QString Settings::read(QString file, QString group, QString value)
{
    QString rv;

    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup(group);
    rv = settings.value(value).toString();
    settings.endGroup();

    return rv;
}

int Settings::readInt(QString file, QString group, QString value)
{
    int rv;

    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup(group);
    rv = settings.value(value).toInt();
    settings.endGroup();

    return rv;
}

QStringList Settings::readGroup(QString file, QString group)
{
    QStringList rv;

    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup(group);
    rv = settings.allKeys();
    settings.endGroup();

    return rv;
}
