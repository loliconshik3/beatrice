#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include "mainwindow.h"

#include <QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QObject *parent = nullptr, MainWindow *proot = nullptr);

	MainWindow *root;
    void highlightBlock(const QString &text);
    map<string, string> generateJASS();
    map<string, string> generateSharp();
    map<string, string> generatePython();
    map<string, string> generateINI();
    map<string, string> generateCPP();
    map<string, string> loadSyntax(const string &ext);
};

#endif // SYNTAXHIGHLIGHTER_H
