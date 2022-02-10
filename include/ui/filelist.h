#ifndef FILESLIST_H
#define FILESLIST_H

#include "filelistwidget.h"
#include "filelistinfo.h"
#include "../api/utils.h"

#include <QListWidget>
#include <string>

using namespace std;

class FileList : public QListWidget
{
    Q_OBJECT
private:
    const string CURRENT_DIR_NAME = ".";
    const string BACK_NAME = "..";
    const string PICK_NAME = ">> PICK DIRECTORY <<";
    string openMode = "STANDART";
    string currentDirectory;

    void updateShortcuts();

public:
    map<string, string> files;
    map<string, string> filesText;
    map<string, vector<string>> directoryFiles;

    MainWindow *root;
    FileListWidget *rootParent = NULL;

    FileList(FileListWidget *parent=nullptr);

    void updateWidgetStyle();

    void loadDirectoryFiles(string path);
    void loadLastFiles();
    void loadTabFiles();

public slots:
    void redrawFiles();

private slots:
    void currentItemChanged();
    void openFile();
    void closeFile();
    void deleteFile();
    void setDirectoryFiles(string path);
    void setFileText(string name, QFile &file);
};

#endif // FILESLIST_H
