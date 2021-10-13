#ifndef INFOPANEL_H
#define INFOPANEL_H

#include <QLabel>
#include "mainwindow.h"

class MainWindow::InfoPanel : public QLabel
{
    Q_OBJECT

public:
    MainWindow *root;

    InfoPanel(MainWindow *parent=nullptr);
    void updateText();
};

#endif // INFOPANEL_H
