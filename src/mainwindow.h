#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include "file.h"

#include <QMainWindow>
#include <QGridLayout>
#include <string>

class Textbox;
class FileListWidget;
class CommandLine;
class InfoPanel;

using namespace std;
namespace fs = std::filesystem;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    string currentDirectory = fs::current_path();
    vector<QString> clipboard;

    vector<File*> files = {};
    File *currentFile = NULL;

    Config *cfg = new Config();

    Textbox *textbox            = NULL;
    InfoPanel *infopanel        = NULL;
    CommandLine *commandline    = NULL;
    FileListWidget *flwidget    = NULL;

    MainWindow(QWidget *parent = nullptr);

    void UpdateTitle();
    void saveLastFile();
    void updateShortcuts();

public slots:
    void newFile();
    void openFile(QString path=nullptr);
    void saveFile(QString path=nullptr);
    void updateCurrentFile(File *file);
    void closeCurrentFile();

    void openUpFile();
    void openDownFile();
    void insertFindCommand();
    void insertSaveCommand(bool saveas=false);
    void OpenFolder(QString path=nullptr);
    void changeFocus();
    void aboutScreen();
    void showCurrentDirFiles();
    void showLastFiles();
    void showTabFiles();

    void Exit() {
        exit(0);
    }

};
#endif // MAINWINDOW_H
