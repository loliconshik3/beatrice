#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "syntaxhighlighter.h"
#include "mainwindow.h"

#include <QTextEdit>
#include <QKeyEvent>

#include <QMimeData>

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
    int getCursorX();
    int getCursorY();
    string getAroundChars();

    void updateWidgetStyle();

    void moveToSelectionStart();
    QString getSelectionText();

    void removePreviousWord();
    string getPreviousWord();

public slots:

    void clearSelection();
    void setTabSize(int size);
    void resizeEvent(QResizeEvent *e);

private slots:
    void updateCurrentLineColor();

    bool isShiftPressed();
    bool isControlPressed();

    void scalePlus();
    void scaleMinus();
    void moveLineUp();
    void moveLineDown();
    void moveCursorLeft();
    void moveCursorRight();
    void insertTabAtLine();
    void removeTabAtLine();
    void enterKey();
    void backspace();
    void tabulation();
    void removeLine();
    void duplicateLine();
    void selectCurrentLine();
    int countOfTabs(string str);
    bool nextSymbIsTab(QTextCursor::MoveOperation side);

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
    void dropEvent(QDropEvent *event) {
        QList<QUrl> urls = event->mimeData()->urls();
        for (const auto &url : urls) {
            QString path = url.toLocalFile();
            root->openFile(path);
        }
    }

    void keyPressEvent(QKeyEvent *e) {
        QTextCursor cur = textCursor();
        switch (e->key()) {
            case (Qt::Key_Up): {
                if (cur.blockNumber() == 0) {
                    auto anchorState = QTextCursor::MoveAnchor;
                    if (isShiftPressed()) {
                        anchorState = QTextCursor::KeepAnchor;
                    }
                    cur.setPosition(0, anchorState);
                    setTextCursor(cur);
                }
                else {
                    if (isControlPressed()) {
                        moveLineUp();
                    }
                }
                break;
            }

            case (Qt::Key_Down): {
                if (cur.blockNumber() == this->document()->blockCount()-1) {
                    auto anchorState = QTextCursor::MoveAnchor;
                    if (isShiftPressed()) {
                        anchorState = QTextCursor::KeepAnchor;
                    }
                    cur.setPosition(this->toPlainText().length(), anchorState);
                    setTextCursor(cur);
                }
                else {
                    if (isControlPressed()) {
                        moveLineDown();
                    }
                }
                break;
            }
        }

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

        else if (e->key() == Qt::Key_Left) {
            moveCursorLeft();
        }
        else if (e->key() == Qt::Key_Right) {
            moveCursorRight();
        }

        else {
            QTextEdit::keyPressEvent(e);
        }
    }

};

#endif // TEXTBOX_H
