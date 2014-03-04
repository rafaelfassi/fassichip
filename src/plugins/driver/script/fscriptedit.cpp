/****************************************************************************
**
**Copyright (C) 2010-2014 - Rafael Fassi Lobão
**Contact: www.fassi.com.br - rafael@fassi.com.br
**
**This file is part of the Fassichip.
**Fassichip is free software; you can redistribute it and/or modify
**it under the terms of the GNU General Public License as published by
**the Free Software Foundation; either version 2 of the License, or
**(at your option) any later version.
**
**This program is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**GNU General Public License for more details.
**
****************************************************************************/

#include "fscriptedit.h"

FScriptEdit::FScriptEdit(QWidget *pard)
        : QsciScintilla(pard)
{
    initSettings();
    lexer = new QsciLexerJavaScript(this);
    this->setLexer(lexer);

    apis = new QsciAPIs(lexer);
    apis->add("Data.ReadValue");
    apis->add("Data.WriteValue");
    apis->add("MsgBox.text");
    apis->add("MsgBox.exec");
    //apis->add("View?1");

    apis->load(QApplication::applicationDirPath() + "/script.api");
    //qDebug() << QApplication::applicationDirPath() + "/apis/" + styleSyntax + ".api";
    apis->prepare();
    lexer->setAPIs(apis);
    this->setMarginsFont(defaultFont);
    lexer->setFont(defaultFont);

    this->setMatchedBraceForegroundColor(QColor("#0000ff"));
    this->setMatchedBraceBackgroundColor(QColor("#ffff55"));
    this->setUnmatchedBraceForegroundColor(QColor("#ff0000"));
    this->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    this->setFont(defaultFont);
}

FScriptEdit::~FScriptEdit() {

}

void FScriptEdit::initSettings() {

    this->setCaretLineBackgroundColor(QColor("#e4e4e4"));
    this->setCaretLineVisible(true);
    //lexer = new QsciLexerCPP(this);
    //this->setLexer(cpp);
    //this->markerAdd(0, 0);

    this->setFolding(QsciScintilla::BoxedTreeFoldStyle);

    //this->setMarginType(1, QsciScintilla::NumberMargin);
    //this->setMarginMarkerMask(10, 10);
    //this->setMarginsBackgroundColor(QColor("#e4e4e4"));

    //this->setMarginLineNumbers(0, true);
    //this->setMarginWidth(0, QString("---"));
    //this->setMarginWidth(0, 15);
    //this->setMarginSensitivity(1, true);

    this->setMarginLineNumbers(0, false);
    this->setMarginLineNumbers(1, true);
    this->setMarginSensitivity(0, true);
    this->setMarginWidth(0, 20);
    this->setMarginWidth(2, 12);
    
    //this->setProperty("marker","2");
    //this->setMarginMarkerMask(1, 1);
    this->markerDefine(QsciScintilla::RightTriangle);
    this->setMarkerBackgroundColor(QColor("#00ff00"));

    this->setIndentationGuides(true);
    this->setTabIndents(true);
    this->setBackspaceUnindents(false);
    this->setAutoIndent(true);
    this->setIndentationsUseTabs(false);
    this->zoomTo(0);
    this->setUtf8(true);
    this->selectAll(true);

    this->setAutoCompletionSource(QsciScintilla::AcsAll);
    //this->setAutoCompletionThreshold(3);
    this->setAutoCompletionCaseSensitivity(true);
    this->setAutoCompletionShowSingle(true);
    this->setAutoCompletionFillupsEnabled(true);

    #ifdef Q_WS_WIN
    defaultFont.setFamily("Courier New");
    defaultFont.setPointSize(10);
    defaultFont.setWeight(0);
    #endif

    #ifdef Q_WS_X11
    defaultFont.setFamily("Monospace");
    defaultFont.setPointSize(10);
    #endif
    this->setMarginsFont(defaultFont);
    this->setFont(defaultFont);
}


void FScriptEdit::keyPressEvent(QKeyEvent *event)
{
    //QMessageBox ms;
    //ms.setText(tr("%1").arg(event->key()));
    //ms.exec(); //47 d68
    //qDebug() << event->key() << Qt::Key_Enter;
    //if(event->key() == Qt::Key_Backtab) event->ignore();

    // Shift+Tab muda o foco do onjeto no Linux
    #if defined(Q_OS_LINUX)
        if(event->key() == Qt::Key_Backtab) unindentHandler();
    #endif

    switch (event->key()) {
        case Qt::Key_Space:
            if (event->modifiers() & Qt::ControlModifier)
            {
                this->autoCompleteFromAPIs();
            }
            else
            {
                QsciScintilla::keyPressEvent(event);
            }
        break;
        case Qt::Key_Return:
            if (event->modifiers() & Qt::ControlModifier) {
                this->autoCompleteFromDocument();
            } else {
                QsciScintilla::keyPressEvent(event);
            }
        break;
        default:
            QsciScintilla::keyPressEvent(event);
    }
}

void FScriptEdit::unindentHandler()
{
    int line1(-1), line2(-1), col1(-1), col2(-1);
    if( hasSelectedText() )
    {
            getSelection(&line1, &col1, &line2, &col2);
            if ( col2 == 0 ) --line2;

            if (line1 <= line2 && line1 >= 0)
            {
                    beginUndoAction();
                    for (int l = line1; l <= line2; ++l) unindent(l);
                    endUndoAction();
            }
            setSelection(line1, 0, line2 + 1, 0);
    }
    else
    {
            getCursorPosition(&line1, &col1);
            if ( line1 >= 0 )
            {
                    unindent(line1);
                    setCursorPosition(line1, col1 - tabWidth());
            }
    }

}
