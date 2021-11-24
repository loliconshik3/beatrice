#include "linenumberarea.h"

LineNumberArea::LineNumberArea(Textbox *editor) : QWidget(editor) {
    codeEditor = editor;

    QFont fnt(editor->root->cfg->textboxFontFamily.c_str());
    fnt.setPixelSize(editor->root->cfg->infopanelFontSize);
    setFont(fnt);
}

QSize LineNumberArea::sizeHint() const {
    return QSize(((Textbox *)codeEditor)->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    ((Textbox *)codeEditor)->lineNumberAreaPaintEvent(event);
}
