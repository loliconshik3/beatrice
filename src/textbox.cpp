#include "textbox.h"
#include "mainwindow.h"
#include "infopanel.h"
#include "linenumberarea.h"
#include "utils.h"

#include <list>

#include <Qt>
#include <QShortcut>
#include <QtWidgets>

Textbox::Textbox(MainWindow *parent) :
    QTextEdit(parent)
{
    root = parent; //font-size: 17px; font-family: Source Code Pro;
    setStyleSheet("QTextEdit { color: lightGray; border: none; background: #1f222d; }");

    QFont fnt(root->cfg->textboxFontFamily.c_str());
    fnt.setPixelSize(root->cfg->textboxFontSize);
    setFont(fnt);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    highlighter = new SyntaxHighlighter(this, parent);
    highlighter->setDocument(this->document());

    updateShortcuts();
    updateLineNumberAreaWidth(0);
}

int Textbox::countOfTabs(string str) {
    int tabs = 0;
    int symb = 0;
    int spaces = 0;
    for (const auto chr : str) {
        if (chr == ' ') {
            spaces++;
        }
        else {
            symb++;
            break;
        }
        if (spaces == tabSize) {
            spaces=0;
            tabs++;
        }
    }
    return tabs;
}


void Textbox::tabulation() {
    QTextCursor cursor  = textCursor();
    int lines           = getSelectedLines(cursor);

    if (lines == 0) {
        insertPlainText(tabString);
    }
    else {
        int start   = cursor.selectionStart();
        int end     = cursor.selectionEnd();
        cursor.setPosition(start);
        int lineNum = cursor.block().blockNumber()+1;
        setTextCursor(cursor);

        int line = lineNum;
        while (line < lines+lineNum) {
            end += tabSize;
            insertTabAtLine();
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            ++line;
        }
        cursor.setPosition(start);
        cursor.setPosition(end, QTextCursor::KeepAnchor);
        setTextCursor(cursor);
    }
}

void Textbox::removeTabAtLine() {
    QTextCursor cursor  = textCursor();
    int lines           = getSelectedLines(cursor);
    int lineNumer       = cursor.block().blockNumber();

    if (lines == 0) {
        if (countOfTabs(cursor.block().text().toStdString()) == 0) {
            return;
        }

        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumer);
        setTextCursor(cursor);

        int index = tabSize;
        while (index > 0) {
            cursor.deleteChar();
            --index;
        }

        setTextCursor(cursor);
    }
    else {
        int start   = cursor.selectionStart();
        int end     = cursor.selectionEnd();
        cursor.setPosition(start);
        int lineNum = cursor.block().blockNumber()+1;
        setTextCursor(cursor);

        int line = lineNum;
        while (line < lines+lineNum) {
            if (countOfTabs(cursor.block().text().toStdString()) > 0) {
                end -= tabSize;
                removeTabAtLine();
            }
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
            ++line;
        }
        cursor.setPosition(start);
        cursor.setPosition(end, QTextCursor::KeepAnchor);
        setTextCursor(cursor);
    }
}

void Textbox::insertTabAtLine() {
    QTextCursor cursor  = textCursor();
    int lineNumer       = cursor.block().blockNumber();
    int columnNumer     = cursor.columnNumber();

    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumer);
    setTextCursor(cursor);
    insertPlainText(tabString);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, columnNumer);
    setTextCursor(cursor);
}

void Textbox::scalePlus() {
    QFont fnt = font();
    int size  = fnt.pixelSize();
    if (size < 30) {
        fnt.setPixelSize(fnt.pixelSize()+1);
    }
    setFont(fnt);
    root->infopanel->updateText();
}

void Textbox::scaleMinus() {
    QFont fnt = font();
    int size  = fnt.pixelSize();
    if (size > 15) {
        fnt.setPixelSize(fnt.pixelSize()-1);
    }
    setFont(fnt);
    root->infopanel->updateText();
}

void Textbox::duplicateLine() {
    QTextCursor cursor = textCursor();
    int lineNumer = cursor.block().blockNumber();

    if (cursor.selectedText() == "") {
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumer);
        cursor.select(QTextCursor::LineUnderCursor);
    }
    QString selectedText = cursor.selectedText();
    cursor.removeSelectedText();
    insertPlainText(selectedText+"\n"+selectedText);

    setTextCursor(cursor);
}

void Textbox::removeLine() {
    QTextCursor cursor = textCursor();
    int lineNumer = cursor.block().blockNumber();

    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumer);
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.removeSelectedText();
    cursor.deleteChar(); // clean up new line

    setTextCursor(cursor);
}

void Textbox::setTabSize(int size) {
    tabSize     = size;
    tabString   = "";

    while (size > 0) {
        tabString += " ";
        size --;
    }

    root->infopanel->updateText();
}

string Textbox::getCursorPos() {
    return to_string(textCursor().block().blockNumber()+1) + "," + to_string(textCursor().columnNumber()+1);
}

void Textbox::enterKey() {
    QTextCursor cursor = textCursor();

    if (cursor.selectedText() != "") {    
        cursor.removeSelectedText();
    }

    string cursorPos = getCursorPos();

    char sep = ',';
    vector<string> out;
    split(cursorPos, sep, out);
    int lineNumer = stoi(out[0]);
    int columnNumer = stoi(out[1]);

    cursor.select(QTextCursor::LineUnderCursor);
    QString selectedText = cursor.selectedText();
    cursor.clearSelection();

    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumer-1);
    if (columnNumer > 1)
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, columnNumer-1);

    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
    QString befChar = cursor.selectedText();
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);

    int tabs = countOfTabs(selectedText.toStdString());

    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumer-1);
    if (columnNumer > 1)
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, columnNumer-1);

    insertPlainText("\n");

    cursor.setVerticalMovementX(lineNumer);
    setTextCursor(cursor);

    list<QString> tabList = {
        ":", "[", "{", "("
    };
    bool found = (std::find(tabList.begin(), tabList.end(), befChar) != tabList.end());
    if (found) {
        tabs++;
    }

    while (tabs > 0) {
        insertPlainText(tabString);
        tabs--;
    }
}

string Textbox::getAroundChars() {
    QTextCursor cursor = textCursor();
	string aroundChars = "";

	cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
    aroundChars = cursor.selectedText().toStdString();
	
	return aroundChars;
}

void Textbox::backspace() {
    QTextCursor cursor = textCursor();

    if (cursor.selectedText() != "") {
        cursor.removeSelectedText();
    }
    else if (getCursorPos() != "1,1") {
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, tabSize);
        QString selectedText = cursor.selectedText();
        int selectedLength   = selectedText.length();
        if (selectedText == tabString) {
            cursor.removeSelectedText();
            setTextCursor(cursor);
            return;
        }
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, selectedLength);

        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
        selectedText = cursor.selectedText();
        selectedLength = selectedText.length();
        cursor.clearSelection();

        if (selectedLength > 1) {
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
        }

        cursor.deletePreviousChar();

        std::list <QString> test = {
            "()",
            "[]",
            "\"\"",
            "{}",
            "''"
        };

        for (const QString &string : test) {
            if (selectedText == string) {
                cursor.deleteChar();
                break;
            }
        }
    }

    setTextCursor(cursor);
}

void Textbox::moveCursorBack() {
    QTextCursor cursor = textCursor();

    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);

    setTextCursor(cursor);
}

void Textbox::moveCursorForward() {
	QTextCursor cursor = textCursor();

    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);

	setTextCursor(cursor);
}

void Textbox::moveCursorRight() {
    QTextCursor cursor = textCursor();
    auto anchorState = QTextCursor::MoveAnchor;

    if(QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)){
        anchorState = QTextCursor::KeepAnchor;
    }

    QString selectedText = cursor.selectedText();
    int selectedSize = selectedText.length();

    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, tabSize);

    string checkTabText = cursor.selectedText().toStdString();
    int length = checkTabText.length();
    int selTabSize = length - selectedSize;

    if (selTabSize >= 0) {
        if (length > (selTabSize+1)) {
            checkTabText = checkTabText.substr(length-1-selTabSize, length-1);
        }
    }
    /*else {
        selTabSize = tabSize;
    }*/

    //cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, tabSize);
    //int selTabSize = cursor.selectedText().length() - selectedSize;
    if (checkTabText.c_str() != tabString) {
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, selTabSize);
        cursor.movePosition(QTextCursor::Right, anchorState, 1);
    }
    else {
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, selTabSize);
        cursor.movePosition(QTextCursor::Right, anchorState, selTabSize);
    }

    setTextCursor(cursor);
}

void Textbox::moveCursorLeft() {
    QTextCursor cursor = textCursor();
    auto anchorState = QTextCursor::MoveAnchor;

    if(QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)){
        anchorState = QTextCursor::KeepAnchor;
    }

    QString selectedText = cursor.selectedText();
    int selectedSize = selectedText.length();

    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, tabSize);

    string checkTabText = cursor.selectedText().toStdString();
    int length = checkTabText.length();
    int selTabSize = length - selectedSize;

    if (selTabSize >= 0) {
        if (length > (selTabSize+1)) {
            checkTabText = checkTabText.substr(length-1-selTabSize, length-1);
        }
    }

    if (checkTabText.c_str() != tabString) {
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, selTabSize);
        cursor.movePosition(QTextCursor::Left, anchorState, 1);
    }
    else {
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, selTabSize);
        cursor.movePosition(QTextCursor::Left, anchorState, selTabSize);
    }

    setTextCursor(cursor);
}

void Textbox::completeQuotes(string quote) {
    string totalQuote   = quote + quote;
    string achars       = getAroundChars();

    if (achars.length() > 1 && (achars.substr(0) != quote && achars.substr(1) == quote)) {
        moveCursorForward();
        return;
    }

    if (achars != totalQuote) {
        insertPlainText(totalQuote.c_str());
        moveCursorBack();
    }
    else {
        moveCursorForward();
    }
}

void Textbox::completeBrackets(string bracket, bool isNew) {
    map<string, string> bracketsList = {
        {"(", ")"},
        {"{", "}"},
        {"[", "]"}
    };
    map<string, string> oppositeBracketsList = {
        {")", "("},
        {"}", "{"},
        {"]", "["}
    };
    string totalBrackets = "";

    if (isNew) {
        string oppositeBracket = bracketsList[bracket];
        totalBrackets = bracket + oppositeBracket;
    }
    else {
        string oppositeBracket = oppositeBracketsList[bracket];
        totalBrackets = oppositeBracket + bracket;
    }

    if (getAroundChars() != totalBrackets) {
        if (isNew) {
            insertPlainText(totalBrackets.c_str());
            moveCursorBack();
        }
        else {
            insertPlainText(bracket.c_str());
        }
    }
    else {
        moveCursorForward();
    }

}


void Textbox::onTextChanged() {
    root->currentFile->text = toPlainText().toStdString();
    root->updateTitle();
    root->infopanel->updateText();
}

int Textbox::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    //13 +
    int space = 6 +  fontMetrics().width(QLatin1Char('9')) * (digits);

    return space;
}

void Textbox::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void Textbox::updateLineNumberArea(QRectF /*rect_f*/)
{
    Textbox::updateLineNumberArea();
}
void Textbox::updateLineNumberArea(int /*slider_pos*/)
{
    Textbox::updateLineNumberArea();
}
void Textbox::updateLineNumberArea()
{
    root->currentFile->cursorPosition = textCursor().position();
    root->infopanel->updateText();

    /*
     * When the signal is emitted, the sliderPosition has been adjusted according to the action,
     * but the value has not yet been propagated (meaning the valueChanged() signal was not yet emitted),
     * and the visual display has not been updated. In slots connected to this signal you can thus safely
     * adjust any action by calling setSliderPosition() yourself, based on both the action and the
     * slider's value.
     */
    // Make sure the sliderPosition triggers one last time the valueChanged() signal with the actual value !!!!
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    // Since "QTextEdit" does not have an "updateRequest(...)" signal, we chose
    // to grab the imformations from "sliderPosition()" and "contentsRect()".
    // See the necessary connections used (Class constructor implementation part).

    QRect rect =  this->contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);
    //----------
    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        lineNumberArea->scroll(0, dy);
    }

    // Addjust slider to alway see the number of the currently being edited line...
    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || this->textCursor().block().blockNumber() == first_block_id-1)
        this->verticalScrollBar()->setSliderPosition(dy-this->document()->documentMargin());

//    // Snap to first line (TODO...)
//    if (first_block_id > 0)
//    {
//        int slider_pos = this->verticalScrollBar()->sliderPosition();
//        int prev_block_height = (int) this->document()->documentLayout()->blockBoundingRect(this->document()->findBlockByNumber(first_block_id-1)).height();
//        if (dy <= this->document()->documentMargin() + prev_block_height)
//            this->verticalScrollBar()->setSliderPosition(slider_pos - (this->document()->documentMargin() + prev_block_height));
//    }

}


void Textbox::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);

    QRect cr = this->contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


int Textbox::getFirstVisibleBlockId()
{
    // Detect the first block for which bounding rect - once translated
    // in absolute coordinated - is contained by the editor's text area

    // Costly way of doing but since "blockBoundingGeometry(...)" doesn't
    // exists for "QTextEdit"...

    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                        this->verticalScrollBar()->sliderPosition()
                        ) ).toRect();

        if (r1.contains(r2, true)) { return i; }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

void Textbox::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), *new QColor("#1f222d"));//*new QColor("#404244")); //
    int blockNumber = this->getFirstVisibleBlockId();

    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
    int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;

    int top = this->viewport()->geometry().top();

    // Adjust text position according to the previous "non entirely visible" block
    // if applicable. Also takes in consideration the document's margin offset.
    int additional_margin;
    if (blockNumber == 0)
        // Simply adjust to document's margin
        additional_margin = (int) this->document()->documentMargin() -1 - this->verticalScrollBar()->sliderPosition();
    else
        // Getting the height of the visible part of the previous "non entirely visible" block
        additional_margin = (int) this->document()->documentLayout()->blockBoundingRect(prev_block)
        .translated(0, translate_y).intersected(this->viewport()->geometry()).height();

    // Shift the starting point
    top += additional_margin;

    int bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();

    QColor col_1(90, 255, 30);      // Current line (custom green)
    QColor col_0(Qt::gray);         // Other lines  (custom darkgrey)

    // Draw the numbers (displaying the current line number in green)
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(120, 120, 120));
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ? col_1 : col_0);
            painter.drawText(-5, top, //-0
                             lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number); //Qt::AlignCenter
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }

}

void Textbox::clearSelection() {
    QTextCursor cur = textCursor();
    cur.clearSelection();
    setTextCursor(cur);
}

void Textbox::updateShortcuts() {

    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_removeLine), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(removeLine()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_duplicateLine), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(duplicateLine()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_scalePlus), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(scalePlus()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_scaleMinus), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(scaleMinus()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_insertTabAtLine), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(insertTabAtLine()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_removeTabAtLine), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(removeTabAtLine()));

    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

    // Line numbers
    lineNumberArea = new LineNumberArea(this);
    ///
    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea/*_2*/(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));
    ///
}
