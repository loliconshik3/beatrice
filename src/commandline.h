#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QLineEdit>
#include "cmdwidget.h"

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
    void completeArg();

private slots:
    string outToCommand(vector<string> out);
    void launchCommand();
    void completeCommand();
    void saveToHistory();
    void loadHistory();
    void escape();

    void previousCommand();
    void nextCommand();

protected:
    void keyPressEvent(QKeyEvent *e) {

        switch (e->key()) {
            case (Qt::Key_Left): {
                this->previousCommand();
                return;
            }

            case (Qt::Key_Right): {
                this->nextCommand();
                return;
            }

            case (Qt::Key_Space): {
                this->completeArg();
                return;
            }
        }

        QLineEdit::keyPressEvent(e);
    }
};

#endif // COMMANDLINE_H
