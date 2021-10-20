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

class MainWindow::Textbox : public QTextEdit
{
    Q_OBJECT

public:
    int tabSize = 4;
    QString tabString = "    ";
    bool isNew = true;

    SyntaxHighlighter *highlighter = NULL;

    MainWindow *root;

    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void moveCursorBack();
    void moveCursorForward();
    string getCursorPos();
    string getAroundChars();

signals:


public slots:

    void setTabSize(int size);
    void resizeEvent(QResizeEvent *e);

private slots:

    void scalePlus();
    void scaleMinus();
    void moveCursorLeft();
    void moveCursorRight();
    void insertTabAtLine();
    void enterKey();
    void backspace();
    void tabulation();
    void removeLine();
    void duplicateLine();
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();
    void onTextChanged();
    int countOfTabs(string str);

private:

    QWidget *lineNumberArea;

public:
    Textbox(MainWindow *parent=nullptr);

protected:
    void keyPressEvent(QKeyEvent *e) {
        if (e->key() == Qt::Key_Tab) {
            tabulation();//insertPlainText(tabString);
        }

        /*switch (e->key()) {
            case (Qt::Key_Apostrophe): {

            }
        }*/

        else if (e->key() == Qt::Key_Apostrophe) {
            if (getAroundChars() != "''") {
                QTextEdit::keyPressEvent(e);
                insertPlainText("'");
                moveCursorBack();
            }
            else {
              moveCursorForward();
            }
        }
        else if (e->key() == Qt::Key_QuoteDbl) {
        	if (getAroundChars() != "\"\"") {
	            QTextEdit::keyPressEvent(e);
	            insertPlainText("\"");
	            moveCursorBack();
	        }
	        else {
	        	moveCursorForward();
	        }
        }
        else if (e->key() == Qt::Key_BracketLeft) {
            if (getAroundChars() != "[]") {
                QTextEdit::keyPressEvent(e);
                insertPlainText("]");
                moveCursorBack();
            }
            else {
                moveCursorForward();
            }
        }
        else if (e->key() == Qt::Key_BraceLeft) {
            if (getAroundChars() != "{}") {
                QTextEdit::keyPressEvent(e);
                insertPlainText("}");
                moveCursorBack();
            }
            else {
                moveCursorForward();
            }
        }
        else if (e->key() == Qt::Key_ParenLeft) {
            if (getAroundChars() != "()") {
                QTextEdit::keyPressEvent(e);
                insertPlainText(")");
                moveCursorBack();
            }
            else {
                moveCursorForward();
            }
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
