#include "settings.h"

settings::settings()
{
    settingsPath = QDir::homePath() + "/.config/CuteMail/";
    accauntsBase = "accaunts.ini";
    settingsBase = "settings.ini";
}

settings::~settings()
{

}

// [general]
// TableHeadersWight
void settings::setTableHeadersWight(QString value)
{
    write("General", "TableHeadersWight", value);
}

QString settings::getTableHeadersWight()
{
    return read("General", "TableHeadersWight");
}

// SplitterSizes
void settings::setSplitterSizes(QString value)
{
    write("General", "SplitterSizes", value);
}

QString settings::getSplitterSizes()
{
    return read("General", "SplitterSizes");
}

// WindowDemention
void settings::setWindowDemention(QString value)
{
    write("General", "WindowDemention", value);
}

QString settings::getWindowDemention()
{
    return read("General", "WindowDemention");
}

QString settings::getSettingsPath()
{
    return settingsPath;
}

void settings::write(QString group, QString value, QString var)
{
    QSettings settings(settingsPath + settingsBase, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(value, var);
    settings.endGroup();
}

QString settings::read(QString group, QString value)
{
    QString rv;

    QSettings settings(settingsPath + settingsBase, QSettings::IniFormat);
    settings.beginGroup(group);
    rv = settings.value(value).toString();
    settings.endGroup();

    return rv;
}
