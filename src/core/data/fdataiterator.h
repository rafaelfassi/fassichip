#ifndef FDATAITERATOR_H
#define FDATAITERATOR_H

#include <QObject>

#include "fdata.h"

class FDataIterator : public QObject
{
Q_OBJECT

Q_PROPERTY(int FileType READ GetFileType WRITE SetFileType)
Q_PROPERTY(QString ModeBits READ GetModeBits WRITE SetModeBits)
Q_PROPERTY(bool Signal READ GetSignal WRITE SetSignal)
Q_PROPERTY(bool Swap READ GetSwap WRITE SetSwap)

public:
    explicit FDataIterator(FData *_Data = 0, QObject *parent = 0);

    void SetData(FData *_Data) { Data = _Data; }

    Q_INVOKABLE double ReadValue(int _Pos);
    Q_INVOKABLE double To16Bits(double _Value) { return (word16)_Value; }
    Q_INVOKABLE QString ToHex(double _Value, int _Dig = 0);
    Q_INVOKABLE double BitXor(double _Value1, double _Value2);
    Q_INVOKABLE double BitAnd(double _Value1, double _Value2);
    Q_INVOKABLE double GetCRC32(int _StartPos, double _EndPos);

    void SetModeBits(QString _ModeBits);
    QString GetModeBits();
    void SetSignal(bool _Signal) { DataMode.Signal = _Signal; }
    bool GetSignal() { return DataMode.Signal; }
    void SetSwap(bool _Swap) { DataMode.Swap = _Swap; }
    bool GetSwap() { return DataMode.Swap; }


    void SetFileType(int _FileType) { FileType = _FileType; }
    int GetFileType() { return FileType; }
signals:

public slots:

private:
    FData *Data;
    FDataMode DataMode;
    int FileType;

};

#endif // FDATAITERATOR_H
