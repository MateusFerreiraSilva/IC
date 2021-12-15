#include <bits/stdc++.h>
using namespace std;

#ifndef COMPRESSED_ARRAY
#define COMPRESSED_ARRAY

class Bitarray
{
private:
    const uint w = 32; // amount of bits in a uint
    uint *W;
    uint W_SIZE;
    uint n; // number of elements
    uint l; // max size of bits that a element will have
    ulong sz = 0;

public:
    // mudar parametros para aceitar int ou uint
    Bitarray(int l, int n);
    Bitarray(int l, int n, uint *arr);
    ~Bitarray();
    void compress(uint *arr);
    bool bitread(int j);
    uint bitsread(int j0, int j);
    uint read(int i);
    void bitset(int j);
    void bitclear(int j);
    void bitswrite(int j0, int j, uint x);
    void write(int i, uint x);
    void bitsPrint();
    void printW();
    ulong size();
    ulong length();
    // void printWbits()
    // {
    //     for (int i = 1; i <= l * n; i++)
    //     {
    //         printf("%d", bitread(i - 1));
    //         if (i % l == 0)
    //             printf(" ");
    //     }
    //     puts("");
    // }
};

#endif