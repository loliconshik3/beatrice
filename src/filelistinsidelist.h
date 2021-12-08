#ifndef FILELISTINSIDELIST_H
#define FILELISTINSIDELIST_H

#include "filelistwidget.h"

#include <QListWidget>

class FileListInsideList : public QListWidget
{
    Q_OBJECT
public:
    MainWindow *root = NULL;
    FileListWidget *rootParent = NULL;

    FileListInsideList(FileListWidget *parent=nullptr);

    void loadDirectoryFiles(vector<string> files);
    void updateWidgetStyle();
};

#endif // FILELISTINSIDELIST_H
