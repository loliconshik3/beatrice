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
    root->textbox->moveToSelectionStart();
    QString pattern = text();
    QRegExp rx = QRegExp(pattern);
    root->textbox->find(rx);
}

void FindText::searchNext() {
    root->textbox->clearSelection();
    QString pattern = text();
    QRegExp rx = QRegExp(pattern);
    root->textbox->find(rx);
}

void FindText::searchPrevious() {
    root->textbox->moveToSelectionStart();
    QString pattern = text();
    QRegExp rx = QRegExp(pattern);
    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    root->textbox->find(rx, flags);
}

void FindText::updateShortcuts() {
    connect(this, &FindText::textChanged, this, &FindText::search);

    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_listMoveDown), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(searchNext()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_listMoveUp), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(searchPrevious()));
}
