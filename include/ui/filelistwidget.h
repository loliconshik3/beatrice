#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include "mainwindow.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class FileList;
class FileListSearch;
class FileListInfo;
class FileListText;
class FileListInsideList;

class FileListWidget : public QWidget
{
    Q_OBJECT
public:
    MainWindow *root = NULL;

    FileListInsideList *flinsideList = NULL;
    FileListSearch *flsearch    = NULL;
    FileList *filelist          = NULL;
    FileListInfo *flinfo        = NULL;
    FileListText *fltext        = NULL;

    explicit FileListWidget(MainWindow *parent = nullptr);

    void updateWidgetStyle();

protected:
    void closeEvent(QCloseEvent *e);
    void hideEvent(QHideEvent *e);

signals:

};

#endif // FILELISTWIDGET_H
