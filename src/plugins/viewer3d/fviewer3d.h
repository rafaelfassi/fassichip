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

#ifndef FVIEWER3D_H
#define FVIEWER3D_H

#include <QtGui>

#include "fwindowtab.h"
#include "fdatapackage.h"
#include "fchart3d.h"

class FViewer3d : public FWindowTab
{  
    Q_OBJECT

public:
    FViewer3d(QWidget *parent = 0);

    void SetData(FDataPackage *_DataPackage);
    FDataPackage* GetData() { return DataPackage; }
    FChart3d *Chart3d;
    QTableWidget *TableChart3d;

public slots:
    void on_Refresh_Triggered();
    void on_TableChart3d_ItemSelectionChanged(int y0, int x0, int y1, int x1);

private:
    void CreateActions();
    void CreateToolBar();
    void CreateConnections();
    FDataPackage *DataPackage;
    FDataMap3d *DataMap3d;

    QAction *ActRefresh;
    QToolBar *ToolBar;

    QLineEdit TesteLineEdit;
};


#endif // FVIEWER3D_H
