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

#ifndef FSCRIPTEDIT_H
#define FSCRIPTEDIT_H

#include <QtCore>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qsciapis.h>


class FScriptEdit : public QsciScintilla
{
    Q_OBJECT

public:
    FScriptEdit(QWidget *pard = 0);
    virtual ~FScriptEdit();
private:
    void initSettings();
    void unindentHandler();
    QsciLexer *lexer;
    QFont defaultFont;
    QsciAPIs *apis;
protected:
    void keyPressEvent(QKeyEvent *event);
};
#endif // FSCRIPTEDIT_H
