#include "syntaxhighlighter.h"
#include "mainwindow.h"
#include "utils.h"

#include <QTextCharFormat>

#include <vector>
#include <string>

using namespace std;

SyntaxHighlighter::SyntaxHighlighter(QObject *parent, MainWindow *proot) : QSyntaxHighlighter(parent)
{
	root = proot;
}

map<string, string> SyntaxHighlighter::generateJASS() {
    map<string, string> patterns = {
        //{"/*.**/", "#8a8d93"},
        {"[0-9]|=", "#71c919"},
        //{"(?=function )(.*)(?= takes)", "#71c919"},
        {"\\b(if|then|endif|loop|exitwhen|endloop)\\b", "#d097a3"},
        {"\\b(this|null|boolexpr|nothing|array|unit|integer|real|boolean|location|timer|group)\\b", "#c95b8e"},
        {"\\b(takes|returns|set|call|private|public|globals|endglobals|local|function|endfunction|library|endlibrary|struct|endstruct|method|endmethod)\\b", "#ff8080"}
    };

    return patterns;
}

map<string, string> SyntaxHighlighter::generatePython() {
    map<string, string> patterns = {
        //{"/*.**/", "#8a8d93"},
        {"[0-9]|=", "#71c919"},
        //{"(?=def )(.*)(?=()", "#71c919"},
        {"\\b(if|for|in|while|break|try|except|pass|return)\\b", "#d097a3"},
        {"\\b(self|True|False)\\b", "#c95b8e"},
        {"\\b(def|class|from|import)\\b", "#ff8080"}
    };

    return patterns;
}


map<string, string> SyntaxHighlighter::generateSharp() {
    map<string, string> patterns = {
        //{"/*.**/", "#8a8d93"},
        {"[0-9]|=", "#71c919"},
        //{"(?=def )(.*)(?=()", "#71c919"},
        {"\\b(if|else|foreach|try|catch|while|break|return)\\b", "#d097a3"},
        {"\\b(this|true|false|new|null|string|int)\\b", "#c95b8e"},
        {"\\b(using|class|namespace|public|private|async|var|await)\\b", "#ff8080"}
    };

    return patterns;
}

map<string, string> SyntaxHighlighter::loadSyntax(const string &ext) {
    map<string, string> patterns = {};

    if (ext == "j") {
        patterns = generateJASS();
    }
    else if (ext == "py") {
        patterns = generatePython();
    }
    else if (ext == "cs") {
        patterns = generateSharp();
    }

    return patterns;
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    map<string, string> patterns = loadSyntax(getFileExt(getFilename(root->filename)));
    QTextCharFormat format;
    QRegExp rx;

    if (patterns.size() > 0) {
        for (const auto &[pattern, color] : patterns) {
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
}
