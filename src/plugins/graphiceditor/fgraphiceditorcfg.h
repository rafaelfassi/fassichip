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

#ifndef FGRAPHICEDITORCFG_H
#define FGRAPHICEDITORCFG_H

#include <QWidget>

class QCheckBox;

class FGraphicEditorCfg : public QWidget
{
    Q_OBJECT

public:
    FGraphicEditorCfg(QString _Key, QWidget *parent = 0);  
    void WriteSettings();

private:
    QString Key;
    QCheckBox *ChkViewXScale;
    QCheckBox *ChkViewYScale;

};

#endif // FGRAPHICEDITORCFG_H
