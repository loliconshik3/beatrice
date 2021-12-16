#ifndef CMDLIST_H
#define CMDLIST_H

#include <QListWidget>
#include "cmdwidget.h"

class CmdList : public QListWidget
{
    Q_OBJECT
public:
    MainWindow *root = NULL;
    CmdWidget *rootParent = NULL;

    CmdList(CmdWidget *parent=nullptr);

    void updateWidgetStyle();

    void loadCommands();
    void redrawCommands();
};

#endif // CMDLIST_H
