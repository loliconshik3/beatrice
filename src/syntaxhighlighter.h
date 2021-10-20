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
};

#endif // SYNTAXHIGHLIGHTER_H
