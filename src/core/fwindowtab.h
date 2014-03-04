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

#ifndef FWINDOWTAB_H
#define FWINDOWTAB_H

#include <QWidget>

//class FData;
//class FDataMap3d;
class FDataPackage;

//enum {NO_DATA_TYPE, FDATA_TYPE, FDATAMAP3D_TYPE};

class FWindowTab : public QWidget
{
    Q_OBJECT

public:
    explicit FWindowTab(QWidget *parent = 0);
    virtual ~FWindowTab() {}

    void SetTabLabel(QString _TabLabel) { TabLabel = _TabLabel; }
    QString GetTabLabel() { return TabLabel; }

    //virtual int GetDataType()=0;
    virtual void RefreshData() {}
    virtual void UpdateData() {}
    virtual void UpdateSettings() {}
    virtual void SetData(FDataPackage *_DataPackage) { Q_UNUSED(_DataPackage) }
    virtual FDataPackage* GetData(){ return 0; }

signals:

public slots:

private:
    QString TabLabel;

};

#endif // FWINDOWTAB_H
