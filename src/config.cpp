#include "config.h"
#include "utils.h"

#include <QSettings>

Config::Config()
{
    addConfigFile();
    loadConfigFile();
}

void Config::addConfigFile() {
    QString homedir = getHomeDir().c_str();
    QFile fl(homedir + "/.config/beatrice/config.ini");
    bool exists = fl.exists();

    if (!exists) {
        QSettings* settings = new QSettings(homedir + "/.config/beatrice/config.ini", QSettings::IniFormat);
        settings->setValue("UI/window/minWidth", 1000);
        settings->setValue("UI/window/minHeight", 600);
        settings->setValue("UI/clipboard/maxSize", 10);
        settings->setValue("UI/infoPanel/separator", " | ");
        settings->setValue("UI/textbox/fontFamily", "Source Code Pro");
        settings->setValue("UI/textbox/fontSize", 17);
        settings->setValue("UI/theme", "Micro");
        settings->sync();
    }
}

void Config::loadConfigFile() {
    QString homedir = getHomeDir().c_str();
    QSettings settings(QString(homedir + "/.config/beatrice/config.ini"), QSettings::IniFormat);

    windowMinWidth      = settings.value("UI/window/minWidth", 1000).toInt();
    windowMinHeight     = settings.value("UI/window/minHeight", 600).toInt();
    clipboardMaxSize    = settings.value("UI/clipboard/maxSize", 10).toInt();
    infoPanelSeparator  = settings.value("UI/infoPanel/separator", " | ").toString().toStdString();
    textboxFontFamily   = settings.value("UI/textbox/fontFamily", "Source Code Pro").toString().toStdString();
    textboxFontSize     = settings.value("UI/textbox/fontSize", 17).toInt();
    theme               = settings.value("UI/theme", "Micro").toString().toStdString();
}
