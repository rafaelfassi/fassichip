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

#include "fhexeditor.h"

FHexEditor::FHexEditor(QWidget *parent)
    : FWindowTab(parent)
{
    Data = 0;

    DataMode.Mode = BITS8;
    DataMode.Signal = false;
    DataMode.Swap = false;

    CreateForm();
    CreateConnections();

}

void FHexEditor::SetData(FDataPackage *_DataPackage)
{
    DataPackage = _DataPackage;
    Data = _DataPackage->Data;
    HexView->SetData(Data);
}

void FHexEditor::RefreshData()
{
    HexView->Update();
    on_HexView_PointerChange(HexView->GetPointer());
}

void FHexEditor::on_HexView_PointerChange(int _Pointer)
{

    if(!Data) return;
    if(Data->IsEmply())
    {
        ValBinary8b->setText(1, "-");
        ValOctanal8b->setText(1, "-");
        ValHex8b->setText(1, "-");
        ValHex16b->setText(1, "-");
        ValHex32b->setText(1, "-");
        ValDec8b->setText(1, "-");
        ValDec8bS->setText(1, "-");
        ValDec16b->setText(1, "-");
        ValDec16bS->setText(1, "-");
        ValDec32b->setText(1, "-");
        ValDec32bS->setText(1, "-");
        ValFP32b->setText(1, "-");
        ValChar8b->setText(1, "-");
        ValUtf8->setText(1, "-");
        HexView->SetPointer(0);
        return;
    }

    int _Serie = Data->GetSerieMain();

    DataMode.Mode = BITS8;
    DataMode.Signal = false;

    ValBinary8b->setText(1, QString::number((qint32)Data->GetValue(_Serie, _Pointer, DataMode), 2));

    ValOctanal8b->setText(1, QString::number((qint32)Data->GetValue(_Serie, _Pointer, DataMode), 8));

    ValHex8b->setText(1, QString::number((qint32)Data->GetValue(_Serie, _Pointer, DataMode), 16).toUpper());

    DataMode.Mode = BITS16;

    ValHex16b->setText(1, QString::number((qint32)Data->GetValue(_Serie, _Pointer, DataMode), 16).toUpper());

    DataMode.Mode = BITS32;

    ValHex32b->setText(1, QString::number((qint64)Data->GetValue(_Serie, _Pointer, DataMode), 16).toUpper());

    DataMode.Mode = BITS8;

    ValDec8b->setText(1, QString::number((qint32)Data->GetValue(_Serie, _Pointer, DataMode), 10));

    DataMode.Signal = true;

    ValDec8bS->setText(1, QString::number((qint32)Data->GetValue(_Serie, _Pointer, DataMode), 10));

    DataMode.Mode = BITS16;
    DataMode.Signal = false;

    ValDec16b->setText(1, QString::number((qint32)Data->GetValue(_Serie, _Pointer, DataMode), 10));

    DataMode.Signal = true;

    ValDec16bS->setText(1, QString::number((qint32)Data->GetValue(_Serie, _Pointer, DataMode), 10));

    DataMode.Mode = BITS32;
    DataMode.Signal = false;

    ValDec32b->setText(1, QString::number((qint64)Data->GetValue(_Serie, _Pointer, DataMode), 10));

    DataMode.Signal = true;

    ValDec32bS->setText(1, QString::number((qint64)Data->GetValue(_Serie, _Pointer, DataMode), 10));
    
    DataMode.Mode = BITSF;

    ValFP32b->setText(1, QString::number(Data->GetValue(_Serie, _Pointer, DataMode)));

    DataMode.Mode = BITS8;

    char _Char[4];
    _Char[0] = Data->GetValue(_Serie, _Pointer, DataMode);
    _Char[1] = Data->GetValue(_Serie, _Pointer+1, DataMode);
    _Char[2] = Data->GetValue(_Serie, _Pointer+2, DataMode);
    _Char[3] = Data->GetValue(_Serie, _Pointer+3, DataMode);

    ValChar8b->setText(1, QString::fromLatin1(_Char, 1));

    ValUtf8->setText(1, QString( QString::fromUtf8(_Char, 4)[0] ));

}

void FHexEditor::on_TreeWidDecode_ItemClicked(QTreeWidgetItem* _Item,int)
{
    int _Ini = HexView->GetPointer();
    int _Size;

    if(_Item == ValUtf8) _Size = QByteArray( QString(ValUtf8->text(1)[0]).toUtf8() ).size();
    else _Size = _Item->data(1, Qt::AccessibleTextRole).toInt();

    HexView->SetSelection(_Ini, _Ini + _Size);
}

void FHexEditor::CreateForm()
{
    HexView = new FHexView(this);

    TreeWidDecode = new QTreeWidget(this);
    TreeWidDecode->setColumnCount(2);
    TreeWidDecode->setIndentation(1);
    TreeWidDecode->setAlternatingRowColors(true);
    TreeWidDecode->setHeaderLabels(QStringList() << tr("Type") << tr("Value"));

    ValBinary8b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Binary 8-bit") << "-");
    ValBinary8b->setTextAlignment(0, Qt::AlignRight);
    ValBinary8b->setData(1, Qt::AccessibleTextRole, 1);

    ValOctanal8b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Octal 8-bit") << "-");
    ValOctanal8b->setTextAlignment(0, Qt::AlignRight);
    ValOctanal8b->setData(1, Qt::AccessibleTextRole, 1);

    ValHex8b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Hexadecimal 8-bit") << "-");
    ValHex8b->setTextAlignment(0, Qt::AlignRight);
    ValHex8b->setData(1, Qt::AccessibleTextRole, 1);

    ValHex16b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Hexadecimal 16-bit") << "-");
    ValHex16b->setTextAlignment(0, Qt::AlignRight);
    ValHex16b->setData(1, Qt::AccessibleTextRole, 2);

    ValHex32b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Hexadecimal 32-bit") << "-");
    ValHex32b->setTextAlignment(0, Qt::AlignRight);
    ValHex32b->setData(1, Qt::AccessibleTextRole, 4);

    ValDec8b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Unsigned 8-bit") << "-");
    ValDec8b->setTextAlignment(0, Qt::AlignRight);
    ValDec8b->setData(1, Qt::AccessibleTextRole, 1);

    ValDec8bS = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Signed 8-bit") << "-");
    ValDec8bS->setTextAlignment(0, Qt::AlignRight);
    ValDec8bS->setData(1, Qt::AccessibleTextRole, 1);

    ValDec16b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Unsigned 16-bit") << "-");
    ValDec16b->setTextAlignment(0, Qt::AlignRight);
    ValDec16b->setData(1, Qt::AccessibleTextRole, 2);

    ValDec16bS = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Signed 16-bit") << "-");
    ValDec16bS->setTextAlignment(0, Qt::AlignRight);
    ValDec16bS->setData(1, Qt::AccessibleTextRole, 2);

    ValDec32b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Unsigned 32-bit") << "-");
    ValDec32b->setTextAlignment(0, Qt::AlignRight);
    ValDec32b->setData(1, Qt::AccessibleTextRole, 4);

    ValDec32bS = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Signed 32-bit") << "-");
    ValDec32bS->setTextAlignment(0, Qt::AlignRight);
    ValDec32bS->setData(1, Qt::AccessibleTextRole, 4);

    ValFP32b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("Float 32-bit") << "-");
    ValFP32b->setTextAlignment(0, Qt::AlignRight);
    ValFP32b->setData(1, Qt::AccessibleTextRole, 4);

    ValChar8b = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("ISO-8859-1") << "-");
    ValChar8b->setTextAlignment(0, Qt::AlignRight);
    ValChar8b->setData(1, Qt::AccessibleTextRole, 1);

    ValUtf8 = new QTreeWidgetItem(TreeWidDecode, QStringList() << tr("UTF-8") << "-");
    ValUtf8->setTextAlignment(0, Qt::AlignRight);
    ValUtf8->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    TreeWidDecode->resizeColumnToContents(0);

    QHBoxLayout *HorLayout1 = new QHBoxLayout;
    HorLayout1->setContentsMargins(0,0,0,0);
    HorLayout1->setSpacing(0);
    HorLayout1->addWidget(HexView);
    HorLayout1->addWidget(TreeWidDecode);
    this->setLayout(HorLayout1);
}

void FHexEditor::CreateConnections()
{
    connect(HexView, SIGNAL(PointerChange(int)), this, SLOT(on_HexView_PointerChange(int)));

    connect(TreeWidDecode, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(on_TreeWidDecode_ItemClicked(QTreeWidgetItem*,int)));
}
