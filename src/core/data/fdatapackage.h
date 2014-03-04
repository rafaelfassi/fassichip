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

#ifndef FDATAPACKAGE_H
#define FDATAPACKAGE_H

#include <QObject>
#include <QVector>

#include "fdata.h"
#include "fdatamap3d.h"
#include "fdataiterator.h"

class FDataPackage : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int StartAddress READ GetStartAddress WRITE SetStartAddres)
    Q_PROPERTY(int EndAddress READ GetEndAddress WRITE SetEndAddress)
    Q_PROPERTY(int SelectionX READ GetSelX WRITE SetSelX)
    Q_PROPERTY(int SelectionY READ GetSelY WRITE SetSelY)

public:
    class FDataView
    {

    public:
        FDataView()
        {
           PointerPos = 0;
           StartAddress = 0;
           EndAddress = 0;
           SelX = 0;
           SelY = 0;
        }

        int PointerPos;
        int StartAddress;
        int EndAddress;
        int SelX;
        int SelY;
    };

    FDataPackage(FData *_Data = 0, QObject *parent = 0);
    void SetData(FData *_Data) { Data = _Data; }

    //Q_INVOKABLE void Exec(QString _Command) { Commands << _Command; };

    //Q_INVOKABLE void Clear() { Commands.clear(); }

    //Q_INVOKABLE int Size() { return Commands.size(); }

    int GetStartAddress() { return DataView.StartAddress; }
    int GetEndAddress() { return DataView.EndAddress; }
    int GetSelX() { return DataView.SelX; }
    int GetSelY() { return DataView.SelY; }
    void SetStartAddres(int _StartAddress) { DataView.StartAddress = _StartAddress; }    
    void SetEndAddress(int _EndAddress) { DataView.EndAddress = _EndAddress; }
    void SetSelX(int _SelX) { DataView.SelX = _SelX; }
    void SetSelY(int _SelY) { DataView.SelY = _SelY; }
    void SetCommand(QString _Command) { Command = _Command; }

    void UseData3d();
    void UseDataIterator();

    void SetPersistent(bool _Persistent) { Persistent = _Persistent; }
    bool IsPersistent() { return Persistent; }

    //QStringList Commands;
    QString Command;
    FData *Data;
    FDataMap3d *Data3d;
    FDataIterator *DataIterator;
    FDataView DataView;
    bool Persistent;

};

#endif // FDATAPACKAGE_H
