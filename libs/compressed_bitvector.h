#include <bits/stdc++.h>
#include "../libs/compressed_array.h"
#include "../libs/sample_pointers.h"
using namespace std;

#ifndef COMPRESSED_BITVECTOR
#define COMPRESSED_BITVECTOR

#define k 4

class CompressedBitVector
{
private:
    int block_size, block_num;
    unsigned long length;
    CompArray *C;
    SamplePointers *O;
    int **K;
    int *R;
    int *P;
    CompArray *S1, *S0; // select arrays
    int m1; // number of 1's
    int m0; // number of 0's
    long unsigned sz = 0;

    pair<unsigned, unsigned> encode(CompArray &B, int i);
    unsigned decode(int i);
    void precompR();

public:
    CompressedBitVector(unsigned block_size, unsigned block_num, unsigned long length);
    CompressedBitVector(unsigned block_size, unsigned block_num, unsigned long length, unsigned *B);
    CompressedBitVector(unsigned block_size, unsigned block_num, unsigned long length, vector<bool> &bitvector);
    ~CompressedBitVector();
    void compress(CompArray B);
    bool access(unsigned i);
    unsigned rank1(unsigned i);
    unsigned rank0(unsigned i);
    unsigned select1(unsigned i);
    unsigned select0(unsigned i);
    void print();
    long unsigned size();
    long unsigned count();
};

#endif