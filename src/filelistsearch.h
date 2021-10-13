#ifndef FILELISTSEARCH_H
#define FILELISTSEARCH_H

#include <QLineEdit>
#include "filelistwidget.h"

class MainWindow::FileListWidget::FileListSearch : public QLineEdit
{
    Q_OBJECT
public:

    MainWindow *root;
    FileListWidget *rootParent = NULL;

    FileListSearch(FileListWidget *parent=nullptr);

public slots:
    void filterList();
    void Up();
    void Down();
};

#endif // FILELISTSEARCH_H
