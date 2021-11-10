#include "linenumberarea.h"

LineNumberArea::LineNumberArea(Textbox *editor) : QWidget(editor) {
    codeEditor = editor;
}

QSize LineNumberArea::sizeHint() const {
    return QSize(((Textbox *)codeEditor)->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    ((Textbox *)codeEditor)->lineNumberAreaPaintEvent(event);
}
