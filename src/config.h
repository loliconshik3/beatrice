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
    //==========THEMES==========
    const string MICRO_THEME    = "Micro";
    const string DIABLO_THEME   = "Diablo";
    const string DRACULA_THEME  = "Dracula";
    //==========================

    QRect rect = QDesktopWidget().availableGeometry();

    bool loadLastSessionFiles = true;
    int clipboardMaxSize = 10;

    int windowMinWidth  = 1000;
    int windowMinHeight = 600;

    int width = rect.width() / 1.5;
    int height = rect.height() / 2;

    //==========CLIPBOARD==========
    int clipboardWidth          = rect.width() / 3;
    int clipboardHeight         = rect.height() / 4;
    int clipboardX              = ( rect.width() - clipboardWidth ) / 2;
    int clipboardY              = ( rect.height() - clipboardHeight ) / 2;
    //=============================

    //==========COMMAND LINE==========
    int cmdWidgetWidth          = rect.width() / 2;
    int cmdWidgetHeight         = rect.height() / 3;
    int cmdWidgetX              = ( rect.width() - cmdWidgetWidth ) / 2;
    int cmdWidgetY              = ( rect.height() - cmdWidgetHeight ) / 2;

    int commandLineWidth        = cmdWidgetWidth;
    int commandLineHeight       = 25;

    int cmdListTopPadding       = commandLineHeight;
    int cmdListWidth            = cmdWidgetWidth;
    int cmdListHeight           = cmdWidgetHeight - cmdListTopPadding;
    //================================

    //==========FILE LIST==========
    int fileListWidgetWidth     = width;
    int fileListWidgetHeight    = height + 20; //LAST height + (height / 2);
    int fileListWidgetX         = ( rect.width() - fileListWidgetWidth ) / 2;
    int fileListWidgetY         = ( rect.height() - fileListWidgetHeight ) / 2;

    int fileListSearchWidth     = fileListWidgetWidth;//LAST fileListWidgetWidth;
    int fileListSearchHeight    = 25;

    int fileListTextWidth       = width / 2; //LAST fileListWidgetWidth;
    int fileListTextHeight      = height - fileListSearchHeight; //LAST height / 2;
    int fileListTextLeftPadding = fileListWidgetWidth - fileListTextWidth;//LAST 0;
    int fileListTextTopPadding  = fileListSearchHeight;//LAST fileListWidgetHeight - fileListTextHeight;

    int fileListInfoWidth       = fileListWidgetWidth; //LAST fileListWidgetWidth;
    int fileListInfoHeight      = 20;
    int fileListInfoTopPadding  = fileListWidgetHeight - fileListInfoHeight;//LAST height - fileListInfoHeight;

    int fileListTopPadding  = 25;
    int fileListWidth       = fileListWidgetWidth / 2; //LAST fileListWidgetWidth;
    int fileListHeight      = height - fileListTopPadding; //LAST - 10 - fileListInfoHeight;
    //=============================

    //==========FONTS==========
    string infoPanelSeparator = " | ";

    string textboxFontFamily    = "Source Code Pro";
    int textboxFontSize         = 17;

    string filelistFontFamily   = "Source Code Pro";
    int filelistFontSize        = 16;

    string infopanelFontFamily  = "Source Code Pro";
    int infopanelFontSize       = 15;

    string clipboardFontFamily  = "Source Code Pro";
    int clipboardFontSize       = 16;

    string commandlineFontFamily= "Source Code Pro";
    int commandlineFontSize     = 15;
    //=========================

    string theme = MICRO_THEME;
    map<string, string> themeColors;

    string terminalCommand = "alacritty --working-directory=$dir$ -e ";

    int newFileCount = 0;

    //==========Shortcuts==========
    QString sct_quit            = "Ctrl+q";
    QString sct_save            = "Ctrl+s";
    QString sct_saveAs          = "Ctrl+Shift+s";
    QString sct_open            = "Ctrl+o";
    QString sct_openFolder      = "Ctrl+k";     //Deprecated. Now work like sct_open
    QString sct_new             = "Ctrl+n";
    QString sct_find            = "Ctrl+f";
    QString sct_run             = "Ctrl+r";     //Run last command in cmd
    QString sct_pasteAs         = "Ctrl+Shift+v";
    QString sct_clearSelection  = "Escape";     //Clear selection in textbox
    QString sct_openCmd         = "Alt+e";      //Open cmd window
    QString sct_about           = "Ctrl+i";     //Show about window
    QString sct_showLastFiles   = "Alt+s";      //Show last opened files
    QString sct_showCurrentDir  = "Ctrl+`";     //Show list of files in current dir (like sct_open)
    QString sct_showTabFiles    = "Alt+o";      //Show list of opened files
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
    QString sct_selectCurrentLine = "Alt+l";    //Select line under text cursor
    //=============================

    Config();
    void loadConfigFile();

private:
    void addConfigFile();
    void setThemeColors();

    map<string, string> loadTheme_Micro();
    map<string, string> loadTheme_Dracula();
    map<string, string> loadTheme_Diablo();
};

#endif // CONFIG_H
