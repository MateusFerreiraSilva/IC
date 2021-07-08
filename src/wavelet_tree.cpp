#include <bits/stdc++.h>
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"
using namespace std;

pair<int, int> findLimits(int* seq, unsigned seq_size) {
    int seqMin, seqMax;
    if(seq_size >= 1)
        seqMin = seqMax = seq[0];
    for(int i = 1; i < seq_size; i++) {
        seqMin = min(seqMin, seq[i]);
        seqMax = max(seqMax, seq[i]);
    }
    return make_pair(seqMin, seqMax);
}

Node::Node() { return; }

Node::Node(int *from, int*to, int lo, int hi, Node *parent) {
    if(from >= to)
        return;

    this->p = parent;

    

    unsigned sz = to - from;
    vector<bool> b(sz);
    unsigned mid = lo + (hi - lo) / 2;
    auto lessThanMid = [mid](int x)
        { return x <= mid; };

    unsigned idx = 0;
    for(auto it = from; it != to; it++)
        b[idx++] = !lessThanMid(*it);

    if(p == NULL) {
        for (int i = 0; i < b.size(); i++)
        {
            cout << b[i] << " ";
        }
        cout << endl;
    }

    this->bitvector = new CompressedBitVector(4, ceil(b.size()/ (float)4), b); // the real bitvector

    auto pivot = stable_partition(from, to, lessThanMid);

    if (lo == mid) // homogeneous array
        this->l = new Leaf(lo);
    else
        l = new Node(from, pivot, lo, mid, this);
    if (mid + 1 == hi) // homogeneous array
        this->r = new Leaf(hi);
    else
        r = new Node(pivot, to, mid + 1, hi, this);
}

Node::~Node() {
    if(l)
        l->~Node();
    if(r)
        r->~Node();
}

CompressedBitVector *Node::getBitvector()
{
    return this->bitvector;
}
void Node::setBitvector(CompressedBitVector *bitvector)
{
    this->bitvector = bitvector;
}
Node *Node::getL()
{
    return this->l;
}
void Node::setL(Node *l)
{
    this->l = l;
}
Node *Node::getR()
{
    return this->r;
}
void Node::setR(Node *r)
{
    this->r = r;
}
Node *Node::getP()
{
    return this->p;
}
void Node::setP(Node *p)
{
    this->p = p;
}

Leaf::Leaf(int x) {
    this->x = x;
}

WaveletTree::WaveletTree(int *seq, unsigned seq_size) {
    this->seq = seq;
    this->seq_size = seq_size;
    pair<int, int> limits = findLimits(seq, seq_size);
    this->root = new Node(seq, seq + seq_size, limits.first, limits.second, NULL);
}

WaveletTree::~WaveletTree() {
    this->root->~Node();
}

int WaveletTree::access(int i) {
    Node *v = this->root;
    pair<int, int> p = findLimits(this->seq, this->seq_size);
    int a = p.first, b = p.second;

    // while(a != b) {
    while(v->getL() != NULL || v->getR() != NULL) {
        if(v->getBitvector()->access(i) == 0) {
            i = v->getBitvector()->rank0(i);
            v = v->getL();
            b = floor((a + b) / (float)2);
        } else {
            i = v->getBitvector()->rank1(i);
            v = v->getR();
            a = floor((a + b)/ (float)2) + 1;
        }
    }
    Leaf *le = (Leaf*)v;
    return le->x;
}