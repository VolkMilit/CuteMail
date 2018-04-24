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

settings::settings()
{
    settingsPath = QDir::homePath() + "/.config/CuteMail/";
    accauntsBase = settingsPath + "accaunts.ini";
    settingsBase = settingsPath + "settings.ini";
}

settings::~settings()
{

}

bool settings::settingsFile()
{
    QFile setting(settingsBase);
    if (!setting.exists())
        return false;
    else
        return true;
}

/*accounts.ini*/
// ProxyString
void settings::setProxyString(QString value, QString account)
{
    write(accauntsBase, account, "ProxyString", value);
}

QString settings::getProxyString(QString account)
{
    return read(accauntsBase, account, "ProxyString");
}

// Port
void settings::setPort(QString value, QString account)
{
    write(accauntsBase, account, "Port", value);
}

QString settings::getPort(QString account)
{
    return read(accauntsBase, account, "Port");
}

// UseSSL
void settings::setUseSSL(QString value, QString account)
{
    write(accauntsBase, account, "UseSSL", value);
}

QString settings::getUseSSL(QString account)
{
    return read(accauntsBase, account, "UseSSL");
}

/*settings.ini*/
// [general] for user related settings
// LastAccount
void settings::setLastAccount(QString value)
{
    write(settingsBase, "General", "LastAccount", value);
}

QString settings::getLastAccount()
{
    return read(settingsBase, "General", "LastAccount");
}

// MailDir
void settings::setMailDir(QString value)
{
    write(settingsBase, "General", "MailDir", value);
}

QString settings::getMailDir()
{
    return read(settingsBase, "General", "MailDir");
}

// AlwaysUseWebview
void settings::setAlwaysUseWebview(int value)
{
    write(settingsBase, "General", "AlwaysUseWebview", value);
}

int settings::getAlwaysUseWebview()
{
    return readInt(settingsBase, "General", "AlwaysUseWebview");
}

// UseWebviewAutomatically
void settings::setUseWebviewAutomatically(int value)
{
    write(settingsBase, "General", "UseWebviewAutomatically", value);
}

int settings::getUseWebviewAutomatically()
{
    return readInt(settingsBase, "General", "UseWebviewAutomatically");
}

// CheckForNewMail
void settings::setCheckForNewMail(int value)
{
    write(settingsBase, "General", "CheckForNewMail", value);
}

int settings::getCheckForNewMail()
{
    return readInt(settingsBase, "General", "CheckForNewMail");
}

// CheckAfter
void settings::setCheckAfter(int value)
{
    write(settingsBase, "General", "CheckAfter", value);
}

int settings::getCheckAfter()
{
    return readInt(settingsBase, "General", "CheckAfter");
}

// DisplayTray
void settings::setDisplayTray(int value)
{
    write(settingsBase, "General", "DisplayTray", value);
}

int settings::getDisplayTray()
{
    return readInt(settingsBase, "General", "DisplayTray");
}

// DisplayNotify
void settings::setDisplayNotify(int value)
{
    write(settingsBase, "General", "DisplayNotify", value);
}

int settings::getDisplayNotify()
{
    return readInt(settingsBase, "General", "DisplayNotify");
}

// UseXDGBrowser
void settings::setUseXDGBrowser(int value)
{
    write(settingsBase, "General", "UseXDGBrowser", value);
}

int settings::getUseXDGBrowser()
{
    return readInt(settingsBase, "General", "UseXDGBrowser");
}

// [Shortcuts] for user shortcuts
// Search
void settings::setShortcutSearch(QString value)
{
    write(settingsBase, "Shortcuts", "Search", value);
}

QString settings::getShortcutSearch()
{
    return read(settingsBase, "Shortcuts", "Search");
}

// [readonly] for program-generated values
// DisplayMessageOnce
void settings::setDisplayMessageOnce(int value)
{
    write(settingsBase, "readonly", "DisplayMessageOnce", value);
}

int settings::getDisplayMessageOnce()
{
    return readInt(settingsBase, "readonly", "DisplayMessageOnce");
}

// TableHeadersWight
void settings::setTableHeadersWight(QString value)
{
    write(settingsBase, "readonly", "TableHeadersWight", value);
}

QString settings::getTableHeadersWight()
{
    return read(settingsBase, "readonly", "TableHeadersWight");
}

// SplitterSizes
void settings::setSplitterSizes(QString value)
{
    write(settingsBase, "readonly", "SplitterSizes", value);
}

QString settings::getSplitterSizes()
{
    return read(settingsBase, "readonly", "SplitterSizes");
}

// WindowDemention
void settings::setWindowDemention(QString value)
{
    write(settingsBase, "readonly", "WindowDemention", value);
}

QString settings::getWindowDemention()
{
    return read(settingsBase, "readonly", "WindowDemention");
}

// WindowFullscreen
void settings::setWindowFullscreen(QString value)
{
    write(settingsBase, "readonly", "WindowFullscreen", value);
}

QString settings::getWindowFullscreen()
{
    return read(settingsBase, "readonly", "WindowFullscreen");
}

QString settings::getSettingsPath()
{
    return settingsPath;
}

void settings::write(QString file, QString group, QString value, QVariant var)
{
    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(value, var);
    settings.endGroup();
}

QString settings::read(QString file, QString group, QString value)
{
    QString rv;

    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup(group);
    rv = settings.value(value).toString();
    settings.endGroup();

    return rv;
}

int settings::readInt(QString file, QString group, QString value)
{
    int rv;

    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup(group);
    rv = settings.value(value).toInt();
    settings.endGroup();

    return rv;
}
