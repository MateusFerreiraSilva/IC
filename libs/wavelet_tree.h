#include <bits/stdc++.h>

using namespace std;

#ifndef WAVELET_TREE
#define WAVELET_TREE


class Node
{
private:
public:
    CompressedBitVector *bitvector;
    Node *l, *r, *p;
    
    Node(int *from, int*to, int lo, int hi, Node *parent);
    ~Node();
};

class WaveletTree {
private:
    int *seq;
    unsigned seq_size = 0;
    Node *root;
public:
    WaveletTree(int *seq, unsigned seq_size);
    ~WaveletTree();
    int access(int i);
};

#endif