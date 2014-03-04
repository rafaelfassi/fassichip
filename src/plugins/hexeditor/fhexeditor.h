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

#ifndef FHEXEDITOR_H
#define FHEXEDITOR_H

#include <QtWidgets>

#include "fwindowtab.h"
#include "fdatapackage.h"
#include "fhexview.h"

class FHexEditor : public FWindowTab
{
    Q_OBJECT

public:
    FHexEditor(QWidget *parent = 0);

    void SetData(FDataPackage *_DataPackage);
    FDataPackage* GetData() { return DataPackage; }
    void RefreshData();

    FHexView *HexView;

private slots:
    void on_HexView_PointerChange(int _Pointer);
    void on_TreeWidDecode_ItemClicked(QTreeWidgetItem*,int);

private:
    void CreateForm();
    void CreateConnections();
    FDataPackage *DataPackage;
    FData *Data;
    FDataMode DataMode;

    QTreeWidget *TreeWidDecode;
    QTreeWidgetItem *ValBinary8b;
    QTreeWidgetItem *ValOctanal8b;
    QTreeWidgetItem *ValHex8b;
    QTreeWidgetItem *ValHex16b;
    QTreeWidgetItem *ValHex32b;
    QTreeWidgetItem *ValDec8b;
    QTreeWidgetItem *ValDec8bS;
    QTreeWidgetItem *ValDec16b;
    QTreeWidgetItem *ValDec16bS;
    QTreeWidgetItem *ValDec32b;
    QTreeWidgetItem *ValDec32bS;
    QTreeWidgetItem *ValFP32b;
    QTreeWidgetItem *ValChar8b;
    QTreeWidgetItem *ValUtf8;



};

#endif // FHEXEDITOR_H
