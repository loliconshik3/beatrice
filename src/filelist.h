#ifndef FILESLIST_H
#define FILESLIST_H

#include "utils.h"
#include "filelistwidget.h"
#include "filelistinfo.h"

#include <QListWidget>
#include <string>

using namespace std;

class FileList : public QListWidget
{
    Q_OBJECT
private:
    const string BACK_NAME = ">> BACK <<";
    const string PICK_NAME = ">> PICK DIRECTORY <<";
    string currentDirectory;

    void updateShortcuts();

public:
    map<string, string> files;
    map<string, string> filesText;

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
    void deleteFile();
    void setFileText(string name, QFile &file);
};

#endif // FILESLIST_H
