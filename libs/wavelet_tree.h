#include <bits/stdc++.h>

using namespace std;

#ifndef WAVELET_TREE
#define WAVELET_TREE

class WaveletTree {
private:
public:
    WaveletTree *l = NULL, *r = NULL, *p = NULL;
    CompressedBitVector *bitvector = NULL;
    unsigned bitvector_size = 0;
    WaveletTree(unsigned *from, unsigned *to, unsigned lo, unsigned hi, string cod, WaveletTree *parent);
    ~WaveletTree();
    int access(int i);
    int rank(unsigned c, int i);
    int select(unsigned c, int i);
    void printBitvector();
};

#endif