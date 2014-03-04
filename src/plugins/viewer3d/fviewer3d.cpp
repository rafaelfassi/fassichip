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

#include "fviewer3d.h"

FViewer3d::FViewer3d(QWidget *parent)
    : FWindowTab(parent)
{

    Chart3d = new FChart3d;
    Chart3d->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    TableChart3d = new QTableWidget(this);
    TableChart3d->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    TableChart3d->setSelectionMode(QAbstractItemView::SingleSelection);

    CreateActions();
    CreateToolBar();

    QSplitter *Splitter = new QSplitter;
    Splitter->setOrientation(Qt::Horizontal);
    Splitter->setContentsMargins(0,0,0,0);
    Splitter->addWidget(Chart3d);
    Splitter->addWidget(TableChart3d);

    QVBoxLayout *vertLayout = new QVBoxLayout;
    vertLayout->setContentsMargins(0,0,0,0);
    vertLayout->setSpacing(0);
    vertLayout->addWidget(ToolBar);
    vertLayout->addWidget(Splitter);
    this->setLayout(vertLayout);

    CreateConnections();
}

void FViewer3d::SetData(FDataPackage *_DataPackage)
{
   DataPackage = _DataPackage;
   DataMap3d = _DataPackage->Data3d;
   Chart3d->SetData(DataMap3d);
}

void FViewer3d::on_Refresh_Triggered()
{
    //DataMap3d->ReadMap(0xAE0, 16, 10);
    //DataMap3d->ModeDataValZ = FDataMode(BITS8, false, false);
    //DataMap3d->CreateDefaultScale();

    Chart3d->Update();

    TableChart3d->clear();

    int NCol = DataMap3d->AddressZ.GetSizeX();
    int NRow = DataMap3d->AddressZ.GetSizeY();
    //int SelIni = 0;

    TableChart3d->setColumnCount(NCol);
    TableChart3d->setRowCount(NRow);

    //int Pos = 0;
    double z = 0;
    //int xAddr = DataMap3d->Data->GetPosByte(1, DataMap3d->ModeDataValZ);

    QStringList HorHeader;
    QStringList VerHeader;

    for(int x = 0; x < NCol; x++)
    {
        for(int y = 0; y < NRow ; y++)
        {
            //Pos = SelIni + x*xAddr + y*NCol*xAddr;
            //z = DataMap3d->Data->GetValue(DataMap3d->Data->GetId(1), Pos, DataMap3d->ModeDataValZ);
            z = DataMap3d->Data->GetValue(DataMap3d->Data->GetSerieMain(),
                                          DataMap3d->AddressZ.GetAddressZ(x, y),
                                          DataMap3d->ModeDataValZ);

            TableChart3d->setItem(y, x, new QTableWidgetItem(QString::number(z)));
            TableChart3d->item(y,x)->setBackgroundColor(Chart3d->GetColorData(z));        

            TableChart3d->resizeRowToContents(y);
            VerHeader.append("Linha " + QString::number(y));
        }

        TableChart3d->resizeColumnToContents(x);
        HorHeader.append("Coluna " + QString::number(x));
    }

    TableChart3d->setHorizontalHeaderLabels(HorHeader);
    TableChart3d->setVerticalHeaderLabels(VerHeader);

    //TableChart3d->item(1,1)->setBackgroundColor(Chart3d->GetColorData(80));

   QPalette p = TesteLineEdit.palette();
   p.setColor(QPalette::Normal, QPalette::Base, Chart3d->GetColorData(80));
   TesteLineEdit.setPalette(p);
}

void FViewer3d::on_TableChart3d_ItemSelectionChanged(int y0, int x0, int y1, int x1)
{
    Chart3d->SetPointer(x0, y0);
}

void FViewer3d::CreateActions()
{
    ActRefresh = new QAction(QIcon(":/images/Cursor.png"), tr("Refresh"), this);
}

void FViewer3d::CreateToolBar()
{
    ToolBar = new QToolBar();
    ToolBar->setIconSize(QSize(16,16));
    ToolBar->addAction(ActRefresh);
    ToolBar->addWidget(&TesteLineEdit);
}

void FViewer3d::CreateConnections()
{
    connect(ActRefresh, SIGNAL(triggered()), this, SLOT(on_Refresh_Triggered()));
    connect(TableChart3d, SIGNAL(currentCellChanged(int,int,int,int)),
            this, SLOT(on_TableChart3d_ItemSelectionChanged(int,int,int,int)));
}
