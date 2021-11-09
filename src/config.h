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

    int width = rect.width() / 2;
    int height = rect.height() / 3;

    int fileListWidgetWidth     = width; //+ (rect.width() / 2 / 3);
    int fileListWidgetHeight    = height + (height / 2);
    int fileListWidgetX         = ( rect.width() - fileListWidgetWidth ) / 2;
    int fileListWidgetY         = ( rect.height() - fileListWidgetHeight ) / 2;

    int fileListSearchWidth     = fileListWidgetWidth;
    int fileListSearchHeight    = 25;

    int fileListTextWidth       = fileListWidgetWidth;
    int fileListTextHeight      = height / 2;
    int fileListTextLeftPadding = 0;//fileListWidgetWidth - fileListTextWidth;
    int fileListTextTopPadding  = fileListWidgetHeight - fileListTextHeight;

    int fileListInfoWidth       = fileListWidgetWidth;
    int fileListInfoHeight      = 20;
    int fileListInfoTopPadding  = height - fileListInfoHeight;

    int fileListTopPadding  = 25;
    int fileListWidth       = fileListWidgetWidth;
    int fileListHeight      = height - fileListTopPadding - 10 - fileListInfoHeight;

    string infoPanelSeparator = " | ";

    string textboxFontFamily    = "Source Code Pro";
    int textboxFontSize         = 17;

    int newFileCount = 0;

    //==========Shortcuts==========
    QString sct_quit            = "Ctrl+q";
    QString sct_save            = "Ctrl+s";
    QString sct_saveAs          = "Ctrl+Shift+s";
    QString sct_open            = "Ctrl+o";
    QString sct_openFolder      = "Ctrl+k";
    QString sct_new             = "Ctrl+n";
    QString sct_find            = "Ctrl+f";
    QString sct_changeFocus     = "Alt+e";      //Change focus from textbox to commandline and conversely
    QString sct_about           = "Ctrl+i";     //Show about window
    QString sct_showLastFiles   = "Alt+s";      //Show last opened files
    QString sct_showCurrentDir  = "Ctrl+`";     //Show list of files in current dir (like sct_open)
    QString sct_showTabFiles    = "Ctrl+p";     //Show list of opened files
    QString sct_openTopFile     = "Alt+Up";     //Move to top opened file
    QString sct_openBottomFile  = "Alt+Down";   //Move to bottom opened file
    QString sct_removeLine      = "Alt+r";      //Remove line under text cursor
    QString sct_duplicateLine   = "Alt+d";      //Duplicate line under text cursor
    QString sct_scalePlus       = "Ctrl+=";     //Increase text scale
    QString sct_scaleMinus      = "Ctrl+-";     //Decrease text scale
    QString sct_insertTabAtLine = "Ctrl+Tab";   //Insert tab at start of current line
    QString sct_removeTabAtLine = "Shift+Tab";  //Remove tab at start of current line
    QString sct_closeCurrentFile= "Ctrl+w";     //Close current file and remove it from file list
    QString sct_launchCommand1  = "Return";     //Launch command in commandline
    QString sct_launchCommand2  = "Enter";      //Launch command in commandline
    QString sct_completeCommand = "Tab";        //Complete last written command
    QString sct_cmdEscape       = "Escape";     //Clear commandline and set focus to textbox
    QString sct_listMoveUp      = "Up";         //Move to up item in files list
    QString sct_listMoveDown    = "Down";       //Move to down item in files list
    QString sct_listOpenFile1   = "Return";     //Open current file in files list
    QString sct_listOpenFile2   = "Enter";      //Open current file in files list
    QString sct_listDeleteFile  = "Ctrl+Delete";//Delete current file in file list
    //=============================

    Config();
};

#endif // CONFIG_H
