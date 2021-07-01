#include <bits/stdc++.h>
using namespace std;

map<string, unsigned> cods;
map<unsigned, string> codsI;
map<unsigned, void *> leaf;

int select1(vector<bool> &bitvector, int n)
{
    if (n >= bitvector.size())
        return -1;

    int r = 0;
    for (int i = 0; i <= n; i++)
    {
        if (r == n)
            return i;
        if (bitvector[i])
            r++;
    }
}

int select0(vector<bool> &bitvector, int n)
{
    if (n >= bitvector.size())
        return -1;

    int r = 0;
    for (int i = 0; i <= n; i++)
    {
        if (r == n)
            return i;
        if (!bitvector[i])
            r++;
    }
}

int rank1(vector<bool> &bitvector, int n)
{
    if (n >= bitvector.size())
        return -1;

    int r = 0;
    for (int i = 1; i <= n; i++)
        if (bitvector[i - 1])
            r++;
    return r;
}

int rank0(vector<bool> &bitvector, int n)
{
    if (n >= bitvector.size())
        return -1;

    int r = 0;
    for (int i = 1; i <= n; i++)
        if (!bitvector[i - 1])
            r++;
    return r;
}

class WaveletTree
{
private:
    WaveletTree *l = NULL, *r = NULL, *p = NULL;
    vector<bool> bitvector;

public:
    WaveletTree(unsigned *from, unsigned *to, unsigned lo, unsigned hi, string cod, WaveletTree *parent)
    {
        if (from >= to)
            return;

        this->p = parent;

        if (lo == hi)
        { // homogeneous array
            cods[cod] = lo;
            codsI[lo] = cod;
            leaf[lo] = this;
            return;
        }

        int sz = to - from;
        bitvector.resize(sz);
        unsigned mid = lo + (hi - lo) / 2;
        auto lessThanMid = [mid](unsigned x)
        { return x <= mid; };

        int idx = 0;
        for (auto it = from; it != to; it++)
            bitvector[idx++] = !lessThanMid(*it);

        // printBitvector();

        auto pivot = stable_partition(from, to, lessThanMid);

        l = new WaveletTree(from, pivot, lo, mid, cod + '0', this);
        r = new WaveletTree(pivot, to, mid + 1, hi, cod + '1', this);
    }

    ~WaveletTree()
    {
        if (l)
            l->~WaveletTree();
        if (r)
            r->~WaveletTree();
    }

    WaveletTree *child(int b)
    {
        if (b)
            return r;
        return l;
    }

    unsigned access(int i)
    {
        WaveletTree *wt = this;
        vector<bool> bitvector;
        unsigned b;
        string cod;
        while (wt->l || wt->r)
        { // while is not the leaf
            bitvector = wt->bitvector;
            b = bitvector[i];
            cod.push_back(b + '0');
            if (b)
            {
                wt = wt->r;
                i = rank1(bitvector, i);
            }
            else
            {
                wt = wt->l;
                i = rank0(bitvector, i);
            }
        }
        cout << cod << endl;
        return cods[cod];
    }

    int rank(unsigned c, int i)
    {
        // bugs em alguns acessos como rank(5, 9), rank(7, 5)
        WaveletTree *wt = this;
        vector<bool> bitvector;
        unsigned b;
        int k = 0;
        while ((wt->l || wt->r) && k < codsI[c].size())
        { // while is not the leaf
            bitvector = wt->bitvector;
            b = codsI[c][k++] - '0';
            if (b)
            {
                wt = wt->r;
                i = rank1(bitvector, i);
            }
            else
            {
                wt = wt->l;
                i = rank0(bitvector, i);
            }
        }
        return i;
    }

    int select(unsigned c, int i)
    {
        WaveletTree *wt = (WaveletTree *)leaf[c];
        vector<bool> bitvector;
        unsigned b;
        int k = codsI[c].size() - 1;
        while (wt != this)
        { // while is not the root
            wt = wt->p;
            bitvector = wt->bitvector;
            b = codsI[c][k--] - '0';
            if (b)
            {
                i = select1(bitvector, i);
            }
            else
            {
                i = select0(bitvector, i);
            }
        }
        return i;
    }

    void printBitvector()
    {
        puts("");
        for (int i = 0; i < bitvector.size(); i++)
            printf("%u", (unsigned)bitvector[i]);
        puts("");
    }
};

int main()
{
    unsigned arr[10] = {2, 1, 9, 1, 7, 9, 1, 5, 9, 1};
    int arrSize = sizeof(arr) / sizeof(unsigned);
    unsigned arrMin = arr[0], arrMax = arr[0];
    for (int i = 1; i < arrSize; i++)
    {
        arrMin = min(arrMin, arr[i]);
        arrMax = max(arrMax, arr[i]);
    }

    WaveletTree wt(arr, arr + arrSize, arrMin, arrMax, "", NULL);

    printf("Cods:\n");
    for (auto c : cods)
    {
        cout << c.first << " " << c.second << endl;
    }

    // while (1)
    // {
    //     int x;
    //     scanf("%d", &x);
    //     printf("%u\n", wt.access(x));
    // }

    // while (1)
    // {
    //     int c, x;
    //     scanf("%d%d", &c, &x);
    //     printf("%u\n", wt.rank(c, x));
    // }

    while (1)
    {
        int c, x;
        scanf("%d%d", &c, &x);
        printf("%u\n", wt.select(c, x));
    }
}