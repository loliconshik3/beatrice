#ifndef FINDTEXT_H
#define FINDTEXT_H

#include "mainwindow.h"
#include "findwidget.h"

#include <QLineEdit>

class FindText : public QLineEdit
{
    Q_OBJECT

private:
    void updateShortcuts();

public:
    MainWindow *root = NULL;

    FindText(FindWidget *parent=nullptr);

private slots:
    void search();
    void searchNext();
    void searchPrevious();
};

#endif // FINDTEXT_H
