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
    this->S = new CompArray(n + 1, n + 1);
    this->m = 0;
    this->sz = ((b + 1) * (b + 1) + ceil(n / (float)k) * 2) * sizeof(int);
}

CompressedBitVector::CompressedBitVector(int b, int n, unsigned *B) : CompressedBitVector(b, n)
{
    compress(CompArray(b, n, B));
}

CompressedBitVector::CompressedBitVector(int b, int n, vector<bool>& bitvector) : CompressedBitVector(b, n)
{
    // first we need to convert all the bitvector in a array int with size of b bits
    cout << n << endl;
    for (int i = 0; i < bitvector.size(); i++)
    {
        cout << bitvector[i];
    }
    cout << endl;
    
    unsigned *B = (unsigned*) calloc(n, sizeof(unsigned));
    if(B) {
        int j = 0, x = b - 1;
        for(int i = 1; i <= bitvector.size(); i++, x--) {
            if(bitvector[i-1])
                B[j] |= 1 << x;
            if (i % b == 0)
            {
                j++;
                x = b;
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
    delete S;
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
            m++;
            S->write(m, i * b + j - 1);
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

int CompressedBitVector::access(int i)
{
    unsigned B1 = decode(i / b);
    // printf("B[%d] =  %d\n", i / b, B1);
    return B1 & (1 << (b - 1 - (i % b)));
}

int CompressedBitVector::rank1(int i)
{
    if (i == 0)                                    // gambiarra
        return decode(0) & (1 << (b - 1)) ? 1 : 0; // in case of the first bit be 1

    int is = ceil(i / ((float)k * b));
    if (i % (k * b) == 0)
        return R[is];

    int r = R[is - 1];
    int p = ceil(i / (float)b) - 1;
    for (int j = P[is - 1]; j < p; j++)
        r += C->read(j);

    unsigned B1 = decode(p);
    for (int j = 0; j <= i % b; j++)
        if (B1 & (1 << (b - 1 - (j % b))))
            r++;

    return r;
}

int CompressedBitVector::rank0(int i)
{
    return i - rank1(i);
}

int CompressedBitVector::select(int i)
{
    if (i > m || i <= 0)
        return -1;
    return S->read(i);
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
    return sz;
}