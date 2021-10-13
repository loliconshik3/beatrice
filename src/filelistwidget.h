#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include "mainwindow.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class MainWindow::FileListWidget : public QWidget
{
    Q_OBJECT
public:
    MainWindow *root = NULL;

    class FilesList;
    class FileListSearch;
    FileListSearch *flsearch = NULL;
    FilesList *fileslist = NULL;

    explicit FileListWidget(MainWindow *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *e);
    void hideEvent(QHideEvent *e);

signals:

};

#endif // FILELISTWIDGET_H
