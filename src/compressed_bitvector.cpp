#include <bits/stdc++.h>
#include "../libs/bitarray.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/combination.h"
using namespace std;

CompressedBitvector::CompressedBitvector(unsigned block_size, unsigned long length)
{
    this->block_size = block_size; // block size in bits
    this->length = length;
    this->block_num = ceil(length / (float) block_size);

    // TODO testes nos construtores
    // try { if(length > block_num * block_size) error }

    this->K = precompComb(K, block_size);

    unsigned SIZE_AUX = ceil(length / (float)k) * sizeof(int);
    this->R = (int *)malloc(SIZE_AUX);
    this->P = (int *)malloc(SIZE_AUX);
    // this->S1 = new Bitarray(length + 1, length + 1);
    // this->S0 = new Bitarray(length + 1, length + 1);
    this->m1 = 0;
    this->m0 = 0;
    this->sz = ((block_size + 1) * (block_size + 1) + SIZE_AUX * 2) * sizeof(int);
}

CompressedBitvector::CompressedBitvector(unsigned block_size, unsigned long length, unsigned *B) : CompressedBitvector(block_size, length)
{
    compress(Bitarray(block_size, length, B));
}

/*
 * block_size = size of the block
 * length = quantity of elements
*/
CompressedBitvector::CompressedBitvector(unsigned block_size, unsigned long length, vector<bool> &bitvector) : CompressedBitvector(block_size,length)
{
    // first we need to convert all the bitvector in a array int with size of block_size bits
    unsigned *B = (unsigned*) calloc(length, sizeof(unsigned));
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

        compress(Bitarray(block_size, length, B));
        free(B);
        vector<bool> aux(bitvector.size(), 0);
        for (int i = 0; i < bitvector.size(); i++)
            aux[i] = access(i + 1);
        assert(aux.size() == bitvector.size());
    }
}

CompressedBitvector::~CompressedBitvector()
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

pair<unsigned, unsigned> CompressedBitvector::encode(Bitarray &B, int i)
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

unsigned CompressedBitvector::decode(int i)
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

void CompressedBitvector::precompR()
{
    memset(R, 0, sizeof(R));
    P[0] = 0;
    // TODO trocar por SIZEAUX
    for (int i = 1; i < ceil(length / (float)k); i++)
    {
        P[i] = i * k;
        R[i] = R[i - 1];
        for (int j = 0; j < block_size; j++)
            R[i] += C->read((i - 1) * k + j);
        // printf("R[%d] = %d\n", i, R[i]);
    }
}

void CompressedBitvector::compress(Bitarray B)
{
    unsigned *C = (unsigned *)malloc(length * sizeof(unsigned));
    unsigned *O = (unsigned *)malloc(length * sizeof(unsigned));

    if (C == NULL || O == NULL)
    {
        printf("Memory allocation error\n");
        return;
    }

    unsigned maxO = 0;
    for (int i = 0; i < length; i++)
    {
        pair<unsigned, unsigned> aux = encode(B, i);
        C[i] = aux.first;
        O[i] = aux.second;
        maxO = max(maxO, O[i]);
    }

    this->C = new Bitarray(block_size, length, C);
    this->O = new SamplePointers(length, k, maxO, O);
    this->sz = this->sz + this->C->size() + this->O->size();
    free(C);
    free(O);
    precompR();
}

bool CompressedBitvector::access(unsigned i)
{
    if(i == 0 || i > length) return -1;

    i--;
    unsigned B1 = decode(i / block_size);
    // printf("B[%d] =  %d\n", i / block_size, B1);
    return B1 & (1 << (block_size - 1 - (i % block_size)));
}

unsigned CompressedBitvector::rank1(unsigned i)
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

unsigned CompressedBitvector::rank0(unsigned i)
{
    if (i == 0 || i > length) return -1;
    return i - rank1(i);
}

unsigned CompressedBitvector::select1(unsigned i)
{
    if (i > m1) return -1;
    // return S1->read(i);

    for (int j = 1; j <= length; j++)
        if (rank1(j) == i) return j;
}

unsigned CompressedBitvector::select0(unsigned i)
{
    if (i > m0) return -1;
    // return S0->read(i);

    for (int j = 1; j <= length; j++)
        if(rank0(j) == i) return j;
}

void CompressedBitvector::print()
{
    for (int i = 0; i < length; i++)
    {
        printf("%u ", decode(i));
    }
    puts("");
}

long unsigned CompressedBitvector::size()
{
    return this->sz + this->C->size() + this->O->size();
}

long unsigned CompressedBitvector::count() // return the number of elements
{
    return this->length;
}