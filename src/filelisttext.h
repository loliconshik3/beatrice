#ifndef FILELISTTEXT_H
#define FILELISTTEXT_H

#include "filelistwidget.h"

#include <QTextEdit>

class FileListText : public QTextEdit
{
    Q_OBJECT
public:
    MainWindow *root = NULL;

    FileListText(FileListWidget *parent=nullptr);
};

#endif // FILELISTTEXT_H
