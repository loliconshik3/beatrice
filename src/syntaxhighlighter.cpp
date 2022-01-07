#include "syntaxhighlighter.h"
#include "mainwindow.h"
#include "textbox.h"
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
        {"//.*\\s", "#8a8d93"},
        {"[0-9]|=", "#71c919"},
        {"\".*\"", "#71c919"},
        //{"(?=function )(.*)(?= takes)", "#71c919"},
        {"\\b(if|then|else|elseif|endif|loop|exitwhen|endloop)\\b", "#d097a3"},
        {"\\b(this|null|boolexpr|nothing|array|unit|string|integer|real|boolean|location|timer|group)\\b", "#c95b8e"},
        {"\\b(takes|returns|set|call|private|public|globals|endglobals|local|function|endfunction|library|endlibrary|struct|endstruct|method|endmethod)\\b", "#ff8080"}
    };

    return patterns;
}

map<string, string> SyntaxHighlighter::generatePython() {
    map<string, string> patterns = {
        //{"/*.**/", "#8a8d93"},
        {"[0-9]|=", "#71c919"},
        {"\".*\"", "#71c919"},
        {"'.*'", "#71c919"},
        //{"(?=def )(.*)(?=()", "#71c919"},
        {"\\b(if|else|elif|for|in|while|break|try|except|pass|return)\\b", "#d097a3"},
        {"\\b(self|True|False)\\b", "#c95b8e"},
        {"\\b(def|class|from|import|async|as)\\b", "#ff8080"},
        {"\\b((import) .*|(from) .*)\\b", "#d097a3"},
        {"\\b((def) .*|(class) .*)\\b", "#71c919"},
    };

    return patterns;
}


map<string, string> SyntaxHighlighter::generateSharp() {
    map<string, string> patterns = {
        //{"/*.**/", "#8a8d93"},
        {"[0-9]|=", "#71c919"},
        {"\".*\"", "#71c919"},
        //{"(?=def )(.*)(?=()", "#71c919"},
        {"\\b(if|else|foreach|try|catch|while|break|return)\\b", "#d097a3"},
        {"\\b(this|true|false|new|null|string|int)\\b", "#c95b8e"},
        {"\\b(using|class|namespace|public|private|async|var|await)\\b", "#ff8080"}
    };

    return patterns;
}

map<string, string> SyntaxHighlighter::generateHTML() {
    map<string, string> patterns = {
        //{"<(.*)>", "#c95b8e"},
        {"\".*\"", "#71c919"},
        {"(>).*(<)", "#109010"}
    };

    return patterns;
}

map<string, string> SyntaxHighlighter::generateINI() {
    map<string, string> patterns = {
        {"\\[.*\\]", "#c95b8e"},
        {".*=", "#d097a3"},
        {"=.*", "#71c919"}
    };

    return patterns;
}

map<string, string> SyntaxHighlighter::generateCPP() {
    map<string, string> patterns = {
        //{"/*.**/", "#8a8d93"},
        {"//.*\\s", "#8a8d93"},
        {"\".*\"", "#71c919"},
        {"[0-9]|=|;", "#71c919"},
        //{"(?=function )(.*)(?= takes)", "#71c919"},
        {"\\b(if|else|for|foreach|while)\\b", "#d097a3"},
        {"\\b(const|auto|std|void|string|int|NULL|nullptr|byte|true|false|bool)\\b", "#c95b8e"},
        {"\\b(switch|case|class|using|namespace|return|#include|#define|#ifndef|#endif|public|private|protected)\\b", "#ff8080"}
    };

    return patterns;
}

map<string, string> SyntaxHighlighter::loadSyntax(const string &ext) {
    map<string, string> patterns = {};

    if (!isSyntaxExists(ext)) {
        return patterns;
    }

    if (ext == "j") {
        patterns = generateJASS();
    }
    else if (ext == "py") {
        patterns = generatePython();
    }
    else if (ext == "cs") {
        patterns = generateSharp();
    }
    else if (ext == "cpp" || ext == "h" || ext == "c") {
        patterns = generateCPP();
    }
    else if (ext == "ini") {
        patterns = generateINI();
    }
    else if (ext == "html") {
        patterns = generateHTML();
    }

    return patterns;
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    map<string, string> patterns = loadSyntax(root->currentFile->syntax);
    QTextCharFormat standartFormat;
    QTextCharFormat format;
    QRegExp rx;

    if (patterns.size() > 0) {
        for (const auto &[pattern, color] : patterns) {
            rx = QRegExp(pattern.c_str());

            if (!rx.isValid() || rx.isEmpty() || rx.exactMatch("")) {
                setFormat(0, text.length(), format);
                continue;
            }

            format.setForeground(QBrush(color.c_str()));

            int index = rx.indexIn(text);
            while (index >= 0) {
                int length = rx.matchedLength();

                QTextCharFormat form = SyntaxHighlighter::format(index);
                if (form == standartFormat) {
                    setFormat(index, length, format);
                }

                index = rx.indexIn(text, index+length);
            }
       }
    }

    if (root->macros->macrosList.size() > 0) {
        for (const auto &[name, txt] : root->macros->macrosList) {
            QString rxStr = name.c_str();
            rx = QRegExp(rxStr);

            if (!root->macros->isMacrosHasExtension(name, root->currentFile->extension)) {
                continue;
            }

            format.setForeground(QBrush(root->theme["textboxFontColor"].c_str()));
            format.setFontUnderline(true);

            int index = rx.indexIn(text);
            while (index >= 0) {
                int length = rx.matchedLength();
                setFormat(index, length, format);
                index = rx.indexIn(text, index+length);
            }
        }
    }
}
