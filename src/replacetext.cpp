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
    updateWidgetStyle();
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

void ReplaceText::updateWidgetStyle() {
    QString style = QString("QLineEdit { color: %1; border: none; background: %2; }")
                    .arg(root->theme["findFontColor"].c_str(),
                         root->theme["findBackground"].c_str());
    setStyleSheet(style);

    QFont fnt(root->cfg->infopanelFontFamily.c_str());
    fnt.setPixelSize(root->cfg->infopanelFontSize);
    setFont(fnt);
}

void ReplaceText::updateShortcuts() {
    QShortcut *shortcut = new QShortcut(QKeySequence("Return"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(replace()));
}
