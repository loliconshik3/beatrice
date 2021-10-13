#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QLineEdit>
#include "mainwindow.h"

class MainWindow::CommandLine : public QLineEdit
{
    Q_OBJECT

public:
    MainWindow *root;

    CommandLine(MainWindow *parent=nullptr);

private slots:
    void launchCommand();
    void completeCommand();
    void escape();
};

#endif // COMMANDLINE_H
