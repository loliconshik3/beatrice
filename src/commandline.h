#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QLineEdit>
#include "mainwindow.h"

class MainWindow::CommandLine : public QLineEdit
{
    Q_OBJECT

public:
    vector<string> commandList = {
        "tabsize",
        "open",
        "save",
        "quit",
        "about",
        "new"
    };

    MainWindow *root;

    CommandLine(MainWindow *parent=nullptr);

private slots:
    void launchCommand();
    void completeCommand();
    void escape();
};

#endif // COMMANDLINE_H
