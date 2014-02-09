#ifndef FDATAMAP3D_H
#define FDATAMAP3D_H

#include <QVector>
#include <QString>
#include "fmaparray.h"
#include "fdata.h"

class FDataMap3d : public QObject
{
    Q_OBJECT

public:
    FDataMap3d(FData *_Data = 0, QObject *parent = 0);

    void SetData(FData *_Data);
    void ReadMap(int _AddressIni, int _SizeX, int _SizeY);
    Q_INVOKABLE void CreateDefaultScale();
    Q_INVOKABLE void SetSize(int _SizeX, int _SizeY);
    Q_INVOKABLE void SetAddressZ(int _PosX, int _PosY, int _AddressZ);
    QString TitleGraf;
    QString LabelX;
    QString LabelY;
    QString LabelZ;

    bool ScaleXIsAddress;
    bool ScaleYIsAddress;
    QVector<double> ScaleX;     // Pode ser o endereço ou o valor, dependendo de "ScaleXIsAddress"
    QVector<double> ScaleY;     // Pode ser o endereço ou o valor, dependendo de "ScaleYIsAddress"
    QString ConverterScaleX;    // Função para converter a escala X.
    QString ConverterScaleY;
    FDataMode ModeDataScaleX;
    FDataMode ModeDataScaleY;

    FMapArray AddressZ;
    QString ConverterValZ;
    FDataMode ModeDataValZ;
    FData *Data;

};

//Q_DECLARE_METATYPE(FDataMap3d)

#endif // FDATAMAP3D_H
