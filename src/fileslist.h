#ifndef FILESLIST_H
#define FILESLIST_H

#include <QListWidget>
#include "filelistwidget.h"
#include <string>

using namespace std;

class MainWindow::FileListWidget::FilesList : public QListWidget
{
    Q_OBJECT
public:

    const string BACK_NAME = ">> BACK <<";

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
    void openFile();
    void deleteFile();
};

#endif // FILESLIST_H
