#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "textbox.h"

class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    LineNumberArea(MainWindow::Textbox *editor);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    MainWindow::Textbox *codeEditor;
};


#endif // LINENUMBERAREA_H
