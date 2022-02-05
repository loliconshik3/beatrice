#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "cmdwidget.h"

#include <QLineEdit>
#include <QProcess>

class CommandLine : public QLineEdit
{
    Q_OBJECT
private:
    void updateShortcuts();

public:
    vector<string> history;
    int historyIndex = 0;
    QProcess *currentProcess = NULL;

    MainWindow *root = NULL;
    CmdWidget *rootParent = NULL;

    CommandLine(CmdWidget *parent=nullptr);
    void updateWidgetStyle();

public slots:
    vector<string> splitCommand(string command);
    void killCurrentProcess();
    void runLastCommand();

private slots:
    void printOutput(QProcess *pr);
    void bashCommandFinished(int, QProcess::ExitStatus);
    void bashCommandError(QProcess::ProcessError);

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
