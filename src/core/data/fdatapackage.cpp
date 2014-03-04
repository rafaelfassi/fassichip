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

#include "fdatapackage.h"

FDataPackage::FDataPackage(FData *_Data, QObject *parent) :
    QObject(parent), Data(0), Data3d(0), DataIterator(0)
{
    Data = _Data;
}

void FDataPackage::UseData3d()
{
    if(!Data3d) Data3d = new FDataMap3d(Data, this);
}

void FDataPackage::UseDataIterator()
{
    if(!DataIterator) DataIterator = new FDataIterator(Data, this);
}
