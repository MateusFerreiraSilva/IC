#include <bits/stdc++.h>
#include "../libs/wavelet_tree.h"
using namespace std;

uint get_mid(uint lo, uint hi)
{
    return lo + (hi - lo) / 2;
}

WaveletTree::WaveletTree(uint *from, uint *to, uint lo, uint hi)
{
    if (lo >= hi) return;

    uint seqSize = to - from;

    this->bitvector_size = seqSize;
    // TODO substituir por bitset                                
    vector<bool> dummy_bitvector(bitvector_size, 0); // dummy bitvector
    unsigned mid = get_mid(lo, hi);
    // TODO substituir por funcao estatica
    auto lessThanMid = [mid](unsigned x)
    { return x <= mid; };

    int idx = 0;
    for (auto it = from; it != to; it++)
        dummy_bitvector[idx++] = !lessThanMid(*it);

    this->bitvector = new CompressedBitvector(
        WaveletTree::BITVECTOR_BLOCK_SIZE,
        ceil(seqSize / (float) WaveletTree::BITVECTOR_BLOCK_SIZE),
        dummy_bitvector
    ); // the real bitvector

    auto pivot = stable_partition(from, to, lessThanMid);

    l = new WaveletTree(from, pivot, lo, mid);
    r = new WaveletTree(pivot, to, mid + 1, hi);
}

WaveletTree::~WaveletTree()
{
    if (l)
        l->~WaveletTree();
    if (r)
        r->~WaveletTree();
}

uint WaveletTree::access(uint i, uint lo, uint hi)
{
    if (i > bitvector_size || i <= 0)
        return 0;

    WaveletTree *wt = this;
    CompressedBitvector *bitvector;
    uint mid;
    bool bit;
    while (lo < hi)
    {
        bitvector = wt->bitvector;
        mid = get_mid(lo, hi);
        bit = bitvector->access(i);
        if(bit == 0)
        {
            wt = wt->l;
            i = bitvector->rank0(i);
            hi = mid;
        } else {
            wt = wt->r;
            i = bitvector->rank1(i);
            lo = mid + 1;
        }
    }

    return lo;
}

/*
    This function is recursive because c != lo we have to know the rank(c, i - 1)
    otherwise the rank will be 0 for every query wich the seq[i] != c
*/
uint WaveletTree::rank(uint c, uint i, uint lo, uint hi)
{
    if (i <= 0 || i > bitvector_size)
        return 0;

    WaveletTree *wt = this;
    CompressedBitvector *bitvector;
    uint mid;
    uint iAux = i, loAux = lo, hiAux = hi;
    while (lo < hi)
    {
        bitvector = wt->bitvector;
        mid = get_mid(lo, hi);
        if (c <= mid)
        {
            wt = wt->l;
            i = bitvector->rank0(i);
            hi = mid;
        }
        else
        {
            wt = wt->r;
            i = bitvector->rank1(i);
            lo = mid + 1;
        }
    }

    return c == lo ? i : rank(c, iAux - 1, loAux, hiAux);
}

/*
    Desça até o final da arvore depois retorne o valores do selects encadeados até o topo
*/
uint WaveletTree::select(uint c, uint i, uint lo, uint hi)
{
    if(lo == hi) return i;

    uint mid = get_mid(lo, hi);
    if(c <= mid) {
        return this->bitvector->select0(
            this->l->select(c, i, lo, mid)
        );
    } else {
        return this->bitvector->select1(
            this->l->select(c, i, mid + 1, hi)
        );
    }

}

// ################## Interface ##################

WaveletTreeInterface::WaveletTreeInterface(uint *from, uint *to) {
    uint seqSize = to - from;
    if(seqSize > 0) {
        uint lo, hi;
        lo = hi = from[0];
        for (int i = 1; i < seqSize; i++)
        {
            lo = min(lo, from[i]);
            hi = max(hi, from[i]);
        }
        this->lo = lo;
        this->hi = hi;
        this->waveletTree = new WaveletTree(from, to, lo, hi);
    }
}

uint WaveletTreeInterface::access(uint i) {
    return this->waveletTree->access(i, this->lo, this->hi);
}

uint WaveletTreeInterface::rank(uint c, uint i)
{
    return this->waveletTree->rank(c, i, this->lo, this->hi);
}

uint WaveletTreeInterface::select(uint c, uint i)
{
    return this->waveletTree->select(c, i, this->lo, this->hi);
}