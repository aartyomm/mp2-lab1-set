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

// {47|46|...|33|32} {31|30|...|17|16} {15|14|...|1|0}    !!!
//      pMem[0]           pMem[1]           pMem[2]
//
// or
//
// {15|14|...|1|0} {31|30|...|17|16} {47|46|...|33|32} 1
//      pMem[0]         pMem[1]           pMem[2]
//
//
// n-ый бит 0<=n<=15 или 0<=n<=47
//             !            1

const int uibit = 32;
const int ui2bit = 5; // степень 2 - количество бит в элементе типа unsigned int (int)
const int uibyte = 4; // количество байт в элементе типа unsigned int (int)

TBitField::TBitField(int len)
{
    BitLen = len;
    MemLen = len >> uibyte;
    MemLen++;
    pMem = new TELEM [MemLen];
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> ui2bit;   //разделить на 2^uibit
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & (uibit - 1));    //return 1 << (n%(uibit-1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= BitLen || n < 0)
        throw out_of_range("out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen || n < 0)
        throw out_of_range("out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= BitLen || n < 0)
        throw out_of_range("out of range");
    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    TELEM *p = new TELEM[bf.MemLen];
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = p;
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return false;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return false;
    return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return FAKE_BITFIELD;
    if (BitLen != bf.BitLen)
        throw length_error("different sizes");
    TBitField tmp(*this);
    for (int i = 0; i < bf.BitLen; i++)
        tmp.pMem[i] |= bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return FAKE_BITFIELD;
    if (BitLen != bf.BitLen)
        throw length_error("different sizes");
    TBitField tmp(*this);
    for (int i = 0; i < bf.BitLen; i++)
        tmp.pMem[i] &= bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    return FAKE_BITFIELD;
    TBitField tmp(BitLen);
    for (int i = 0; i < BitLen; i++)
        tmp.pMem[i] = ~pMem[i];
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int len;
    istr >> len;
    TBitField tmp(len);
    bf = tmp;
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int s = bf.GetLength();
    for (int i = 0; i < s; i++) {
        if (bf.GetBit(i))
            ostr << '1';
        else
            ostr << '0';
    }
    return ostr;
}
