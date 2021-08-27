#include <bits/stdc++.h>

using namespace std;

#ifndef WAVELET_TREE
#define WAVELET_TREE

pair<unsigned, unsigned> findLimits(unsigned *seq, unsigned seq_size);

class WaveletTree {
public:
    static map<string, unsigned> cods;
    static map<unsigned, string> codsInverted;
    static map<unsigned, WaveletTree*> leaf;
    static const unsigned BITVECTOR_BLOCK_SIZE = 4;
    
    WaveletTree *l = NULL, *r = NULL, *p = NULL;
    CompressedBitvector *bitvector = NULL;
    unsigned bitvector_size = 0;
    WaveletTree(unsigned *from, unsigned *to, string cod, WaveletTree *parent);
    ~WaveletTree();
    int access(int i);
    int rank(unsigned c, int i);
    int select(unsigned c, int i);
    void printBitvector();
    unsigned long size();
};

#endif