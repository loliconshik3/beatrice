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
        {"Bash: launch command", "cmd"},
        {"File: change syntax", "syntax"}
    };

    MainWindow *root = NULL;
    explicit CmdWidget(MainWindow *parent = nullptr);

    CommandLine *cmd = NULL;
    CmdList *cmdList = NULL;

    void updateWidgetStyle();
    void resizeWidgetByListItems();

public slots:
    void previousItem();
    void nextItem();
};

#endif // CMDWIDGET_H
