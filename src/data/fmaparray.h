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
