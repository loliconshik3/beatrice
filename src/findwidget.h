#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include "mainwindow.h"

#include <QWidget>

class FindText;

class FindWidget : public QWidget
{
    Q_OBJECT
public:
    MainWindow *root    = NULL;
    FindText *findtext  = NULL;

    explicit FindWidget(MainWindow *parent = nullptr);

    void updateSize();

signals:

};

#endif // FINDWIDGET_H
