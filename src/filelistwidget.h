#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include "mainwindow.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class FilesList;
class FileListSearch;
class FileListInfo;
class FileListText;

class FileListWidget : public QWidget
{
    Q_OBJECT
public:
    MainWindow *root = NULL;

    FileListSearch *flsearch    = NULL;
    FilesList *fileslist        = NULL;
    FileListInfo *flinfo        = NULL;
    FileListText *fltext        = NULL;

    explicit FileListWidget(MainWindow *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *e);
    void hideEvent(QHideEvent *e);

signals:

};

#endif // FILELISTWIDGET_H
