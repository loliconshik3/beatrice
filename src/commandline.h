#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QLineEdit>
#include "mainwindow.h"

class CommandLine : public QLineEdit
{
    Q_OBJECT
private:
    void updateShortcuts();

public:
    vector<string> commandList = {
        "tabsize",
        "open",
        "save",
        "quit",
        "about",
        "new"
    };

    vector<string> history;
    int historyIndex = 0;

    MainWindow *root;

    CommandLine(MainWindow *parent=nullptr);
    void updateWidgetStyle();

public slots:
    void runLastCommand();

private slots:
    string outToCommand(vector<string> out);
    void launchCommand();
    void completeCommand();
    void saveToHistory();
    void loadHistory();
    void escape();

    void previousCommand();
    void nextCommand();
};

#endif // COMMANDLINE_H
