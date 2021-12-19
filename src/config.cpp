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
        settings->setValue("UI/window/minWidth",        windowMinWidth);
        settings->setValue("UI/window/minHeight",       windowMinHeight);
        settings->setValue("UI/clipboard/maxSize",      clipboardMaxSize);
        settings->setValue("UI/infoPanel/separator",    infoPanelSeparator.c_str());
        settings->setValue("UI/textbox/fontFamily",     textboxFontFamily.c_str());
        settings->setValue("UI/textbox/fontSize",       textboxFontSize);
        settings->setValue("UI/infoPanel/fontFamily",   infopanelFontFamily.c_str());
        settings->setValue("UI/infoPanel/fontSize",     infopanelFontSize);
        settings->setValue("UI/commandline/fontFamily", commandlineFontFamily.c_str());
        settings->setValue("UI/commandline/fontSize",   commandlineFontSize);
        settings->setValue("UI/clipboard/fontFamily",   clipboardFontFamily.c_str());
        settings->setValue("UI/clipboard/fontSize",     clipboardFontSize);
        settings->setValue("UI/filelist/fontFamily",    filelistFontFamily.c_str());
        settings->setValue("UI/filelist/fontSize",      filelistFontSize);
        settings->setValue("UI/theme",                  theme.c_str());
        settings->setValue("USER/loadLastSessionFiles", loadLastSessionFiles);
        settings->setValue("USER/terminalCommand",      terminalCommand.c_str());
        settings->sync();
    }
}

void Config::loadConfigFile() {
    QString homedir = getHomeDir().c_str();
    QSettings settings(QString(homedir + "/.config/beatrice/config.ini"), QSettings::IniFormat);

    windowMinWidth          = settings.value("UI/window/minWidth",          windowMinWidth).toInt();
    windowMinHeight         = settings.value("UI/window/minHeight",         windowMinHeight).toInt();
    clipboardMaxSize        = settings.value("UI/clipboard/maxSize",        clipboardMaxSize).toInt();
    infoPanelSeparator      = settings.value("UI/infoPanel/separator",      infoPanelSeparator.c_str()).toString().toStdString();
    textboxFontFamily       = settings.value("UI/textbox/fontFamily",       textboxFontFamily.c_str()).toString().toStdString();
    textboxFontSize         = settings.value("UI/textbox/fontSize",         textboxFontSize).toInt();
    infopanelFontFamily     = settings.value("UI/infoPanel/fontFamily",     infopanelFontFamily.c_str()).toString().toStdString();
    infopanelFontSize       = settings.value("UI/infoPanel/fontSize",       infopanelFontSize).toInt();
    commandlineFontFamily   = settings.value("UI/commandline/fontFamily",   commandlineFontFamily.c_str()).toString().toStdString();
    commandlineFontSize     = settings.value("UI/commandline/fontSize",     commandlineFontSize).toInt();
    clipboardFontFamily     = settings.value("UI/clipboard/fontFamily",     clipboardFontFamily.c_str()).toString().toStdString();
    clipboardFontSize       = settings.value("UI/clipboard/fontSize",       clipboardFontSize).toInt();
    clipboardFontFamily     = settings.value("UI/filelist/fontFamily",      filelistFontFamily.c_str()).toString().toStdString();
    clipboardFontSize       = settings.value("UI/filelist/fontSize",        filelistFontSize).toInt();
    theme                   = settings.value("UI/theme",                    theme.c_str()).toString().toStdString();

    loadLastSessionFiles    = settings.value("USER/loadLastSessionFiles",   loadLastSessionFiles).toBool();
    terminalCommand         = settings.value("USER/terminalCommand",        terminalCommand.c_str()).toString().toStdString();

    setThemeColors();
}

void Config::setThemeColors() {
    if (theme == MICRO_THEME) {
        themeColors = loadTheme_Micro();
    }
    else if (theme == DRACULA_THEME) {
        themeColors = loadTheme_Dracula();
    }
    else if (theme == DIABLO_THEME) {
        themeColors = loadTheme_Diablo();
    }
    else {
        themeColors = {};
    }
}

map<string, string> Config::loadTheme_Micro() {
    map<string, string> theme;

    theme["flistBackground"]        = "#2a2b2e";
    theme["flinfoBackground"]       = "#d3d3d3";
    theme["fltextBackground"]       = "#3a3b3e";
    theme["flwidgetBackground"]     = "#2a2b2e";
    theme["flsearchBackground"]     = "#2a2b2e";

    theme["flistFontColor"]         = "#d3d3d3";
    theme["flinfoFontColor"]        = "#2a2b2e";
    theme["fltextFontColor"]        = "#d3d3d3";
    theme["flwidgetFontColor"]      = "#d3d3d3";
    theme["flsearchFontColor"]      = "#d3d3d3";

    theme["findBackground"]         = "#1f221d";
    theme["textboxBackground"]      = "#1f222d";
    theme["infopanelBackground"]    = "#d3d3d3";
    theme["clipboardBackground"]    = "#2a2b2e";
    theme["mainwindowBackground"]   = "#1f222d";
    theme["commandlineBackground"]  = "#1f222d";

    theme["findFontColor"]          = "#d3d3d3";
    theme["textboxFontColor"]       = "#d3d3d3";
    theme["infopanelFontColor"]     = "#1f222d";
    theme["clipboardFontColor"]     = "#d3d3d3";
    theme["commandlineFontColor"]   = "#d3d3d3";

    theme["linenumCurrentLineColor"]= "#5aff1e";
    theme["linenumBackground"]      = "#1f222d";
    theme["linenumFontColor"]       = "#808080";

    return theme;
}

map<string, string> Config::loadTheme_Dracula() {
    map<string, string> theme;

    theme["flistBackground"]        = "#383a46"; //382a36
    theme["flinfoBackground"]       = "#bd93f9";
    theme["fltextBackground"]       = "#483a50";
    theme["flwidgetBackground"]     = "#383a46"; //382a36
    theme["flsearchBackground"]     = "#44475a"; //382a36

    theme["flistFontColor"]         = "#d3d3d3";
    theme["flinfoFontColor"]        = "#382a36";
    theme["fltextFontColor"]        = "#d3d3d3";
    theme["flwidgetFontColor"]      = "#d3d3d3";
    theme["flsearchFontColor"]      = "#d3d3d3";

    theme["findBackground"]         = "#282a26";
    theme["textboxBackground"]      = "#282a36";
    theme["infopanelBackground"]    = "#bd93f9";
    theme["clipboardBackground"]    = "#383a46"; //382a36
    theme["mainwindowBackground"]   = "#282a36";
    theme["commandlineBackground"]  = "#44475a"; //282a36

    theme["findFontColor"]          = "#d3d3d3";
    theme["textboxFontColor"]       = "#d3d3d3";
    theme["infopanelFontColor"]     = "#282a36";
    theme["clipboardFontColor"]     = "#d3d3d3";
    theme["commandlineFontColor"]   = "#d3d3d3";

    theme["linenumCurrentLineColor"]= "#f1fa8c";
    theme["linenumBackground"]      = "#282a36";
    theme["linenumFontColor"]       = "#808080";

    return theme;
}

map<string, string> Config::loadTheme_Diablo() {
    map<string, string> theme;

    theme["flistBackground"]        = "#303030";
    theme["flinfoBackground"]       = "#ff0024";
    theme["fltextBackground"]       = "#303030";
    theme["flwidgetBackground"]     = "#303030";
    theme["flsearchBackground"]     = "#303030";

    theme["flistFontColor"]         = "#d3d3d3";
    theme["flinfoFontColor"]        = "#303030";
    theme["fltextFontColor"]        = "#d3d3d3";
    theme["flwidgetFontColor"]      = "#d3d3d3";
    theme["flsearchFontColor"]      = "#d3d3d3";

    theme["findBackground"]         = "#303020";
    theme["textboxBackground"]      = "#303030";
    theme["infopanelBackground"]    = "#ff0024";
    theme["clipboardBackground"]    = "#382a36";
    theme["mainwindowBackground"]   = "#303030";
    theme["commandlineBackground"]  = "#303030";

    theme["findFontColor"]          = "#d3d3d3";
    theme["textboxFontColor"]       = "#d3d3d3";
    theme["infopanelFontColor"]     = "#303030";
    theme["clipboardFontColor"]     = "#d3d3d3";
    theme["commandlineFontColor"]   = "#d3d3d3";

    theme["linenumCurrentLineColor"]= "#ff2424";
    theme["linenumBackground"]      = "#303030";
    theme["linenumFontColor"]       = "#808080";

    return theme;
}
