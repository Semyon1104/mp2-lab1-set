// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len <= 0) throw "len <= 0";
    BitLen = len;
    if (len % (8 * sizeof(TELEM)) == 0) {
        MemLen = len / (8 * sizeof(TELEM));
    }
    else {
        MemLen = len / (8*sizeof(TELEM)) + 1;
    }
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i<MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    if (pMem != nullptr) {
        delete[]pMem;
        pMem = nullptr;
        BitLen = 0;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen) throw "No correct index";
    return (n / (8 * sizeof(TELEM)));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n - sizeof(TELEM) * GetMemIndex(n));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= 0 && n < BitLen) {
        pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
    }
    else throw " no correct n";
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= 0 && n < BitLen)
        pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n);
    else throw " no correct n";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= 0 && n < BitLen)
        return pMem[GetMemIndex(n)] & GetMemMask(n);
    else throw " no correct n";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int flag = 1;
    if (MemLen == bf.MemLen && BitLen == bf.BitLen) {
        for (size_t i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) flag = 0;
        }
    }
    else {
        flag = 0;
    }
    return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    int flag = 1;
    if (MemLen == bf.MemLen && BitLen == bf.BitLen) {
        for (size_t i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) flag = 0;
        }
    }
    else {
        flag = 0;
    }
    if (flag) return 0;
    else return 1; 
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len = bf.BitLen;
    if (BitLen > len) len = BitLen;
    TBitField tmp(len);
    for (int i = 0; i < MemLen; i++) tmp.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++) tmp.pMem[i] = tmp.pMem[i] | bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len;
    if (BitLen > bf.BitLen) len = BitLen;
    else len = bf.BitLen;
    TBitField tmp(len);
    int minlen;
    if (bf.BitLen < BitLen) minlen = bf.BitLen;
    else minlen = BitLen;
    for (int i = 0; i < minlen; i++) {
        if (GetBit(i) & bf.GetBit(i)) tmp.SetBit(i);
    }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);
    for (int i = 0; i < tmp.BitLen; i++) {
        if (GetBit(i) == 0) tmp.SetBit(i);
        else tmp.ClrBit(i);

    }
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string tmp;
    istr >> tmp;
    if (tmp.size() != bf.GetLength()) {
        throw "no correct length";
    }
    else {
        for (int i = 0; i < bf.GetLength(); i++) {
            if (tmp[i] == '0') bf.ClrBit(i);
            if (tmp[i] == '1') bf.SetBit(i);
            else throw "no correct string";
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        if (bf.GetBit(i) == 1) cout << 1;
        else cout << 0;
    }
    return ostr;
}
