#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "mainwindow.h"
#include "syntaxhighlighter.h"

#include <QTextEdit>
#include <QKeyEvent>

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Textbox : public QTextEdit
{
    Q_OBJECT
private:
    void updateShortcuts();

public:
    int tabSize         = 4;
    QString tabString   = "    ";
    bool isNew          = true;

    SyntaxHighlighter *highlighter = NULL;

    MainWindow *root;

    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void moveCursorBack();
    void moveCursorForward();
    string getCursorPos();
    string getAroundChars();

public slots:

    void clearSelection();
    void setTabSize(int size);
    void resizeEvent(QResizeEvent *e);

private slots:

    void scalePlus();
    void scaleMinus();
    void moveCursorLeft();
    void moveCursorRight();
    void insertTabAtLine();
    void removeTabAtLine();
    void enterKey();
    void backspace();
    void tabulation();
    void removeLine();
    void duplicateLine();
    int countOfTabs(string str);
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();
    void onTextChanged();
    void completeQuotes(string quote);
    void completeBrackets(string bracket, bool isNew = true);

private:

    QWidget *lineNumberArea;

public:
    Textbox(MainWindow *parent=nullptr);

protected:
    void keyPressEvent(QKeyEvent *e) {
        if (e->key() == Qt::Key_Tab) {
            tabulation();
        }

        /*switch (e->key()) {
            case (Qt::Key_Apostrophe): {

            }
        }*/

        else if (e->key() == Qt::Key_Apostrophe) {
            completeQuotes("'");
        }
        else if (e->key() == Qt::Key_QuoteDbl) {
            completeQuotes("\"");
        }

        else if (e->key() == Qt::Key_BracketLeft) {
            completeBrackets("[", true);
        }
        else if (e->key() == Qt::Key_BracketRight) {
            completeBrackets("]", false);
        }

        else if (e->key() == Qt::Key_BraceLeft) {
            completeBrackets("{", true);
        }
        else if (e->key() == Qt::Key_BraceRight) {
            completeBrackets("}", false);
        }

        else if (e->key() == Qt::Key_ParenLeft) {
            completeBrackets("(", true);
        }
        else if (e->key() == Qt::Key_ParenRight) {
            completeBrackets(")", false);
        }

        else if (e->key() == Qt::Key_Backspace) {
            backspace();
        }
        else if (e->key() == Qt::Key_Return or e->key() == Qt::Key_Enter) {
            enterKey();
        }

        /*else if (e->key() == Qt::Key_Left) {
            moveCursorLeft();
        }
        else if (e->key() == Qt::Key_Right) {
            moveCursorRight();
        }*/

        else {
            QTextEdit::keyPressEvent(e);
        }
    }

};

#endif // TEXTBOX_H
