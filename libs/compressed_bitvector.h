#include <bits/stdc++.h>
#include "../libs/bitarray.h"
#include "../libs/sample_pointers.h"
using namespace std;

#ifndef COMPRESSED_BITVECTOR
#define COMPRESSED_BITVECTOR

#define k 4

class CompressedBitvector
{
private:
    unsigned block_size, block_num;
    unsigned long length;
    Bitarray *C;
    SamplePointers *O;
    unsigned **Comb; // represents variable K
    unsigned SUPER_BLOCK_SIZE, SUPER_BLOCK_NUM, *R, *P;
    unsigned ones, zeros; // number of 1's or 0's
    long unsigned sz = 0;

    pair<unsigned, unsigned> encode(Bitarray &B, int i);
    unsigned decode(int i);
    void precompR();

public:
    CompressedBitvector(unsigned block_size, unsigned long length);
    CompressedBitvector(unsigned block_size, unsigned long length, unsigned *B);
    CompressedBitvector(unsigned block_size, unsigned long length, vector<bool> &bitvector);
    ~CompressedBitvector();
    void compress(Bitarray B);
    bool access(unsigned i);
    unsigned rank1(unsigned i);
    unsigned rank0(unsigned i);
    unsigned caclMidIdx(unsigned lo_idx, unsigned hi_idx);
    unsigned rank_binary_search(unsigned lo_idx, unsigned hi_idx, unsigned rank);
    unsigned select1(unsigned i);
    unsigned select0(unsigned i);
    void print();
    long unsigned size();
    long unsigned count();
};

#endif