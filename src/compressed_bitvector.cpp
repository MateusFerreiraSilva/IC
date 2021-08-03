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

CompressedBitVector::CompressedBitVector(unsigned block_size, unsigned block_num, unsigned long length)
{
    this->block_size = block_size; // block size in bits
    this->block_num = block_num; // number of bits;
    this->length = length;

    // TODO testes nos construtores
    // try { if(length > block_num * block_size) error }

    this->K = precompComb(K, block_size);
    this->R = (int *)malloc(ceil(block_num / (float)k) * sizeof(int));
    this->P = (int *)malloc(ceil(block_num / (float)k) * sizeof(int));
    // this->S1 = new CompArray(block_num + 1, block_num + 1);
    // this->S0 = new CompArray(block_num + 1, block_num + 1);
    this->m1 = 0;
    this->m0 = 0;
    this->sz = ((block_size + 1) * (block_size + 1) + ceil(block_num / (float)k) * 2) * sizeof(int);
}

CompressedBitVector::CompressedBitVector(unsigned block_size, unsigned block_num, unsigned long length, unsigned *B) : CompressedBitVector(block_size, block_num, length)
{
    compress(CompArray(block_size, block_num, B));
}

/*
 * block_size = size of the block
 * block_num = number of blocks
*/
CompressedBitVector::CompressedBitVector(unsigned block_size, unsigned block_num, unsigned long length, vector<bool> &bitvector) : CompressedBitVector(block_size, block_num, length)
{
    // first we need to convert all the bitvector in a array int with size of block_size bits
    unsigned *B = (unsigned*) calloc(block_num, sizeof(unsigned));
    if(B) {
        int j = 0, x = block_size - 1;
        for(int i = 1; i <= bitvector.size(); i++, x--) {
            if(bitvector[i-1])
                B[j] |= 1 << x;
            if (i % block_size == 0)
            {
                j++;
                x = block_size; // x will be decremented in the end of the loop, so x = block_size - 1
            }
        }

        compress(CompArray(block_size, block_num, B));
        free(B);
        vector<bool> aux(bitvector.size(), 0);
        for (int i = 0; i < bitvector.size(); i++)
            aux[i] = access(i + 1);
        assert(aux.size() == bitvector.size());
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
    for (int j = 1; j <= block_size; j++)
    {
        if (B1 & (1 << (block_size - j)))
        {
            c++;
            m1++;
            // S1->write(m1, i * block_size + j - 1);
            
        } else {
            m0++;
            // S0->write(m0, i * block_size + j - 1);
        }
    }

    unsigned o = 0;
    unsigned c1 = c;
    int j = 1;
    while (c1 > 0 && c1 <= block_size - j)
    {
        if (B1 & (1 << (block_size - j)))
        {
            o += K[block_size - j][c1];
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
        if (o >= K[block_size - j][c])
        {
            B1 |= (1 << (block_size - j)); // bitset
            o -= K[block_size - j][c];
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
    for (int i = 1; i < ceil(block_num / (float)k); i++)
    {
        P[i] = i * k;
        R[i] = R[i - 1];
        for (int j = 0; j < block_size; j++)
            R[i] += C->read((i - 1) * k + j);
        // printf("R[%d] = %d\n", i, R[i]);
    }
}

void CompressedBitVector::compress(CompArray B)
{
    unsigned *C = (unsigned *)malloc(block_num * sizeof(unsigned));
    unsigned *O = (unsigned *)malloc(block_num * sizeof(unsigned));

    if (C == NULL || O == NULL)
    {
        printf("Memory allocation error\n");
        return;
    }

    unsigned maxO = 0;
    for (int i = 0; i < block_num; i++)
    {
        pair<unsigned, unsigned> aux = encode(B, i);
        C[i] = aux.first;
        O[i] = aux.second;
        maxO = max(maxO, O[i]);
    }

    this->C = new CompArray(block_size, block_num, C);
    this->O = new SamplePointers(block_num, k, maxO, O);
    this->sz = this->sz + this->C->size() + this->O->size();
    free(C);
    free(O);
    precompR();
}

bool CompressedBitVector::access(unsigned i)
{
    if(i == 0 || i > length) return -1;

    i--;
    unsigned B1 = decode(i / block_size);
    // printf("B[%d] =  %d\n", i / block_size, B1);
    return B1 & (1 << (block_size - 1 - (i % block_size)));
}

unsigned CompressedBitVector::rank1(unsigned i)
{
    if (i == 0 || i > length) return -1;

    i--;
    if (i == 0) return decode(0) & (1 << (block_size - 1)) ? 1 : 0; // in case of the first bit be 1

    int is = ceil(i / ((float)k * block_size));
    if (i % (k * block_size) == 0) return R[is];

    // no caso rank 4 5 R[0] block_num não está correto, deveria ser 1 e nao 0 e isso gera erro nos proximos
    int r = R[is - 1];
    int p = ceil(i / (float)block_size) - 1;
    for (int j = P[is - 1]; j < p; j++)
        r += C->read(j);

    unsigned B1 = decode(p);
    for (int j = 0; j <= i % block_size; j++)
        if (B1 & (1 << (block_size - 1 - (j % block_size)))) r++;

    return r;
}

unsigned CompressedBitVector::rank0(unsigned i)
{
    if (i == 0 || i > length) return -1;
    return i - rank1(i);
}

unsigned CompressedBitVector::select1(unsigned i)
{
    if (i > m1) return -1;
    // return S1->read(i);

    for (int j = 1; j <= length; j++)
        if (rank1(j) == i) return j;
}

unsigned CompressedBitVector::select0(unsigned i)
{
    if (i > m0) return -1;
    // return S0->read(i);

    for (int j = 1; j <= length; j++)
        if(rank0(j) == i) return j;
}

void CompressedBitVector::print()
{
    for (int i = 0; i < block_num; i++)
    {
        printf("%u ", decode(i));
    }
    puts("");
}

long unsigned CompressedBitVector::size()
{
    return block_num;
}

long unsigned CompressedBitVector::count() // return the number of elements
{
    return this->block_num;
}