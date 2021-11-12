#ifndef FILELISTSEARCH_H
#define FILELISTSEARCH_H

#include <QLineEdit>
#include "filelistwidget.h"

class FileListSearch : public QLineEdit
{
    Q_OBJECT
private:
    void updateShortcuts();

public:
    MainWindow *root;
    FileListWidget *rootParent = NULL;

    FileListSearch(FileListWidget *parent=nullptr);

    void updateWidgetStyle();

public slots:
    void filterList();
    void Up();
    void Down();
};

#endif // FILELISTSEARCH_H
