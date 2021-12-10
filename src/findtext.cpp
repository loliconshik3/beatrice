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
    updateWidgetStyle();
}

void FindText::search() {
    QTextDocument::FindFlags flags = QTextDocument::FindCaseSensitively;
    root->textbox->moveToSelectionStart();
    root->textbox->find(text(), flags);
}

void FindText::searchNext() {
    QTextDocument::FindFlags flags = QTextDocument::FindCaseSensitively;
    root->textbox->clearSelection();
    root->textbox->find(text(), flags);

    if (root->textbox->getSelectionText() == "") {
        QTextCursor cur = root->textbox->textCursor();
        int position = cur.position();

        cur.setPosition(0);
        root->textbox->setTextCursor(cur);
        root->textbox->find(text(), flags);

        if (root->textbox->getSelectionText() == "") {
            cur.setPosition(position);
            root->textbox->setTextCursor(cur);
        }
    }
}

void FindText::searchPrevious() {
    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    root->textbox->moveToSelectionStart();
    root->textbox->find(text(), flags);

    if (root->textbox->getSelectionText() == "") {
        QTextCursor cur = root->textbox->textCursor();
        int position = cur.position();

        cur.setPosition(root->textbox->toPlainText().length()-1);
        root->textbox->setTextCursor(cur);
        root->textbox->find(text(), flags);

        if (root->textbox->getSelectionText() == "") {
            cur.setPosition(position);
            root->textbox->setTextCursor(cur);
        }
    }
}

void FindText::updateWidgetStyle() {
    QString style = QString("QLineEdit { color: %1; border: none; background: %2; }")
                    .arg(root->theme["findFontColor"].c_str(),
                         root->theme["findBackground"].c_str());
    setStyleSheet(style);

    QFont fnt(root->cfg->infopanelFontFamily.c_str());
    fnt.setPixelSize(root->cfg->infopanelFontSize);
    setFont(fnt);
}

void FindText::updateShortcuts() {
    connect(this, &FindText::textChanged, this, &FindText::search);

    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_listMoveDown), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(searchNext()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_listMoveUp), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(searchPrevious()));
}
