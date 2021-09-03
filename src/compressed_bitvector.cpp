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
    this->block_num = ceil(length * (float) block_size); // number of bits in the bitvector

    // TODO testes nos construtores
    // try { if(length > block_num * block_size) error }

    this->Comb = precompComb(Comb, block_size); // represents variable K
    this->SUPER_BLOCK_SIZE = block_size * k;
    this->SUPER_BLOCK_NUM = (block_num / SUPER_BLOCK_SIZE + 1);
    this->R = (unsigned *)malloc(SUPER_BLOCK_NUM * sizeof(unsigned));
    this->P = (unsigned *)malloc(SUPER_BLOCK_NUM * sizeof(unsigned));
    this->ones = 0;
    this->zeros = 0;
    this->sz = ((block_size + 1) * (block_size + 1) + SUPER_BLOCK_NUM * 2) * sizeof(int);
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
    free(Comb);
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
            ones++;
            
        } else {
            zeros++;
        }
    }

    unsigned o = 0;
    unsigned c1 = c;
    int j = 1;
    while (c1 > 0 && c1 <= block_size - j)
    {
        if (B1 & (1 << (block_size - j)))
        {
            o += Comb[block_size - j][c1];
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
        if (o >= Comb[block_size - j][c])
        {
            B1 |= (1 << (block_size - j)); // bitset
            o -= Comb[block_size - j][c];
            c--;
        }
        j++;
    }

    return B1;
}

void CompressedBitvector::precompR()
{

    unsigned r = 0, r_idx = 1;
    R[0] = r;
    for (int i = 1; i <= length; i++)
    {
        r += C->read(i - 1);
        if(i % block_size == 0)
            R[r_idx++] = r;
    }

    // TODO
    // when there is no more R block to be filled break the loop

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
    return B1 & (1 << (block_size - 1 - (i % block_size)));
}

// problema esta aqui
unsigned CompressedBitvector::rank1(unsigned i)
{
    if (i == 0 || i > length * block_size) return -1;

    unsigned is = ceil(i / (float)(block_size * k));
    if(i % (block_size * k) == 0) return R[is]; // allready know rank

    unsigned r = R[is - 1];
    for (int t = (is - 1) * k; t < floor(i / (float)block_size); t++) // walk in blocks
        r += C->read(t);

    unsigned aux = i % block_size;
    if(aux == 0) return r; // aux is in the end of block the block
    // else aux = number of bit to be readed for the next block

    unsigned x = decode(ceil(i / (float)block_size) - 1);
    unsigned b = 0;
    for (int j = 0; j < aux; j++)
        b += x & (1 << (block_size - 1) - j) ? 1 : 0;
    return r + b;
}

unsigned CompressedBitvector::rank0(unsigned i)
{
    if (i == 0 || i > length * block_size) return -1;
    return i - rank1(i);
}

/*
    return idx o mid position closer of a R stored value return allways the greather
*/


/*
    do a binary search in R then return smallest closest idx of the searched rank
*/
unsigned CompressedBitvector::rank1_binary_search(unsigned lo_idx, unsigned hi_idx, unsigned rank)
{
    if(lo_idx < 0 || hi_idx >= SUPER_BLOCK_NUM) return 0;
    

    if (lo_idx >= hi_idx)
    {
        uint idx = min(lo_idx, hi_idx);
        if (rank <= R[idx])
            return idx ? idx - 1 : 0;
        return idx;
    }

    uint mid_idx = lo_idx + (hi_idx - lo_idx) / 2;

    if (rank < R[mid_idx])
        return rank1_binary_search(lo_idx, mid_idx - 1, rank);
    else if (rank > R[mid_idx])
        return rank1_binary_search(mid_idx + 1, hi_idx, rank);
    else if (rank == R[mid_idx])
        return mid_idx - 1;
}

unsigned CompressedBitvector::select1(unsigned i)
{
    if (i > ones) return -1;

    unsigned idx = rank1_binary_search(0, SUPER_BLOCK_NUM - 1, i);
    unsigned c, r = R[idx];
    idx = idx * SUPER_BLOCK_SIZE;

    while(true) {
        c = C->read(idx / block_size); // walk in C blocks
        if (r + c >= i) break;
        r += c;
        idx += block_size;
    }

    unsigned x = decode(idx / block_size);
    for (int j = 0; j < block_size; j++) {
        idx++;
        if(x & (1 << (block_size - 1) - j)) r++;
        if (r == i) break;
    }

    return idx;
}

unsigned CompressedBitvector::rank0_binary_search(unsigned lo_idx, unsigned hi_idx, unsigned rank)
{
    if (lo_idx < 0 || hi_idx >= SUPER_BLOCK_NUM)
        return 0;

    if (lo_idx >= hi_idx)
    {
        uint idx = min(lo_idx, hi_idx);
        uint offset = idx * SUPER_BLOCK_SIZE;
        if (rank <= offset - R[idx])
            return idx ? idx - 1 : 0;
        return idx;
    }

    uint mid_idx = lo_idx + (hi_idx - lo_idx) / 2;
    uint offset = mid_idx * SUPER_BLOCK_SIZE;

    if (rank < offset - R[mid_idx])
        return rank0_binary_search(lo_idx, mid_idx - 1, rank);
    else if (rank > offset - R[mid_idx])
        return rank0_binary_search(mid_idx + 1, hi_idx, rank);
    else if (rank == offset - R[mid_idx])
        return mid_idx - 1;
}

unsigned CompressedBitvector::select0(unsigned i)
{
    if (i > zeros)
        return -1;

    uint idx = rank0_binary_search(0, SUPER_BLOCK_NUM - 1, i);
    uint c, r = idx * SUPER_BLOCK_SIZE - R[idx];
    idx = idx * SUPER_BLOCK_SIZE;

    while (true)
    {
        c = block_size - C->read(idx / block_size); // walk in C blocks
        if (r + c >= i)
            break;
        r += c;
        idx += block_size;
    }

    unsigned x = decode(idx / block_size);
    for (int j = 0; j < block_size; j++)
    {
        idx++;
        if (!(x & (1 << (block_size - 1) - j)))
            r++;
        if (r == i)
            break;
    }

    return idx;
}

// unsigned CompressedBitvector::select0(unsigned i)
// {
//     if (i > zeros) return -1;

//     for (int j = 1; j <= length; j++)
//         if(rank0(j) == i) return j;
// }

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