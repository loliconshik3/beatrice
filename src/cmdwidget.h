#ifndef CMDWIDGET_H
#define CMDWIDGET_H

#include "mainwindow.h"

#include <QWidget>

class CommandLine;
class CmdText;

class CmdWidget : public QWidget
{
    Q_OBJECT
public:
    MainWindow *root = NULL;
    explicit CmdWidget(MainWindow *parent = nullptr);

    CommandLine *cmd = NULL;
    CmdText *cmdText = NULL;

    void updateWidgetStyle();
};

#endif // CMDWIDGET_H
