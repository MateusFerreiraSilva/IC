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
    Bitarray(uint l, uint n);
    Bitarray(uint l, uint n, uint *arr);
    ~Bitarray();
    void compress(uint *arr);
    bool bitread(uint j);
    uint bitsread(uint j0, uint j);
    uint read(uint i);
    void bitset(uint j);
    void bitclear(uint j);
    void bitswrite(uint j0, uint j, uint x);
    void write(uint i, uint x);
    void bitsPrint();
    void printW();
    ulong size();
    ulong length();
};

#endif