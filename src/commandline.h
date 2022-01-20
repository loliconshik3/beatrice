#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "cmdwidget.h"

#include <QLineEdit>

class CommandLine : public QLineEdit
{
    Q_OBJECT
private:
    void updateShortcuts();

public:
    vector<string> history;
    int historyIndex = 0;

    MainWindow *root = NULL;
    CmdWidget *rootParent = NULL;

    CommandLine(CmdWidget *parent=nullptr);
    void updateWidgetStyle();

public slots:
    vector<string> splitCommand(string command);
    void runLastCommand();

private slots:
    string outToCommand(vector<string> out);
    void completeCommand();
    void launchCommand();
    void saveToHistory();
    void loadHistory();
    void escape();

    void previousCommand();
    void nextCommand();
};

#endif // COMMANDLINE_H
