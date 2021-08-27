#include <bits/stdc++.h>
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"
using namespace std;

map<string, unsigned>WaveletTree::cods;
map<unsigned, string>WaveletTree::codsInverted;
map<unsigned, WaveletTree *>WaveletTree::leaf;

pair<unsigned, unsigned> findLimits(unsigned *seq, unsigned seq_size)
{
    unsigned seqMin, seqMax;
    if (seq_size >= 1)
        seqMin = seqMax = seq[0];
    for (int i = 1; i < seq_size; i++)
    {
        seqMin = min(seqMin, seq[i]);
        seqMax = max(seqMax, seq[i]);
    }
    return make_pair(seqMin, seqMax);
}

WaveletTree::WaveletTree(unsigned *from, unsigned *to, string cod, WaveletTree *parent)
{
    if (from > to) return;

    this->p = parent; // not using

    unsigned lo, hi, arr_size = to - from;;
    auto limits = findLimits(from, arr_size);
    lo = limits.first;
    hi = limits.second;

    if (lo == hi)
    { // homogeneous array
        WaveletTree::cods[cod] = lo;
        WaveletTree::codsInverted[lo] = cod;
        WaveletTree::leaf[lo] = this;
        return;
    }

    this->bitvector_size = arr_size;
    // TODO substituir por bitset
    vector<bool> dummy_bitvector(bitvector_size); // dummy bitvector
    unsigned mid = lo + (hi - lo) / 2;
    // TODO substituir por funcao estatica
    auto lessThanMid = [mid](unsigned x)
    { return x <= mid; };

    int idx = 0;
    for (auto it = from; it != to; it++) dummy_bitvector[idx++] = !lessThanMid(*it);
    this->bitvector = new CompressedBitvector(
        WaveletTree::BITVECTOR_BLOCK_SIZE, dummy_bitvector.size(), dummy_bitvector
    ); // the real bitvector

    auto pivot = stable_partition(from, to, lessThanMid);

    l = new WaveletTree(from, pivot, cod + '0', this);
    r = new WaveletTree(pivot, to, cod + '1', this);
}

WaveletTree::~WaveletTree()
{
    if (l)
        l->~WaveletTree();
    if (r)
        r->~WaveletTree();
}

int WaveletTree::access(int i)
{
    if(i > bitvector_size || i <= 0) return -1;

    WaveletTree *wt = this;
    CompressedBitvector *bitvector;
    bool bit;
    string cod;
    while (wt->l || wt->r) { // while is not the leaf
        bitvector = wt->bitvector;
        bit = bitvector->access(i);
        cod.push_back(bit + '0');
        if (bit == 1)
        {
            wt = wt->r;
            i = bitvector->rank1(i);
        }
        else
        {
            wt = wt->l;
            i = bitvector->rank0(i);
        }
    }

    return WaveletTree::cods[cod];
}

int WaveletTree::rank(unsigned c, int i)
{
    if(i > bitvector_size || i <= 0) return -1;

    WaveletTree *wt = this;
    CompressedBitvector *bitvector;
    bool bit;
    int K = 0;
    while ((wt->l || wt->r) && K < WaveletTree::codsInverted[c].size())
    { // while is not the leaf
        bitvector = wt->bitvector;
        bit = WaveletTree::codsInverted[c][K++] - '0';
        if (bit == 1)
        {
            wt = wt->r;
            i = bitvector->rank1(i);
        }
        else
        {
            wt = wt->l;
            i = bitvector->rank0(i);
        }

        // in case of the element is not present in the range or not exist
        if(i <= 0) return 0;
    }
    return i;
}

int WaveletTree::select(unsigned c, int i)
{
    if (i > bitvector_size || i <= 0) return -1;

    WaveletTree *wt = WaveletTree::leaf[c];
    CompressedBitvector *bitvector;
    bool bit;
    int K = WaveletTree::codsInverted[c].size() - 1;
    while (wt != this)
    { // while is not the root
        wt = wt->p;
        bitvector = wt->bitvector;
        bit = WaveletTree::codsInverted[c][K--] - '0';
        if (bit == 1)
        {
            i = bitvector->select1(i);
        }
        else
        {
            i = bitvector->select0(i);
        }
    }
    return i;
}

void WaveletTree::printBitvector()
{
    puts("");
    for (int i = 1; i <= bitvector->count(); i++)
        printf("%u", (unsigned)bitvector->access(i));
    puts("");
}

unsigned long treeSize(WaveletTree *tree)
{
    unsigned long leftSize = 0, rightSize = 0, bitvectorSize = 0;
    
    if(tree->bitvector)
        bitvectorSize = tree->bitvector->size();
    if (tree->l)
        leftSize = treeSize(tree->l);
    if (tree->r)
        rightSize = treeSize(tree->r);

    return bitvectorSize + leftSize + rightSize + sizeof(WaveletTree*) * 2 + sizeof(CompressedBitvector*);
}

unsigned long WaveletTree::size()
{
    long unsigned codsSize = 0;
    long unsigned codsInvertedSize = 0;
    long unsigned leafSize = 0;

    for (auto it : WaveletTree::cods)
    {
        codsSize += it.first.size() + 1; // plus one because of the \0
        codsSize += sizeof(unsigned);
    }
    
    codsInvertedSize = codsSize;

    for(auto it : WaveletTree::leaf) {
        leafSize += sizeof(unsigned);
        leafSize += sizeof(WaveletTree*);
    }

    return treeSize(this) + codsSize + codsInvertedSize + leafSize;
}