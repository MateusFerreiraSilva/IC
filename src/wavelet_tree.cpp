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

Node::Node(int *from, int*to, int lo, int hi, Node *parent) {
    if(from >= to)
        return;

    this->p = parent;

    if(lo == hi) // homogeneous array
        return;

    unsigned sz = to - from;
    vector<bool> b(sz);
    unsigned mid = lo + (hi - lo) / 2;
    auto lessThanMid = [mid](int x)
        { return x <= mid; };

    unsigned idx = 0;
    for(auto it = from; it != to; it++)
        b[idx++] = !lessThanMid(*it);

    cout << "sizes\n";
    this->bitvector = new CompressedBitVector(4, ceil(b.size()/ (float)4), b); // the real bitvector
    this->bitvector->print();

    auto pivot = stable_partition(from, to, lessThanMid);

    l = new Node(from, pivot, lo, mid, this);
    r = new Node(pivot, to, mid + 1, hi, this);
}

Node::~Node() {
    if(l)
        l->~Node();
    if(r)
        r->~Node();
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

    while(a != b) {
        if(v->bitvector->access(i) == 0) {
            i = v->bitvector->rank0(i);
            v = v->l;
            b = floor(a + b) / 2;
        } else {
            i = v->bitvector->rank1(i);
            v = v->r;
            a = floor(a + b) / 2 + 1;
        }
    }
    return a;
}