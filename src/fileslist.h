#ifndef FILESLIST_H
#define FILESLIST_H

#include "utils.h"
#include "filelistwidget.h"
#include "filelistinfo.h"

#include <QListWidget>
#include <string>

using namespace std;

class MainWindow::FileListWidget::FilesList : public QListWidget
{
    Q_OBJECT
private:
    const string BACK_NAME = ">> BACK <<";
    const string PICK_NAME = ">> PICK DIRECTORY <<";

public:
    map<string, string> files;
    MainWindow *root;
    FileListWidget *rootParent = NULL;

    FilesList(FileListWidget *parent=nullptr);

    void loadDirectoryFiles(string path);
    void loadLastFiles();
    void loadTabFiles();

public slots:
    void redrawFiles();

private slots:
    void currentItemChanged();
    void openFile();
    void deleteFile();
};

#endif // FILESLIST_H
