#include "fdata.h"

FData::FData()
{
    SerieMain = 0;
    MaxFileType = 0;
    CanClearRedo = true;
}

FData::~FData()
{
    //for(int x = 0; x <= 4; x++)
           // if(ExistFileType(x)) DeleteDataFile(x);
}

void FData::NewDataFile(int _Type, int _Size)
{
    /*
      Cria um buffer para novo arquivo
    */

    if(!ExistFileType(_Type))
    {
        Files.push_back(FFile());
        Files[Files.size()-1].Type = _Type;

        int Id = GetId(_Type);
        Files[Id].Buffer = new char[_Size];
        Files[Id].SizeBuffer = _Size;
    }

    UpdateMaxFileType();
    DefSerieMain();

}

void FData::DeleteDataFile(int _Type)
{
    /*
      Delata o buffer de um arquivo
    */

    if(ExistFileType(_Type))
    {
        int Id = GetId(_Type);
        delete [] Files[Id].Buffer;
        Files.erase(Files.begin() + Id);
        if(_Type == EDIT) UndoClear();
    }

    UpdateMaxFileType();
    DefSerieMain();
}

void FData::DefSerieMain()
{
    /*
      Define a série principal onde se baseia o tamanho do arquivo de trabalho e
      outras configurações. Ele sempre será o arquivo aberto com Type menor,
      isto é, se o arquivo EDIT estiver aberto ele sempre será o principal, caso
      não esteja aberto será o ORI, e assim por diante.
    */

    for(int x = 0; x <= MaxFileType; x++)
    {
      if(!ExistFileType(x)) continue;
      SerieMain = GetId(x);
      break;
    }

}

void FData::UpdateMaxFileType()
{
    /*
      Determina qual o maior tipo de arquivo aberto.
    */

    for(int x = 0; x < Files.size(); x++)
        if(Files[x].Type > MaxFileType) MaxFileType = Files[x].Type;
}

bool FData::ExistFileType(int _Type)
{
    /*
      Verifica se um determinado tipo de arquivo existe.
    */

    for(int x=0; x < Files.size(); x++)
        if(Files[x].Type == _Type) return true;

    return false;
}

int FData::GetId(int _Type)
{
    /*
      Retorna o Id do arquivo definido por Type.
      O Id não é igual ao type, pois type se refere ao tipo de arquivo aberto,
      enquanto Id é o índice do arquivo no vetor.
    */

    for(int x=0; x < Files.size(); x++)
        if(Files[x].Type == _Type) return x;

    return -1;
}

char *FData::AddressBuffer(int Id)
{
    /*
      Retorna o endereço do buffer.
    */

    return Files[Id].Buffer;
}

double FData::GetValue(int Id, int Pos, FDataMode _DataMode, bool *_Ok)
{
    /*
      Retorna o dado solicitado usando as configurações de "_DataMode".
    */

    if(Files.size() <= Id) { if(_Ok) *_Ok = false; return NanValue; }

    union
    {
       word8 Tmp8b[4];
       word8s Tmp8bs[4];
       word16 Tmp16b[2];
       word16s Tmp16bs[2];
       word32 Tmp32b;
       word32s Tmp32bs;
       float TmpFloat;
     };

     Tmp32b = 0; //Zera todas as variáveis da união.

     Pos -= Files[Id].Offset; // Offset do arquivo. Desloca o arquivo em relação aos outros.

     const int _OffSetByte = GetPosByte(1, _DataMode);

     if( (Pos < 0) || (Pos > (GetSizeFile(Id) - _OffSetByte)) ) { if(_Ok) *_Ok = false; return NanValue; }

     if(_Ok) *_Ok = true;

       switch(_DataMode.Mode)
       {
             case BITS8:
                     Tmp8b[0] = Files[Id].Buffer[Pos];
                     if(_DataMode.Signal) return  Tmp8bs[0];
                     else return Tmp8b[0];
             case BITS16:
                     if(_DataMode.Swap) // No PC por padrão os bytes ja são invertidos.
                     {
                       Tmp8b[0] =  Files[Id].Buffer[Pos];
                       Tmp8b[1] =  Files[Id].Buffer[Pos + 1];
                     }
                     else
                     {
                       Tmp8b[1] =  Files[Id].Buffer[Pos];
                       Tmp8b[0] =  Files[Id].Buffer[Pos + 1];
                     }
                     if(_DataMode.Signal) return Tmp16bs[0];
                     else return Tmp16b[0];
             case BITS32:
                     if(_DataMode.Swap) // No PC por padrão os bytes ja são invertidos.
                     {
                       Tmp8b[0] =  Files[Id].Buffer[Pos];
                       Tmp8b[1] =  Files[Id].Buffer[Pos + 1];
                       Tmp8b[2] =  Files[Id].Buffer[Pos + 2];
                       Tmp8b[3] =  Files[Id].Buffer[Pos + 3];
                     }
                     else
                     {
                       Tmp8b[3] =  Files[Id].Buffer[Pos];
                       Tmp8b[2] =  Files[Id].Buffer[Pos + 1];
                       Tmp8b[1] =  Files[Id].Buffer[Pos + 2];
                       Tmp8b[0] =  Files[Id].Buffer[Pos + 3];
                     }
                     if(_DataMode.Signal) return Tmp32bs;
                     else return Tmp32b;
             case BITSF:
                     if(_DataMode.Swap) // No PC por padrão os bytes ja são invertidos.
                     {
                       Tmp8b[0] =  Files[Id].Buffer[Pos];
                       Tmp8b[1] =  Files[Id].Buffer[Pos + 1];
                       Tmp8b[2] =  Files[Id].Buffer[Pos + 2];
                       Tmp8b[3] =  Files[Id].Buffer[Pos + 3];
                     }
                     else
                     {
                       Tmp8b[3] =  Files[Id].Buffer[Pos];
                       Tmp8b[2] =  Files[Id].Buffer[Pos + 1];
                       Tmp8b[1] =  Files[Id].Buffer[Pos + 2];
                       Tmp8b[0] =  Files[Id].Buffer[Pos + 3];
                     }
                     return TmpFloat;
       }

    if(_Ok) *_Ok = false;
    return NanValue;

}

double FData::GetValueForType(int Type, int Pos, FDataMode _DataMode, bool *_Ok)
{
    /*
      Esta função é semelhante a GetData, porém recebe o tipo de arquivo em vez do Id.
      Para loops que fazem sucessivas solicitações de dados do arquivo, é melhor chamar GetId
      antes do loop para obter o Id e depois trabalhar com GetData, pois cada vez que esta função
      é chamada, ela vai chamar o GetId que faz um pequeno loop para procurar o Id do arquivo, reduzindo
      a eficiência da função.
    */

    int _Id = GetId(Type);

    if(_Id < 0) { if(_Ok) *_Ok = false; return NanValue; }

    return GetValue(_Id, Pos, _DataMode, _Ok);
}


bool FData::SetValue(int Pos, double _Value, FDataMode _DataMode, int _UndoFlags)
{
    const int Id = GetId(0); // Procura o Id do arquivo de edição

    if(Id < 0) return false; // O arquivo de edição não está aberto

    // Quando ocorrer uma edição o refazer deve ser rezetado, exceto quado a edição for
    // chamada pelo Undo() ou pelo próprio Redo().
    if(CanClearRedo) RedoClear();

    Pos -= Files[Id].Offset; // Offset do arquivo. Desloca o arquivo em relação aos outros.

    // OffSet que deverá ser subtraído ao final do arquivo, a depender do modo.
    // Ex. Na visualização de 16Bits de um arquivo de tamanho 7FFF (8000 - 1 = Size - 1) o endereço 7FFF não
    // pode ser selecionado, pois ultrapassaria o tamanho do arquivo.
    const int _OffSetByte = GetPosByte(1, _DataMode);

    // Verifica se a posição informada está dentro do range do arquivo
    if( (Pos < 0) || (Pos > (GetSizeFile(Id) - _OffSetByte)) ) return false;

    // Verifica se valor a ser escrito no buffer está em conformidade com o modo informado
    if( (_Value > GetMaxY(_DataMode)) || (_Value < GetMinY(_DataMode)) ) return false;

    if(_UndoFlags & ADD_UNDO_POINT) AddUndoPoint();
    if(_UndoFlags & ADD_UNDO) AddUndoValue(Pos, _DataMode);


    /*union
    {
       word8s Tmp8bs[4];
       word16s Tmp16bs[2];
       word32s Tmp32bs;
       float TmpFloat;
     };*/

    union
    {
       word8 Tmp8b[4];
       word8s Tmp8bs[4];
       word16 Tmp16b[2];
       word16s Tmp16bs[2];
       word32 Tmp32b;
       word32s Tmp32bs;
       float TmpFloat;
     };

     Tmp32bs = 0; //Zera todas as variáveis da união.


       switch(_DataMode.Mode)
       {
             case BITS8:
                     if(_DataMode.Signal) Tmp8bs[0] = _Value;
                     else Tmp8b[0] = _Value;
                     Files[Id].Buffer[Pos] = Tmp8bs[0];
                     break;
             case BITS16:
                     if(_DataMode.Signal) Tmp16bs[0] = _Value;
                     else Tmp16b[0] = _Value;
                     if(_DataMode.Swap) // No PC por padrão os bytes ja são invertidos.
                     {
                       Files[Id].Buffer[Pos] = Tmp8bs[0];
                       Files[Id].Buffer[Pos + 1] = Tmp8bs[1];
                     }
                     else
                     {
                       Files[Id].Buffer[Pos] = Tmp8bs[1];
                       Files[Id].Buffer[Pos + 1] = Tmp8bs[0];
                     }
                     break;
             case BITS32:
                     if(_DataMode.Signal) Tmp32bs = _Value;
                     else Tmp32b = _Value;
                     if(_DataMode.Swap) // No PC por padrão os bytes ja são invertidos.
                     {
                       Files[Id].Buffer[Pos] = Tmp8bs[0];
                       Files[Id].Buffer[Pos + 1] = Tmp8bs[1];
                       Files[Id].Buffer[Pos + 2] = Tmp8bs[2];
                       Files[Id].Buffer[Pos + 3] = Tmp8bs[3];
                     }
                     else
                     {
                       Files[Id].Buffer[Pos] = Tmp8bs[3];
                       Files[Id].Buffer[Pos + 1] = Tmp8bs[2];
                       Files[Id].Buffer[Pos + 2] = Tmp8bs[1];
                       Files[Id].Buffer[Pos + 3] = Tmp8bs[0];
                     }
                     break;
             case BITSF:
                     TmpFloat = _Value;
                     if(_DataMode.Swap) // No PC por padrão os bytes ja são invertidos.
                     {
                       Files[Id].Buffer[Pos] = Tmp8bs[0];
                       Files[Id].Buffer[Pos + 1] = Tmp8bs[1];
                       Files[Id].Buffer[Pos + 2] = Tmp8bs[2];
                       Files[Id].Buffer[Pos + 3] = Tmp8bs[3];
                     }
                     else
                     {
                       Files[Id].Buffer[Pos] = Tmp8bs[3];
                       Files[Id].Buffer[Pos + 1] = Tmp8bs[2];
                       Files[Id].Buffer[Pos + 2] = Tmp8bs[1];
                       Files[Id].Buffer[Pos + 3] = Tmp8bs[0];
                     }

       }

    return true;

}

int FData::GetPosByte(int PosRelative, FDataMode _DataMode)
{
    /*
      Recebe a posição relativa ao modo e retorna a posição em bytes.
      Ex: Pos. relativa 2 em 16bits se torna 4
    */

  switch(_DataMode.Mode)
  {
        case BITS8:
                return PosRelative;
        case BITS16:
                return PosRelative*2;
        default:
                return PosRelative*4;
  }
}

int FData::GetPosRelative(int PosByte, FDataMode _DataMode)
{
    /*
      Recebe a posição em bytes e retorna a posição relativa ao modo.
      Ex: Pos. em Bytes 4 se torna 2 em 16Bits.
    */

  switch(_DataMode.Mode)
  {
        case BITS8:
                return PosByte;
        case BITS16:
                return PosByte/2;
        default:
                return PosByte/4;
  }
}


int FData::GetSizeRelative(int Id, int _ByteOffset, FDataMode _DataMode)
{
    /*
      Retorna o tamanho do arquivo com relação ao modo.
      Ex: 7FFF em 8bits se torna 3FFF em 16Bits.
    */

    switch(_DataMode.Mode)
    {
          case BITS8:
                  return Files[Id].SizeBuffer;
          case BITS16:
                  return (Files[Id].SizeBuffer - _ByteOffset)/2;
          default:
                  return (Files[Id].SizeBuffer - _ByteOffset)/4;
    }

}

double FData::GetMaxY(FDataMode _DataMode)
{

   switch(_DataMode.Mode)
   {
         case BITS8:
                 return _DataMode.Signal ? 127.0 : 255.0;
         case BITS16:
                 return _DataMode.Signal ? 32767.0 : 65535.0;
         case BITS32:
                 return _DataMode.Signal ? 2147483647.0 : 4294967295.0;
         default:
                 return 3.40E38;
   }

}

double FData::GetMinY(FDataMode _DataMode)
{

   switch(_DataMode.Mode)
   {
         case BITS8:
                 return _DataMode.Signal ? -128.0 : 0.0;
         case BITS16:
                 return _DataMode.Signal ? -32768.0 : 0.0;
         case BITS32:
                 return _DataMode.Signal ? -2147483648.0 : 0.0;
         default:
                 return -3.40E38;
   }

}

void FData::Undo()
{
    if(UndoLevel.isEmpty()) return;

    RedoLevel.push_back(RedoData.size());

    CanClearRedo = false;

    int _Id = GetId(0);

     for(int x=(UndoData.size()-1); x >= UndoLevel.last(); x--)
     {
       RedoData.push_back(UndoData.last());
       RedoData.last().Value = GetValue(_Id, RedoData.last().Pos, RedoData.last().DataMode);

       SetValue(UndoData.last().Pos, UndoData.last().Value, UndoData.last().DataMode, NO_UNDO);
       UndoData.pop_back();
     }
     UndoLevel.pop_back();

    CanClearRedo = true;
}

 void FData::Redo()
 {
     if(RedoLevel.isEmpty()) return;

     UndoLevel.push_back(UndoData.size());

     CanClearRedo = false;

      for(int x=(RedoData.size()-1); x >= RedoLevel.last(); x--)
      {
        AddUndoValue(RedoData.last().Pos, RedoData.last().DataMode);

        SetValue(RedoData.last().Pos, RedoData.last().Value, RedoData.last().DataMode, NO_UNDO);
        RedoData.pop_back();
      }
      RedoLevel.pop_back();

     CanClearRedo = true;
 }

void FData::AddUndoPoint()
{
    UndoLevel.push_back(UndoData.size());
}

void FData::AddUndoValue(int _Pos, FDataMode _DataMode)
{
    FUndoData _UndoData;

    _UndoData.Pos = _Pos;
    _UndoData.Value = GetValueForType(0, _Pos, _DataMode);
    _UndoData.DataMode = _DataMode;

    UndoData.push_back(_UndoData);
}

void FData::UndoClear()
{
    UndoData.clear();
    UndoLevel.clear();
    RedoClear();
}

void FData::RedoClear()
{
    RedoData.clear();
    RedoLevel.clear();
}
