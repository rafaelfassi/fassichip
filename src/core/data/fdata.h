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

#ifndef FDATA_H
#define FDATA_H

#include <QColor>
#include <QVector>
#include <QVariant>
#include <QHash>
#include <math.h>

#define N_SERIES 5

#if defined(Q_CC_MINGW) || defined(Q_CC_MSVC)
    #define IsNan  _isnan
#else
    #define IsNan  isnan
#endif

#if defined(Q_CC_MSVC)
    static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
    #define NanValue (*(const float *) __nan)
#else
    #define NanValue 0.0F/0.0F
#endif


typedef unsigned char word8;
typedef char word8s;
typedef unsigned short int word16;
typedef short int word16s;
typedef unsigned int word32;
typedef int word32s;

enum {EDIT, ORI, COMP1, COMP2, COMP3}; //Tipos de arquivo padrão
enum {BITS8,BITS16, BITS32, BITSF}; //Modo de visualização
enum {NO_UNDO, ADD_UNDO, ADD_UNDO_POINT};

class FFile{
public:
    FFile()
    {
        Buffer = 0;
        Offset = 0;
        ColorFile = Qt::black;
        FileName = "";
        Type = 0;
        SizeBuffer = 0;
    }

    char *Buffer;
    int Type;
    int SizeBuffer;
    QString FileName;
    QColor ColorFile;
    int Offset;
    QHash<QString, QVariant> Attributes;
};

class FDataMode
{
public:
    FDataMode(char _ModeBits = BITS8, bool _Signal = false, bool _Swap = false)
    {
        Mode = _ModeBits;
        Signal = _Signal;
        Swap = _Swap;
    }

    char Mode;
    bool Signal;
    bool Swap;
};


class FData
{
public:
   FData();
   ~FData();

   void NewDataFile(int _Type, int _Size);
   void DeleteDataFile(int _Type);
   bool ExistFileType(int _Type);
   int GetId(int _Type);
   char *AddressBuffer(int Id);
   double GetValue(int Id, int Pos, FDataMode _DataMode, bool *_Ok = 0);
   double GetValueForType(int Type, int Pos, FDataMode _DataMode, bool *_Ok = 0);
   bool SetValue(int Pos, double _Value, FDataMode _DataMode, int _UndoFlags = ADD_UNDO | ADD_UNDO_POINT);
   int GetPosByte(int PosRelative, FDataMode _DataMode);
   int GetPosRelative(int PosByte, FDataMode _DataMode);
   int GetSizeRelative(int Id, int _ByteOffset, FDataMode _DataMode);
   int GetSizeFile(int _Id) const { return Files[_Id].SizeBuffer; }
   double GetMaxY(FDataMode _DataMode);
   double GetMinY(FDataMode _DataMode);
   int Count() const { return Files.size(); }
   bool IsEmply() const { return Files.empty(); }
   int GetSerieMain() const { return SerieMain; }
   QColor GetSerieColor(int _Id) const { return Files[_Id].ColorFile; }
   void SetColorFile(int _Id, QColor _Color) { Files[_Id].ColorFile = _Color; }
   QString GetFileName(int _Id) const { return Files[_Id].FileName; }
   void SetFileName(int _Id, QString _FileName) { Files[_Id].FileName = _FileName; }
   void SetOffsetFile(int _Id, int _Offset) { Files[_Id].Offset = _Offset; }
   int GetOffsetFile(int _Id) { return Files[_Id].Offset; }
   bool ContainsAttribute(int _Id, QString _AttributeName) { return Files[_Id].Attributes.contains(_AttributeName); }
   QVariant GetAttribute(int _Id, QString _AttributeName) { return Files[_Id].Attributes[_AttributeName]; }
   void SetAttribute(int _Id, QString _AttributeName, QVariant _Value) { Files[_Id].Attributes[_AttributeName] = _Value; }
   int GetMaxFileType() const { return MaxFileType; }


   void Undo();
   void Redo();
   bool UndoIsEmply() { return UndoLevel.isEmpty(); }
   bool RedoIsEmply() { return RedoLevel.isEmpty(); }
   void AddUndoPoint();
   void AddUndoValue(int _Pos, FDataMode _DataMode);
   void UndoClear();
   void RedoClear();

private:
   QVector<FFile> Files;
   void DefSerieMain();
   void UpdateMaxFileType();
   int SerieMain;       // Arquivo principal onde o scroll é baseado
   int MaxFileType;     // Quantidade de tipos de arquivo existentes (ORI, EDIT, COMP1, etc...)

   struct FUndoData {int Pos; double Value; FDataMode DataMode; };
   QVector<FUndoData> UndoData;
   QVector<int> UndoLevel;
   QVector<FUndoData> RedoData;
   QVector<int> RedoLevel;
   bool CanClearRedo;
};

#endif

