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

    system("mkdir ~/.config/beatrice");
    system("touch ~/.config/beatrice/config.ini");

    QString config =
    "[UI]\n"
    "window/minWidth=1000;\n"
    "window/minHeight=600;\n"
    "clipboard/maxSize=10;\n"
    "infoPanel/separator=\" | \";\n"
    "textbox/fontFamily=\"Source Code Pro\";\n"
    "textbox/fontSize=17;\n"
    "theme=\"Micro\";\n";

    if (!exists) {
        QFile file(homedir + "/.config/beatrice/config.ini");
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);
            out << config;
            file.close();
        }
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
