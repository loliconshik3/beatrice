#ifndef CMDTEXT_H
#define CMDTEXT_H

#include "cmdwidget.h"

#include <QTextEdit>

class CmdText : public QTextEdit
{
    Q_OBJECT
public:
    MainWindow *root = NULL;
    CmdWidget *rootParent = NULL;

    CmdText(CmdWidget *parent);

    void printCommand(QString command);
    void printOutput(QString out);
    void moveScrollbarToEnd();
    void updateWidgetStyle();
};

#endif // CMDTEXT_H
