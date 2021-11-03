#ifndef CONFIG_H
#define CONFIG_H

#include <QGuiApplication>
#include <QDesktopWidget>
#include <QRect>

#include <string>
using namespace std;

class Config
{
public:

    QRect rect = QDesktopWidget().availableGeometry();

    int windowMinWidth  = 1000;
    int windowMinHeight = 600;

    int fileListSearchWidth     = rect.width() / 2;
    int fileListSearchHeight    = 25;

    int fileListWidgetWidth     = rect.width() / 2;
    int fileListWidgetHeight    = rect.height() / 3;
    int fileListWidgetX         = ( rect.width() - fileListWidgetWidth ) / 2;
    int fileListWidgetY         = ( rect.height() - fileListWidgetHeight ) / 2;

    int fileListTopPadding  = 25;
    int fileListWidth       = rect.width() / 2;
    int fileListHeight      = rect.height() / 3 - fileListTopPadding;

    string infoPanelSeparator = " | ";

    //==========Shortcuts==========
    QString sct_quit            = "Ctrl+q";
    QString sct_save            = "Ctrl+s";
    QString sct_saveAs          = "Ctrl+Shift+s";
    QString sct_open            = "Ctrl+o";
    QString sct_openFolder      = "Ctrl+k";
    QString sct_new             = "Ctrl+n";
    QString sct_changeFocus     = "Alt+e";    //Change focus from textbox to commandline and conversely
    QString sct_about           = "Ctrl+i";   //Show about window
    QString sct_showLastFiles   = "Alt+s";    //Show last opened files
    QString sct_showCurrentDir  = "Ctrl+`";   //Show list of files in current dir (like sct_open)
    QString sct_showTabFiles    = "Ctrl+p";   //Show list of opened files
    QString sct_openTopFile     = "Alt+Up";   //Move to top opened file
    QString sct_openBottomFile  = "Alt+Down"; //Move to bottom opened file
    QString sct_removeLine      = "Alt+r";    //Remove line under text cursor
    QString sct_duplicateLine   = "Alt+d";    //Duplicate line under text cursor
    QString sct_scalePlus       = "Ctrl+=";   //Increase text scale
    QString sct_scaleMinus      = "Ctrl+-";   //Decrease text scale
    QString sct_insertTabAtLine = "Ctrl+Tab"; //Insert tab at start of current line
    //=============================

    Config();
};

#endif // CONFIG_H
