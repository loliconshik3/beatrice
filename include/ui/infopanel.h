#ifndef INFOPANEL_H
#define INFOPANEL_H

#include "mainwindow.h"

#include <QLabel>

class InfoPanel : public QLabel
{
    Q_OBJECT

public:
    MainWindow *root;

    InfoPanel(MainWindow *parent=nullptr);

    void updateWidgetStyle();
    void updateText();
};

#endif // INFOPANEL_H
