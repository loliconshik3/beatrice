#ifndef FILELISTINFO_H
#define FILELISTINFO_H

#include "filelistwidget.h"

#include <QLabel>

class FileListInfo : public QLabel
{
    Q_OBJECT
public:
    MainWindow *root = NULL;
    FileListWidget *rootParent = NULL;

    FileListInfo(FileListWidget *parent=nullptr);

    void updateWidgetStyle();
};

#endif // FILELISTINFO_H
