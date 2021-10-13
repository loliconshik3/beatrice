#include "linenumberarea.h"

LineNumberArea::LineNumberArea(MainWindow::Textbox *editor) : QWidget(editor) {
    codeEditor = editor;
}

QSize LineNumberArea::sizeHint() const {
    return QSize(((MainWindow::Textbox *)codeEditor)->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    ((MainWindow::Textbox *)codeEditor)->lineNumberAreaPaintEvent(event);
}
