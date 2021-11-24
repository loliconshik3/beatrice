#include "replacetext.h"
#include "findtext.h"
#include "textbox.h"

#include <QShortcut>

ReplaceText::ReplaceText(FindWidget *parent) : QLineEdit(parent)
{
    root = parent->root;
    rootParent = parent;
    setPlaceholderText("Press enter to replace...");

    updateShortcuts();
}

void ReplaceText::replace() {
    QTextCursor cur = root->textbox->textCursor();
    if (!cur.selectedText().isEmpty()) {
        cur.removeSelectedText();
        root->textbox->setTextCursor(cur);
        root->textbox->insertPlainText(text());
        rootParent->findtext->searchNext();
    }
}

void ReplaceText::updateShortcuts() {
    QShortcut *shortcut = new QShortcut(QKeySequence("Return"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(replace()));
}
