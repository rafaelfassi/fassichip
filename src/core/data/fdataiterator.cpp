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

#include "fdataiterator.h"
#include "misc.h"

FDataIterator::FDataIterator(FData *_Data, QObject *parent) :
    QObject(parent)
{
    Data = _Data;

    DataMode.Mode = BITS8;
    DataMode.Signal = false;
    DataMode.Swap = false;
    FileType = EDIT;

}

double FDataIterator::ReadValue(int _Pos)
{
    return Data->GetValueForType(FileType, _Pos, DataMode);
}

void FDataIterator::SetModeBits(QString _ModeBits)
{
    if(_ModeBits == "8Bits") DataMode.Mode = BITS8;
    else if(_ModeBits == "16Bits") DataMode.Mode = BITS16;
    else if(_ModeBits == "32Bits") DataMode.Mode = BITS32;
    else if(_ModeBits == "Float") DataMode.Mode = BITSF;
    //else ERRO
}

QString FDataIterator::GetModeBits()
{
    switch(DataMode.Mode)
    {
        case BITS8: return "8Bits";
        case BITS16: return "16Bits";
        case BITS32: return "32Bits";
        case BITSF: return "Float";
        default: return ""; //ERRO
    }
}

QString FDataIterator::ToHex(double _Value, int _Dig)
{
    return DoubleToHexS(_Value, _Dig);
}

double FDataIterator::BitXor(double _Value1, double _Value2)
{
  return ((qint64)_Value1 ^ (qint64)_Value2);
}

double FDataIterator::BitAnd(double _Value1, double _Value2)
{
    return ((qint64)_Value1 & (qint64)_Value2);
}

double FDataIterator::GetCRC32(int _StartPos, double _EndPos)
{
    CRC32 crc32;
    int id = Data->GetId(FileType);
    unsigned char *buffer = reinterpret_cast<unsigned char *>(Data->AddressBuffer(id));
    crc32.update(&buffer[_StartPos], _EndPos - _StartPos + 1);
    return crc32.result();
}
