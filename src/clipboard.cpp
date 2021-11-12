#include "clipboard.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QShortcut>

Clipboard::Clipboard(MainWindow *parent)
    : QListWidget(parent)
{
    root = parent;
    setWindowFlags(Qt::Popup);

    setGeometry(0, 0, root->cfg->clipboardWidth, root->cfg->clipboardHeight);
    move(root->cfg->clipboardX, root->cfg->clipboardY);

    QString style = QString("QListWidget { color: %1; border: none; background: %2; }")
                    .arg(root->theme["clipboardFontColor"].c_str(),
                         root->theme["clipboardBackground"].c_str());
    setStyleSheet(style);

    QFont fnt(root->cfg->clipboardFontFamily.c_str());
    fnt.setPixelSize(root->cfg->clipboardFontSize);
    setFont(fnt);

    //setStyleSheet("QListWidget { font-size: 16px; font-family: Source Code Pro; color: lightGray; border: none; background: #2a2b2e; }");

    QShortcut *shortcut = new QShortcut(QKeySequence("Escape"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(hide()));

    shortcut = new QShortcut(QKeySequence("Return"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(insertClipboardText()));

    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
}

void Clipboard::clipboardDataChanged() {
    if (clipboardList.size() == root->cfg->clipboardMaxSize) {
        clipboardList.erase(clipboardList.begin()+root->cfg->clipboardMaxSize);
    }
    clipboardList.insert(clipboardList.begin(), clipboard->text());
}

void Clipboard::loadClipboardList() {
    clear();
    int max = clipboardList.size()-1;
    int index = 0;
    while (index <= max) {
        QListWidgetItem *item = new QListWidgetItem(clipboardList[index]);
        item->setSizeHint(QSize(0, 20));
        addItem(item);
        ++index;
    }
    setCurrentItem(item(0));
    show();
}

void Clipboard::insertClipboardText() {
    if (count() > 0) {
        QListWidgetItem *item = currentItem();
        root->textbox->insertPlainText(item->text());
    }
    hide();
}
