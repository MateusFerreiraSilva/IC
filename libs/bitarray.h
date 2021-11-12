#include <bits/stdc++.h>
using namespace std;

#ifndef COMPRESSED_ARRAY
#define COMPRESSED_ARRAY

class Bitarray
{
private:
    const unsigned w = 32; // amount of bits in a unsigned
    unsigned *W;
    unsigned W_SIZE;
    unsigned n; // number of elements
    unsigned l; // max size of bits that a element will have
    long unsigned sz = 0;

public:
    // mudar parametros para aceitar int ou unsigned
    Bitarray(int l, int n);
    Bitarray(int l, int n, unsigned *arr);
    ~Bitarray();
    void compress(unsigned *arr);
    bool bitread(int j);
    unsigned bitsread(int j0, int j);
    unsigned read(int i);
    void bitset(int j);
    void bitclear(int j);
    void bitswrite(int j0, int j, unsigned x);
    void write(int i, unsigned x);
    int bitsPrint();
    void printW();
    long unsigned size();
    long unsigned length();
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