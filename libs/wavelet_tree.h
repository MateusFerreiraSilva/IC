#include <bits/stdc++.h>

using namespace std;

#ifndef WAVELET_TREE
#define WAVELET_TREE


class Node
{
private:
    CompressedBitVector *bitvector;
    Node *l, *r, *p;
public:
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