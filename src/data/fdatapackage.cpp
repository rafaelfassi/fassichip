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
