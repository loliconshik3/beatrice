#include "syntaxhighlighter.h"
#include <QTextCharFormat>

#include <vector>
#include <string>

using namespace std;

SyntaxHighlighter::SyntaxHighlighter(QObject *parent, MainWindow *proot) : QSyntaxHighlighter(parent)
{
	root = proot;
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    return;

    map<string, string> reg_color = {
        {"[0-9]", "#71c919"},
        {"\\b(if|then|endif)\\b", "#d097a3"},
        {"\\b(call|local|function|library|endfunction)\\b", "#ff8080"}
    };

    if (true) { //file ext
        reg_color = { {"", ""} };
    }

    QTextCharFormat format;
    QRegExp rx;

    for (const auto &[pattern, color] : reg_color) {
        rx = QRegExp(pattern.c_str());

        if (!rx.isValid() || rx.isEmpty() || rx.exactMatch("")) {
            setFormat(0, text.length(), format);
            return;
        }

        format.setForeground(QBrush(color.c_str()));

        int index = rx.indexIn(text);
        while (index >= 0) {
            int length = rx.matchedLength();
            setFormat(index, length, format);
            index = rx.indexIn(text, index+length);
        }
   }
}
