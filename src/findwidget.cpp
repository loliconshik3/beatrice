#include "findwidget.h"
#include "findtext.h"
#include "textbox.h"
#include "utils.h"

FindWidget::FindWidget(MainWindow *parent) : QWidget(parent)
{
    root = parent;
    setWindowFlags(Qt::Popup);
    //setStyleSheet("QWidget {background: #ffffff}");
}

void FindWidget::updateSize() {
    int width   = root->width();
    int height  = root->height();
    int x       = root->x();
    int y       = root->y();

    int findWidth   = width - 50;
    int findHeight  = 25;
    int findX       = (x + width) - findWidth - 25;
    int findY       = (y + height) - findHeight;

    findtext->setGeometry(0, 0, findWidth, findHeight);
    setGeometry(0, 0, findWidth, findHeight);
    move(findX, findY);

    show();
    QString text = root->textbox->getSelectionText();
    if (!text.isEmpty()) {
        findtext->setText(text);
    }
    else {
        findtext->clear();
    }
    findtext->setFocus();
}
