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

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setAutoFormatting(QTextEdit::AutoNone);
    setAcceptRichText(false);

    highlighter = new SyntaxHighlighter(this, parent);
    highlighter->setDocument(this->document());

    updateShortcuts();
    updateWidgetStyle();
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
        int columns = getCursorY()-1;
        int remaind = columns % tabSize;
        int tSize   = tabSize;
        int size    = tabSize;

        if (remaind > 0) {
            size = tabSize - remaind;
        }

        setTabSize(size);
        insertPlainText(tabString);
        setTabSize(tSize);
    }
    else {
        int start   = cursor.selectionStart();
        int end     = cursor.selectionEnd();

        cursor.setPosition(start);
        cursor.movePosition(QTextCursor::StartOfLine);
        start       = cursor.position();

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
    int position        = cursor.position();
    int cursorX         = getCursorX()-1; // X = LINES
    int cursorY         = getCursorY()-1; // Y = COLUMNS

    if (lines == 0) {
        string text = cursor.block().text().toStdString();
        if (countOfTabs(text) == 0) {
            return;
        }

        cursor.setPosition(position-cursorY);

        int index = tabSize;
        while (index > 0) {
            cursor.deleteChar();
            --index;
        }

        cursor.setPosition(position);
        setTextCursor(cursor);
    }
    else {
        int start   = cursor.selectionStart();
        int end     = cursor.selectionEnd();

        cursor.setPosition(start);
        cursor.movePosition(QTextCursor::StartOfLine);
        start       = cursor.position();

        int lineNum = cursor.block().blockNumber()+1;
        setTextCursor(cursor);

        int line = lineNum;
        while (line < lines+lineNum) {
            string text = cursor.block().text().toStdString();
            if (countOfTabs(text) > 0) {
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
    int cursorX         = getCursorX()-1;
    int cursorY         = getCursorY()-1;
    int position        = cursor.position();

    cursor.setPosition(position-cursorX);
    insertPlainText(tabString);
    cursor.setPosition(position);
    setTextCursor(cursor);
}

void Textbox::scalePlus() {
    QFont lnfnt = lineNumberArea->font();
    int lnSize  = lnfnt.pixelSize();
    QFont fnt   = font();
    int size    = fnt.pixelSize();
    if (size < 30) {
        fnt.setPixelSize(fnt.pixelSize()+1);

        if (lnSize < 28) {
            lnfnt.setPixelSize(lnSize+1);
        }
    }
    setFont(fnt);
    lineNumberArea->setFont(lnfnt);
    root->infopanel->updateText();
}

void Textbox::scaleMinus() {
    QFont lnfnt = lineNumberArea->font();
    int lnSize  = lnfnt.pixelSize();
    QFont fnt = font();
    int size  = fnt.pixelSize();
    if (size > 1) {
        fnt.setPixelSize(fnt.pixelSize()-1);

        if (lnSize > 1) {
            lnfnt.setPixelSize(lnSize-1);
        }
    }
    setFont(fnt);
    lineNumberArea->setFont(lnfnt);
    root->infopanel->updateText();
}

void Textbox::duplicateLine() {
    QTextCursor cursor  = textCursor();
    int cursorPosition  = cursor.position();

    if (cursor.selectedText() == "") {;
        cursor.setPosition(cursorPosition);
        cursor.select(QTextCursor::LineUnderCursor);
    }
    QString selectedText = cursor.selectedText();
    cursor.removeSelectedText();
    insertPlainText(selectedText+"\n"+selectedText);

    setTextCursor(cursor);
}

void Textbox::removeLine() {
    QTextCursor cursor = textCursor();

    cursor.select(QTextCursor::LineUnderCursor);
    cursor.removeSelectedText();
    cursor.deleteChar(); // clean up new line

    if (cursor.block().text() == "") {
        cursor.deletePreviousChar();
    }

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

int Textbox::getCursorX() {
    return textCursor().block().blockNumber()+1;
}

int Textbox::getCursorY() {
    return textCursor().columnNumber()+1;
}

string Textbox::getCursorPos() {
    return to_string(getCursorX()) + "," + to_string(getCursorY());
}

void Textbox::enterKey() {
    QTextCursor cursor = textCursor();

    if (cursor.selectedText() != "") {    
        cursor.removeSelectedText();
    }

    int lineNumer   = getCursorY();
    int position    = textCursor().position();

    cursor.select(QTextCursor::LineUnderCursor);
    QString selectedText = cursor.selectedText();
    int end = cursor.selectionEnd();
    cursor.clearSelection();

    cursor.setPosition(position);
    QString befChar = getAroundChars().substr(0).c_str();
    befChar = befChar.simplified();

    int tabs = countOfTabs(selectedText.toStdString());
    if (tabs * tabSize == selectedText.length()) {
        cursor.setPosition(end);
        setTextCursor(cursor);
    }
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
            "()", "[]", "{}", "\"\"", "''"
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

bool Textbox::nextSymbIsTab(QTextCursor::MoveOperation side) {
    QTextCursor cursor  = textCursor();

    cursor.clearSelection();
    cursor.movePosition(side, QTextCursor::KeepAnchor, tabSize);
    if (cursor.selectedText() == tabString) {
        return true;
    }

    return false;
}

void Textbox::moveCursorRight() {
    QTextCursor cursor  = textCursor();
    auto anchorState    = QTextCursor::MoveAnchor;

    if(QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)){
        anchorState = QTextCursor::KeepAnchor;
    }

    if (!nextSymbIsTab(QTextCursor::Right)) {
        cursor.movePosition(QTextCursor::Right, anchorState, 1);
    }
    else {
        cursor.movePosition(QTextCursor::Right, anchorState, tabSize);
    }

    setTextCursor(cursor);
}

void Textbox::moveCursorLeft() {
    QTextCursor cursor = textCursor();
    auto anchorState = QTextCursor::MoveAnchor;

    if(QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)){
        anchorState = QTextCursor::KeepAnchor;
    }

    if (!nextSymbIsTab(QTextCursor::Left)) {
        cursor.movePosition(QTextCursor::Left, anchorState, 1);
    }
    else {
        cursor.movePosition(QTextCursor::Left, anchorState, tabSize);
    }

    setTextCursor(cursor);
}

void Textbox::completeQuotes(string quote) {
    QTextCursor cursor  = textCursor();
    string totalQuote   = quote + quote;
    string achars       = getAroundChars();

    // insert quotes around selected text
    if (cursor.selectedText().length() > 0) {
        int selectionStart  = cursor.selectionStart();
        int selectionEnd    = cursor.selectionEnd();

        cursor.setPosition(selectionStart);
        setTextCursor(cursor);

        insertPlainText(quote.c_str());

        cursor.setPosition(selectionEnd+1);
        setTextCursor(cursor);

        insertPlainText(quote.c_str());

        cursor.setPosition(selectionStart+1);
        cursor.setPosition(selectionEnd+1, QTextCursor::KeepAnchor);
        setTextCursor(cursor);

        return;
    }

    // if quote insert in "", but not in "'
    if (achars.length() > 1 && (achars.substr(0) != quote && achars.substr(1) == quote)) {
        moveCursorForward();
        return;
    }

    // if chars around cursor != "" or ''
    if (achars != totalQuote) {
        QString ach = achars.c_str();
        ach = ach.simplified();

        list<QString> checkList = {
            "()", "[]", "{}", "\"\"", "''",
            "))", "]]", "}}", "{{", "[[", "((",
        };
        list<QString> checkSolo = {
            "(", ")", "[", "]", "{", "}",
            "'", "\""
        };
        bool found = (std::find(checkList.begin(), checkList.end(), ach) != checkList.end());
        bool foundSolo = false;
        for (const auto &check : checkSolo) {
            if (ach.contains(check)) {
                foundSolo = true;
                break;
            }
        }

        if ((ach.length() > 1 && ( ach.contains(" ") ))
            or ach.length() == 1
            or ach == ""
            or found
            or foundSolo) {

            insertPlainText(totalQuote.c_str());
            moveCursorBack();
        }
        else {
            insertPlainText(quote.c_str());
        }
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
    QTextCursor cursor = textCursor();
    string oppositeBracket = "";
    string totalBrackets = "";

    // if it - first bracket ( or [ or { - beatrice concatinate bracket + opposite bracket
    if (isNew) {
        oppositeBracket = bracketsList[bracket];
        totalBrackets   = bracket + oppositeBracket;
    }
    else {
        oppositeBracket = oppositeBracketsList[bracket];
        totalBrackets   = oppositeBracket + bracket;
    }

    // insert quotes around selected text
    if (cursor.selectedText().length() > 0) {
        int selectionStart  = cursor.selectionStart();
        int selectionEnd    = cursor.selectionEnd();

        cursor.setPosition(selectionStart);
        setTextCursor(cursor);

        insertPlainText(QString(totalBrackets.c_str()).at(0));

        cursor.setPosition(selectionEnd+1);
        setTextCursor(cursor);

        insertPlainText(QString(totalBrackets.c_str()).at(1));

        cursor.setPosition(selectionStart+1);
        cursor.setPosition(selectionEnd+1, QTextCursor::KeepAnchor);
        setTextCursor(cursor);

        return;
    }

    // if chars around cursor != () or [] or {} - input total brackets or just bracket
    if (getAroundChars() != totalBrackets) {
        string achars = getAroundChars();
        if (achars.length() > 1 && achars.substr(1) == totalBrackets.substr(1) && !isNew) {
            moveCursorForward();
            return;
        }

        if (isNew) {
            QString ach = achars.c_str();
            ach = ach.simplified();

            list<QString> checkList = {
                "()", "[]", "{}", "\"\"", "''",
                "))", "]]", "}}", "{{", "[[", "(("
            };
            list<QString> checkSolo = {
                "(", ")", "[", "]", "{", "}",
                "'", "\""
            };
            bool found = (std::find(checkList.begin(), checkList.end(), ach) != checkList.end());
            bool foundSolo = false;
            for (const auto &check : checkSolo) {
                if (ach.contains(check)) {
                    foundSolo = true;
                    break;
                }
            }
            if ((ach.length() > 1 && ( ach.contains(" ") ))
                or ach.length() == 1
                or ach == ""
                or found
                or foundSolo) {

                insertPlainText(totalBrackets.c_str());
                moveCursorBack();
            }
            else {
                insertPlainText(bracket.c_str());
            }
        }
        else {
            insertPlainText(bracket.c_str());
        }
    }
    else { // if chars == () or [] or {} - move cursor out of brackets or input total brackets
        if (!isNew) {
            moveCursorForward();
        }
        else {
            insertPlainText(totalBrackets.c_str());
            moveCursorBack();
        }
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

    int scrollPos = this->verticalScrollBar()->sliderPosition();
    QRect r1    = this->viewport()->geometry();
    int blockNum = this->cursorForPosition(QPoint(0, 0)).blockNumber();
    int viewX   = r1.x();
    int viewY   = r1.y();

    if (blockNum == 0) {
        return 0;
    }

    return blockNum + 1;

    /*for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    viewX, viewY - scrollPos).toRect();

        //int id = this->visibleRegion().begin();

        //QRect r1 = this->viewport()->geometry();
        //QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
        //            this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
        //                this->verticalScrollBar()->sliderPosition()
        //                ) ).toRect();

        if (r1.contains(r2, true)) { return i; }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;*/
}

void Textbox::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), *new QColor(root->theme["linenumBackground"].c_str()));//*new QColor("#404244")); //
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

    QColor col_1(root->theme["linenumCurrentLineColor"].c_str());//(90, 255, 30);      // Current line (custom green)
    QColor col_0(root->theme["linenumFontColor"].c_str());//(Qt::gray);         // // Other lines  (custom darkgrey)

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

QString Textbox::getSelectionText() {
    QTextCursor cur = textCursor();
    return cur.selectedText();
}

void Textbox::moveToSelectionStart() {
    QTextCursor cur = textCursor();
    int start       = cur.selectionStart();
    cur.setPosition(start);
    setTextCursor(cur);
}

void Textbox::clearSelection() {
    QTextCursor cur = textCursor();
    cur.clearSelection();
    setTextCursor(cur);
}

void Textbox::updateWidgetStyle() {
    QString style = QString("QTextEdit { color: %1; border: none; background: %2; }")
                    .arg(root->theme["textboxFontColor"].c_str(),
                         root->theme["textboxBackground"].c_str());
    setStyleSheet(style);
    //setStyleSheet("QTextEdit { color: lightGray; border: none; background: #1f222d; }");

    QFont fnt(root->cfg->textboxFontFamily.c_str());
    fnt.setPixelSize(root->cfg->textboxFontSize);
    setFont(fnt);
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
