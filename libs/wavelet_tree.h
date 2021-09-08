#include <bits/stdc++.h>
#include "../libs/compressed_bitvector.h"

using namespace std;

#ifndef WAVELET_TREE
#define WAVELET_TREE

pair<uint, uint> findLimits(uint *seq, uint seq_size);

class WaveletTree {
public:
    static const uint BITVECTOR_BLOCK_SIZE = 4;
    
    WaveletTree *l = NULL, *r = NULL;
    CompressedBitvector *bitvector = NULL;
    uint lo, hi;
    uint bitvector_size = 0;
    WaveletTree(uint *from, uint *to, uint lo, uint hi);
    ~WaveletTree();
    uint access(uint i, uint lo, uint hi);
    uint rank(uint c, uint i, uint lo, uint hi);
    uint select(uint c, uint i, uint lo, uint hi);
    void printBitvector();
    ulong size();
};

class WaveletTreeInterface {
public:
    uint lo, hi;
    WaveletTree *waveletTree;

    WaveletTreeInterface(uint *from, uint *to);
    uint access(uint i);
    uint rank(uint c, uint i);
    uint select(uint c, uint i);
};

#endif