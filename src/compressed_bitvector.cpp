#include <bits/stdc++.h>
#include "../libs/compressed_array.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
using namespace std;

// Combination

int *memo;

int fat(int x)
{
    if (x == 1 || x == 0)
        return 1;
    else if (memo[x])
        return memo[x];
    return memo[x] = x * fat(x - 1);
}

int comb(int a, int b)
{
    if (a < b)
        return 0;
    else if (a == b)
        return 1;
    return fat(a) / (fat(a - b) * fat(b));
}

int **precompComb(int **K, int x)
{
    K = (int **)malloc((x + 1) * sizeof(int *));
    for (int i = 0; i <= x; i++)
        K[i] = (int *)malloc((x + 1) * sizeof(int));

    memo = (int *)malloc((x + 1) * sizeof(int));
    memset(memo, 0, sizeof(memo));
    for (int i = x; i >= 0; i--)
        for (int j = x; j >= 0; j--)
            K[i][j] = comb(i, j);

    free(memo);
    return K;
}

// END Combination

CompressedBitVector::CompressedBitVector(int b, int n)
{
    this->b = b; // block size in bits
    this->n = n;
    this->K = precompComb(K, b);
    this->R = (int *)malloc(ceil(n / (float)k) * sizeof(int));
    this->P = (int *)malloc(ceil(n / (float)k) * sizeof(int));
    // this->S1 = new CompArray(n + 1, n + 1);
    // this->S0 = new CompArray(n + 1, n + 1);
    this->m1 = 0;
    this->m0 = 0;
    this->sz = ((b + 1) * (b + 1) + ceil(n / (float)k) * 2) * sizeof(int);
}

CompressedBitVector::CompressedBitVector(int b, int n, unsigned *B) : CompressedBitVector(b, n)
{
    compress(CompArray(b, n, B));
}

/*
 * b = size of the block
 * n = number of blocks
*/
CompressedBitVector::CompressedBitVector(int b, int n, vector<bool>& bitvector) : CompressedBitVector(b, n)
{
    // first we need to convert all the bitvector in a array int with size of b bits
    unsigned *B = (unsigned*) calloc(n, sizeof(unsigned));
    if(B) {
        int j = 0, x = b - 1;
        for(int i = 1; i <= bitvector.size(); i++, x--) {
            if(bitvector[i-1])
                B[j] |= 1 << x;
            if (i % b == 0)
            {
                j++;
                x = b; // x will be decremented in the end of the loop, so x = b - 1
            }
        }

        compress(CompArray(b, n, B));
        free(B);
    }
}

CompressedBitVector::~CompressedBitVector()
{
    delete C;
    delete O;
    // bug, double free ao desalocar S's
    // delete S1;
    // delete S0;
    free(K);
    free(R);
    free(P);
}

pair<unsigned, unsigned> CompressedBitVector::encode(CompArray &B, int i)
{
    unsigned B1 = B.read(i);
    unsigned c = 0;
    for (int j = 1; j <= b; j++)
    {
        if (B1 & (1 << (b - j)))
        {
            c++;
            m1++;
            // S1->write(m1, i * b + j - 1);
            
        } else {
            m0++;
            // S0->write(m0, i * b + j - 1);
        }
    }

    unsigned o = 0;
    unsigned c1 = c;
    int j = 1;
    while (c1 > 0 && c1 <= b - j)
    {
        if (B1 & (1 << (b - j)))
        {
            o += K[b - j][c1];
            c1--;
        }
        j++;
    }

    return make_pair(c, o);
}

unsigned CompressedBitVector::decode(int i)
{
    int c = C->read(i);
    int o = O->gammaDecode(i);

    int B1 = 0;
    int j = 1;
    while (c > 0)
    {
        if (o >= K[b - j][c])
        {
            B1 |= (1 << (b - j)); // bitset
            o -= K[b - j][c];
            c--;
        }
        j++;
    }

    return B1;
}

void CompressedBitVector::precompR()
{
    memset(R, 0, sizeof(R));
    // printf("R[0] = %d\n", R[0]);
    P[0] = 0;
    for (int i = 1; i < ceil(n / (float)k); i++)
    {
        P[i] = i * k;
        R[i] = R[i - 1];
        for (int j = 0; j < b; j++)
            R[i] += C->read((i - 1) * k + j);
        // printf("R[%d] = %d\n", i, R[i]);
    }
}

void CompressedBitVector::compress(CompArray B)
{
    unsigned *C = (unsigned *)malloc(n * sizeof(unsigned));
    unsigned *O = (unsigned *)malloc(n * sizeof(unsigned));

    if (C == NULL || O == NULL)
    {
        printf("Memory allocation error\n");
        return;
    }

    unsigned maxO = 0;
    for (int i = 0; i < n; i++)
    {
        pair<unsigned, unsigned> aux = encode(B, i);
        C[i] = aux.first;
        O[i] = aux.second;
        maxO = max(maxO, O[i]);
    }

    this->C = new CompArray(b, n, C);
    this->O = new SamplePointers(n, k, maxO, O);
    this->sz = this->sz + this->C->size() + this->O->size();
    free(C);
    free(O);
    precompR();
}

bool CompressedBitVector::access(int i)
{
    if(i <= 0 || i > b * n) return -1;

    i--;
    unsigned B1 = decode(i / b);
    // printf("B[%d] =  %d\n", i / b, B1);
    return B1 & (1 << (b - 1 - (i % b)));
}

int CompressedBitVector::rank1(int i)
{
    if (i <= 0 || i > b * n) return -1;

    i--;
    if (i == 0) return decode(0) & (1 << (b - 1)) ? 1 : 0; // in case of the first bit be 1

    int is = ceil(i / ((float)k * b));
    if (i % (k * b) == 0) return R[is];

    int r = R[is - 1];
    int p = ceil(i / (float)b) - 1;
    for (int j = P[is - 1]; j < p; j++)
        r += C->read(j);

    unsigned B1 = decode(p);
    for (int j = 0; j <= i % b; j++)
        if (B1 & (1 << (b - 1 - (j % b)))) r++;

    return r;
}

int CompressedBitVector::rank0(int i)
{
    if (i <= 0 || i > b * n) return -1;
    return i - rank1(i);
}

int CompressedBitVector::select1(int i)
{
    if (i > m1 || i < 0) return -1;
    // return S1->read(i);

    for (int j = 1; j <= b * n; j++)
        if (rank1(j) == i) return j;
}

int CompressedBitVector::select0(int i)
{
    if (i > m0 || i < 0) return -1;
    // return S0->read(i);

    for (int j = 1; j <= b * n; j++)
        if(rank0(j) == i) return j;
}

void CompressedBitVector::print()
{
    for (int i = 0; i < n; i++)
    {
        printf("%u ", decode(i));
    }
    puts("");
}

long unsigned CompressedBitVector::size()
{
    return n;
}

long unsigned CompressedBitVector::count() // return the number of elements
{
    return this->n;
}