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

#include "plughexeditor.h"


void PlugHexEditor::SetMainWindow(FMainWindow *_MainWindow)
{
    MainWindow = _MainWindow;

    ActNewHexEditor = new QAction(QIcon(":/hexeditor/images/HexEditor.png"), tr("Hexadecimal Editor"), MainWindow);
    ActNewHexEditor->setIconVisibleInMenu(true);
    connect(ActNewHexEditor, SIGNAL(triggered()), this, SLOT(on_ActNewHexEditor_triggered()));

    MainWindow->MenuEdit->addAction(ActNewHexEditor);
    MainWindow->ToolBarEdit->addAction(ActNewHexEditor);

}

void PlugHexEditor::on_ActNewHexEditor_triggered()
{
    NewHexEditor(0, "");
}

QStringList PlugHexEditor::KeyFunctions() const
{
    return QStringList() << "Show";
}

void PlugHexEditor::ExecFunction(const QString _KeyFunction, FDataPackage *_DataPackage)
{
    QString _Function;
    QString _Label;

    int _Index = _KeyFunction.indexOf(QChar(':'));

    if(_Index > 0)
    {
        _Function = _KeyFunction.mid(0, _Index);
        _Label = _KeyFunction.mid(_Index+1);
    }
    else
    {
        _Function = _KeyFunction;
        _Label = "";
    }


    if(_Function == "Show") NewHexEditor(_DataPackage, _Label);
}

void PlugHexEditor::NewHexEditor(FDataPackage *_DataPackage, QString _Label)
{
    if(!_DataPackage) _DataPackage = new FDataPackage(MainWindow->Data);

    FHexEditor *HexEditor = new FHexEditor(MainWindow);

    if(_Label == "") _Label = tr("Hexadecimal Editor");

    HexEditor->SetData(_DataPackage);
    HexEditor->SetTabLabel(_Label);
    MainWindow->AddNewTab(HexEditor);
    MainWindow->RefreshDataTabs();
}

//Q_EXPORT_PLUGIN2(plughexeditor, PlugHexEditor)
