#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../api/macroslist.h"
#include "../api/config.h"
#include "../api/utils.h"
#include "../api/file.h"

#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QCloseEvent>
#include <string>

class FileListWidget;
class CmdWidget;
class FindWidget;
class InfoPanel;
class Clipboard;
class Textbox;

using namespace std;
namespace fs = std::filesystem;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    string currentDirectory = fs::current_path();
    File *currentFile       = NULL;
    vector<File*> files     = {};

    MacrosList *macrosList      = new MacrosList();
    Config *cfg                 = new Config();
    map<string, string> theme   = cfg->themeColors;

    Clipboard *clip             = NULL;
    Textbox *textbox            = NULL;
    InfoPanel *infopanel        = NULL;
    FindWidget *findwidget      = NULL;
    CmdWidget *commandline      = NULL;
    FileListWidget *flwidget    = NULL;

    MainWindow(QWidget *parent = nullptr);

    void updateTitle();
    void saveLastFile();
    void updateShortcuts();
    void updateWidgetStyle();

public slots:
    void newFile();
    void openFile(QString path=nullptr);
    void saveFile(QString path=nullptr);
    void updateCurrentFile(File *file);
    void closeCurrentFile();

    void runLastCommand();
    void openUpFile();
    void openDownFile();
    void insertFindCommand();
    void insertSaveCommand(bool saveas=false);
    void openFolder(QString path=nullptr);
    void openCmd();
    void aboutScreen();
    void showCurrentDirFiles();
    void showLastFiles();
    void showTabFiles();

    void Exit() {
        exit(0);
    }

    void loadSessionFiles();

private:
    void saveSessionFiles();
    void reloadConfig();

};
#endif // MAINWINDOW_H
