#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include "mainwindow.h"

#include <QWidget>

class FindText;
class ReplaceText;

class FindWidget : public QWidget
{
    Q_OBJECT
public:
    MainWindow *root    = NULL;
    FindText *findtext  = NULL;
    ReplaceText *replacetext = NULL;

    explicit FindWidget(MainWindow *parent = nullptr);

    void updateSize();

signals:

};

#endif // FINDWIDGET_H
