#ifndef REPLACETEXT_H
#define REPLACETEXT_H

#include "mainwindow.h"
#include "findwidget.h"

#include <QLineEdit>

class ReplaceText : public QLineEdit
{
    Q_OBJECT
public:
    MainWindow *root;
    FindWidget *rootParent;

    ReplaceText(FindWidget *parent=nullptr);

private slots:
    void updateWidgetStyle();
    void updateShortcuts();
    void replace();
};

#endif // REPLACETEXT_H
