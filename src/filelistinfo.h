#ifndef FILELISTINFO_H
#define FILELISTINFO_H

#include <QLabel>

#include "filelistwidget.h"

class FileListInfo : public QLabel
{
    Q_OBJECT
public:
    MainWindow *root = NULL;
    FileListWidget *rootParent = NULL;

    FileListInfo(FileListWidget *parent=nullptr);
};

#endif // FILELISTINFO_H
