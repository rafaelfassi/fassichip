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

#include "fdatamap3d.h"

FDataMap3d::FDataMap3d(FData *_Data, QObject *parent) :
        QObject(parent)
{
    Data = _Data;
}

void FDataMap3d::SetData(FData *_Data)
{
     Data = _Data;
}

void FDataMap3d::ReadMap(int _AddressIni, int _SizeX, int _SizeY)
{

    if(_SizeX<2 || _SizeY<2)
    {
        AddressZ.Clear();
        return;
    }

    AddressZ.SetSize(_SizeX, _SizeY);

    int xAddr = Data->GetPosByte(1, ModeDataValZ);

    for(int x=0; x<_SizeX; x++)
    {
        for(int y=0; y<_SizeY; y++)
        {
            AddressZ.SetAddressZ(x, y, _AddressIni + x*xAddr + y*_SizeX*xAddr);
        }
    }

    //ModeDataValZ = Data->GetCurrDataMode();
}

void FDataMap3d::CreateDefaultScale()
{
    int nX = AddressZ.GetSizeX();// Data->GetSelectionNPos();
    int nY = AddressZ.GetSizeY();//Data->GetSelectionMult();

    if(nX<2 || nY<2) return;

    if(!ScaleX.isEmpty()) ScaleX.clear();
    if(!ScaleY.isEmpty()) ScaleY.clear();

    for(int x=0; x<nX; x++) ScaleX.append(x);

    for(int y=0; y<nY; y++) ScaleY.append(y);
}

void FDataMap3d::SetSize(int _SizeX, int _SizeY)
{
    if(_SizeX<2 || _SizeY<2)
    {
        AddressZ.Clear();
        return;
    }

    AddressZ.SetSize(_SizeX, _SizeY);
}

void FDataMap3d::SetAddressZ(int _PosX, int _PosY, int _AddressZ)
{
    if(_PosX < AddressZ.GetSizeX() && _PosY < AddressZ.GetSizeY())
        AddressZ.SetAddressZ(_PosX, _PosY, _AddressZ);
}
