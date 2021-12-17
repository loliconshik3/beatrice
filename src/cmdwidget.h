#ifndef CMDWIDGET_H
#define CMDWIDGET_H

#include "mainwindow.h"

#include <QWidget>

class CommandLine;
class CmdList;

class CmdWidget : public QWidget
{
    Q_OBJECT
public:
    map<string, string> COMMAND_LIST = {
        {"Text: change tab size", "tabsize"},
        {"File: open", "open"},
        {"File: save", "save"},
        {"Editor: quit", "quit"},
        {"Editor: about", "about"},
        {"File: new", "new"},
        {"Editor: open config file", "cfg"},
        {"Text: find", "find"},
        {"Bash: launch command", "cmd"}
    };
    /*vector<string> COMMAND_LIST = {
        "tabsize",
        "open",
        "save",
        "quit",
        "about",
        "new",
        "cfg",
        "find",
        "cmd"
    };*/

    /*map<string, string, string> COMMAND_LIST = {
        {"tabsize", "int=1:12", "Change tab size (tabsize)", },
        {"open",    "str", "Open file (open)" },
        {"save",    "str", "Save file (save)", },
        {"quit",    "none", "Quit (quit)", },
        {"about",   "none", "About editor (about)", },
        {"new",     "none", "Create new file (new)", },
        {"cfg",     "none", "Open config file (cfg)", },
        {"find",    "str", "Find text (find)", },
        {"cmd",     "str", "Bash command (cmd)", }
    };*/

    MainWindow *root = NULL;
    explicit CmdWidget(MainWindow *parent = nullptr);

    CommandLine *cmd = NULL;
    CmdList *cmdList = NULL;

    void updateWidgetStyle();

public slots:
    void previousItem();
    void nextItem();
};

#endif // CMDWIDGET_H
