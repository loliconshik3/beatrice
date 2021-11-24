#include "findtext.h"
#include "textbox.h"
#include "utils.h"

#include <QShortcut>
#include <QRegExp>

FindText::FindText(FindWidget *parent) : QLineEdit(parent)
{
    root = parent->root;
    setPlaceholderText("Type to search...");

    updateShortcuts();
}

void FindText::search() {
    QTextDocument::FindFlags flags = QTextDocument::FindCaseSensitively;
    root->textbox->moveToSelectionStart();
    QRegExp rx = QRegExp(text());
    root->textbox->find(rx, flags);
}

void FindText::searchNext() {
    QTextDocument::FindFlags flags = QTextDocument::FindCaseSensitively;
    root->textbox->clearSelection();
    QRegExp rx = QRegExp(text());
    root->textbox->find(rx, flags);
}

void FindText::searchPrevious() {
    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    root->textbox->moveToSelectionStart();
    QRegExp rx = QRegExp(text());
    root->textbox->find(rx, flags);
}

void FindText::updateShortcuts() {
    connect(this, &FindText::textChanged, this, &FindText::search);

    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_listMoveDown), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(searchNext()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_listMoveUp), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(searchPrevious()));
}
