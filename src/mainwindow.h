#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include "file.h"

#include <QMainWindow>
#include <QGridLayout>
#include <string>

using namespace std;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    string filename = "Untitled";
    string filetext = "";
    string currentDir = "";

    vector<File> files;
    File currentFile;

    Config *cfg = new Config();

    class FileListWidget;
    class Textbox;
    class InfoPanel;
    class CommandLine;

    Textbox *textbox = NULL;
    InfoPanel *infopanel = NULL;
    CommandLine *commandline = NULL;
    FileListWidget *flwidget = NULL;

    MainWindow(QWidget *parent = nullptr);

    void UpdateTitle();
    void saveLastFile();
    void loadLastFile();
    void updateShortcuts();
    bool isFileOld();

public slots:
    void NewFile();
    void openUpFile();
    void openDownFile();
    void insertSaveCommand(bool saveas=false);
    void SaveFile(QString name=nullptr);
    void OpenFile(QString path=nullptr);
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
