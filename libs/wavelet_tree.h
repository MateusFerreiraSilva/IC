#include <bits/stdc++.h>

using namespace std;

#ifndef WAVELET_TREE
#define WAVELET_TREE

class Bitvectorr {
    public:
        vector<bool> b;

        Bitvectorr(int n);
        int access(int i);
        int rank1(int i);
        int rank0(int i);
        int select1(int i);
        int select0(int i);
};


class Node
{
private:
    // CompressedBitVector *bitvector;
public:
    Node *l, *r, *p;
    Bitvectorr *bitvector;
    Node();
    Node(int *from, int*to, int lo, int hi, Node *parent);
    ~Node();
    CompressedBitVector* getBitvector();
    void setBitvector(CompressedBitVector* bitvector);
    Node* getL();
    void setL(Node* l);
    Node* getR();
    void setR(Node* r);
    Node* getP();
    void setP(Node* p);
};

class Leaf : public Node {
    public:
        int x;
        Leaf(int x);
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