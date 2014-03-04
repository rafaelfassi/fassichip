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

#ifndef FMAPARRAY_H
#define FMAPARRAY_H

class FMapArray
{
public:
    FMapArray();
    ~FMapArray();

    void Clear();
    void SetSize(int _SizeX, int _SizeY);
    void SetAddressZ(int _IdX, int _IdY, int _AddressZ);
    int GetAddressZ(int _IdX, int _IdY);
    int GetSizeX() { return SizeX; }
    int GetSizeY() { return SizeY; }
    bool IsEmpty() { return SizeNull; }

private:
    int **Z; // Endereço de Z
    int *X;     // Indice de X
    int *Y;     // Indice de Y
    int SizeX;
    int SizeY;
    bool SizeNull;
};

#endif // FMAPARRAY_H
