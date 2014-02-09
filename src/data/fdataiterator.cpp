#include "fdataiterator.h"

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
