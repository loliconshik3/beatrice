#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include "mainwindow.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class FileList;
class FileListSearch;
class FileListInfo;
class FileListText;

class FileListWidget : public QWidget
{
    Q_OBJECT
public:
    MainWindow *root = NULL;

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
