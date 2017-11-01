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
// [general]
// TableHeadersWight
void settings::setTableHeadersWight(QString value)
{
    write(settingsBase, "General", "TableHeadersWight", value);
}

QString settings::getTableHeadersWight()
{
    return read(settingsBase, "General", "TableHeadersWight");
}

// SplitterSizes
void settings::setSplitterSizes(QByteArray value)
{
    write(settingsBase, "General", "SplitterSizes", value);
}

QString settings::getSplitterSizes()
{
    return read(settingsBase, "General", "SplitterSizes");
}

// WindowDemention
void settings::setWindowDemention(QString value)
{
    write(settingsBase, "General", "WindowDemention", value);
}

QString settings::getWindowDemention()
{
    return read(settingsBase, "General", "WindowDemention");
}

// WindowFullscreen
void settings::setWindowFullscreen(QString value)
{
    write(settingsBase, "General", "WindowFullscreen", value);
}

QString settings::getWindowFullscreen()
{
    return read(settingsBase, "General", "WindowFullscreen");
}

// LastAccount
void settings::setLastAccount(QString value)
{
    write(settingsBase, "General", "LastAccount", value);
}

QString settings::getLastAccount()
{
    return read(settingsBase, "General", "LastAccount");
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
