#include <bits/stdc++.h>
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"
using namespace std;

map<string, unsigned>WaveletTree::cods;
map<unsigned, string>WaveletTree::codsInverted;
map<unsigned, WaveletTree *>WaveletTree::leaf;

WaveletTree::WaveletTree(unsigned *from, unsigned *to, unsigned lo, unsigned hi, string cod, WaveletTree *parent)
{
    if (from >= to)
        return;

    this->p = parent; // not using

    if (lo == hi)
    { // homogeneous array
        WaveletTree::cods[cod] = lo;
        WaveletTree::codsInverted[lo] = cod;
        WaveletTree::leaf[lo] = this;
        return;
    }

    this->bitvector_size = to - from;
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

    l = new WaveletTree(from, pivot, lo, mid, cod + '0', this);
    r = new WaveletTree(pivot, to, mid + 1, hi, cod + '1', this);
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
    unsigned b;
    string cod;
    while (wt->l || wt->r) { // while is not the leaf
        bitvector = wt->bitvector;
        b = bitvector->access(i);
        cod.push_back(b + '0');
        if (b)
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
    unsigned bit;
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
    unsigned bit;
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

// int main()
// {
//     unsigned arr[10] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
//     int arrSize = sizeof(arr) / sizeof(unsigned);
//     unsigned arrMin = arr[0], arrMax = arr[0];
//     for (int i = 1; i < arrSize; i++)
//     {
//         arrMin = min(arrMin, arr[i]);
//         arrMax = max(arrMax, arr[i]);
//     }

//     WaveletTree wt(arr, arr + arrSize, arrMin, arrMax, "", NULL);

//     printf("Cods:\n");
//     for (auto c : WaveletTree::cods) cout << c.first << " " << c.second << endl;
//     puts("");
//     for(int i = 1; i <= arrSize; i++) printf("access(%d): %u\n", i, wt.access(i));
//     // while (1)
//     // {
//     //     int c, x;
//     //     scanf("%d%d", &c, &x);
//     //     printf("%u\n", wt.rank(c, x));
//     // }

//     while (1)
//     {
//         int c, x;
//         scanf("%d%d", &c, &x);
//         printf("%u\n", wt.select(c, x));
//     }
// }