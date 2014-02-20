#include "fmaparray.h"

FMapArray::FMapArray()
{
    SizeX = 0;
    SizeY = 0;
    Z = 0;
    X = 0;
    Y = 0;
    SizeNull = true;
}

FMapArray::~FMapArray()
{
    Clear();
}

void FMapArray::Clear()
{

    if(Z)
    {
        for(int c = 0; c < SizeX; c++) delete Z[c];
        delete [] Z;
        Z = 0;
    }

    if(X)
    {
        delete [] X;
        X = 0;
    }

    if(Y)
    {
        delete [] Y;
        Y = 0;
    }

    SizeX = 0;
    SizeY = 0;
    SizeNull = true;
}

void FMapArray::SetSize(int _SizeX, int _SizeY)
{
    if(SizeX > 0 || SizeY > 0) Clear();

    SizeX = _SizeX;
    SizeY = _SizeY;

    X = new int[SizeX];
    Y = new int[SizeY];
    Z = new int*[SizeX];
    for(int c = 0; c < SizeX; c++) Z[c] = new int[SizeY];

    if(SizeX > 1 && SizeY > 1) SizeNull = false;
}

void FMapArray::SetAddressZ(int _IdX, int _IdY, int _AddressZ)
{
    Z[_IdX][_IdY] = _AddressZ;
}

int FMapArray::GetAddressZ(int _IdX, int _IdY)
{
    return Z[_IdX][_IdY];
}
