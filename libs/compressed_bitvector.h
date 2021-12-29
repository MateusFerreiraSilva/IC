#include <bits/stdc++.h>
#include "../libs/bitarray.h"
#include "../libs/sample_pointers.h"
using namespace std;

#ifndef COMPRESSED_BITVECTOR
#define COMPRESSED_BITVECTOR

class CompressedBitvector
{
private:
    uint block_size;
    uint blocks_total;
    ulong bits;
    Bitarray *C;
    SamplePointers *O;
    uint **Comb; // represents variable K
    uint SUPER_BLOCK_SIZE, SUPER_BLOCK_NUM, *R;
    uint ones, zeros; // number of 1's or 0's
    ulong sz = 0;

    pair<uint, uint> encode(Bitarray &B, uint i);
    uint decode(uint i);
    void precompR();

public:
    CompressedBitvector(uint block_size, ulong length);
    CompressedBitvector(uint block_size, ulong length, uint *B);
    CompressedBitvector(uint block_size, ulong length, vector<bool> &bitvector);
    ~CompressedBitvector();
    void compress(Bitarray B);
    bool access(uint i);
    uint rank1(uint i);
    uint rank0(uint i);
    uint caclMidIdx(uint lo_idx, uint hi_idx);
    uint find_first_rank(uint idx);
    uint rank1_binary_search(uint lo_idx, uint hi_idx, uint rank);
    uint rank0_binary_search(uint lo_idx, uint hi_idx, uint rank);
    uint select1(uint i);
    uint select0(uint i);
    void print_blocks();
    void print();
    ulong size();
};

#endif