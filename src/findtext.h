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
    void updateWidgetStyle();

public:
    MainWindow *root = NULL;

    FindText(FindWidget *parent=nullptr);

public slots:
    void search();
    void searchNext();
    void searchPrevious();
};

#endif // FINDTEXT_H
