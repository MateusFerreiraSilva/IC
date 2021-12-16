#include <bits/stdc++.h>
#include "../libs/bitarray.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/combination.h"
#include "../libs/memprofile.h"
using namespace std;

CompressedBitvector::CompressedBitvector(uint block_size, ulong length)
{
    this->block_size = block_size; // block size in bits
    this->length = length;
    this->block_num = length * block_size; // number of bits in the bitvector

    this->Comb = precompComb(Comb, block_size); // represents variable K
    this->SUPER_BLOCK_SIZE = block_size * k;
    this->SUPER_BLOCK_NUM = (block_num / SUPER_BLOCK_SIZE + 1);
    this->R = (uint*) new uint[SUPER_BLOCK_NUM];
    this->P = (uint*) new uint[SUPER_BLOCK_NUM];
    this->ones = 0;
    this->zeros = 0;
    this->sz = ((block_size + 1) * (block_size + 1) + SUPER_BLOCK_NUM * 2) * sizeof(int);
}

CompressedBitvector::CompressedBitvector(uint block_size, ulong length, uint *B) : CompressedBitvector(block_size, length)
{
    compress(Bitarray(block_size, length, B));
}

/*
 * block_size = size of the block
 * length = quantity of elements
*/
CompressedBitvector::CompressedBitvector(uint block_size, ulong length, vector<bool> &bitvector) : CompressedBitvector(block_size,length)
{ 
    // first we need to convert all the bitvector in a array int with size of block_size bits

    uint *BLOCKS = (uint*) calloc(length, sizeof(uint));
    if(BLOCKS) {
        uint block_idx, bit_number;
        for(uint i = 0; i < bitvector.size(); i++) {
            if(i % block_size == 0) {
                bit_number =  block_size - 1;
                block_idx = i / block_size;
            } else bit_number--;
            
            if (bitvector[i])
                BLOCKS[block_idx] |= 1 << bit_number;
        }

        // printf("BLOCKS: [ ");
        // for (int i = 0; i < length; i++)
        // {
        //     printf("%d", BLOCKS[i]);
        //     if (i != length - 1)
        //         printf(", ");
        // }
        // printf("]\n");

        compress(Bitarray(block_size, length, BLOCKS));
        free(BLOCKS);
    }
}

CompressedBitvector::~CompressedBitvector()
{
    delete[] C;
    delete[] O;
    freeComb(Comb, block_size);
    delete[] R;
    delete[] P;
}

pair<uint, uint> CompressedBitvector::encode(Bitarray &B, uint i)
{
    uint BLOCK = B.read(i);
    uint c = 0;
    uint b = block_size;

    while (b > 0)
    {
        if (BLOCK & (1 << (b - 1)))
        {
            c++;
            ones++;
        } else {
            zeros++;
        }
        b--;
    }

    uint o = 0;
    uint c1 = c;
    b = block_size;

    while (c1 > 0)
    {
        if (BLOCK & (1 << (b - 1)))
        {
            o += Comb[b - 1][c1]; // Comb[b][c1] OK
            c1--;
        }
        b--;
    }

    return make_pair(c, o);
}

uint CompressedBitvector::decode(uint i)
{
    uint c = C->read(i);
    uint o = O->read(i); // quando access(1) o valor esperado para esse O eh 2 e nao 0
    uint b = block_size;

    uint BLOCK = 0;
    while (c > 0) // b = 0, c = 0, o = 0, Comb[b][c]
    {
        if (o >= Comb[b - 1][c])
        {
            BLOCK |= (1 << (b - 1));
            o -= Comb[b - 1][c];
            c--;
        }
        b--;
    }

    return BLOCK;
}

void CompressedBitvector::precompR()
{

    uint r = 0, r_idx = 1;
    R[0] = r;
    for (ulong i = 1; i <= length; i++)
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
    uint *C = (uint*) new uint[length];
    uint *O = (uint*) new uint[length];

    if (C == NULL || O == NULL)
    {
        printf("Memory allocation error\n");
        return;
    }

    // uint maxO = 0;
    for (uint i = 0; i < length; i++)
    {
        pair<uint, uint> aux = encode(B, i);
        C[i] = aux.first;
        O[i] = aux.second;
    }

    this->C = new Bitarray(block_size, length, C);
    
    this->O = new SamplePointers(length, k, O);

    // printf("O: [ ");
    // for (int i = 0; i < length; i++)
    // {
    //     printf("%d", O[i]);
    //     if (i != length - 1) printf(", ");
    // }
    // printf("]\n");
    
    this->sz = this->sz + this->C->size() + this->O->size();
    delete C;
    delete O;
    precompR();
}

bool CompressedBitvector::access(uint i)
{
    if(i == 0 || i > block_num) return 0;

    i--;
    uint block = decode(i / block_size);
    return block & (1 << (block_size - 1 - (i % block_size)));
}

uint CompressedBitvector::rank1(uint i)
{
    if (i == 0 || i > block_num) return 0;

    uint is = ceil(i / (float)(block_size * k));
    if(i % (block_size * k) == 0) return R[is]; // allready know rank

    uint r = R[is - 1];
    for (int t = (is - 1) * k; t < floor(i / (float)block_size); t++) // walk in blocks
        r += C->read(t);

    uint aux = i % block_size;
    if(aux == 0) return r; // aux is in the end of block the block
    // else aux = number of bit to be readed for the next block

    uint x = decode(ceil(i / (float)block_size) - 1);
    uint b = 0;
    for (uint j = 0; j < aux; j++)
        b += x & (1 << (block_size - 1) - j) ? 1 : 0; // TODO check
    return r + b;
}

uint CompressedBitvector::rank0(uint i)
{
    if (i == 0 || i > block_num) return 0;
    return i - rank1(i);
}

/*
    return idx o mid position closer of a R stored value return allways the greather
*/


/*
    do a binary search in R then return smallest closest idx of the searched rank
*/
uint CompressedBitvector::rank1_binary_search(uint lo_idx, uint hi_idx, uint rank)
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
    // else if (rank == R[mid_idx])
    return mid_idx - 1;
}

uint CompressedBitvector::select1(uint i)
{
    if (i > ones) return 0;

    uint idx = rank1_binary_search(0, SUPER_BLOCK_NUM - 1, i);
    uint c, r = R[idx];
    idx = idx * SUPER_BLOCK_SIZE;

    while(true) {
        c = C->read(idx / block_size); // walk in C blocks
        if (r + c >= i) break;
        r += c;
        idx += block_size;
    }

    // erro, select(5, 1), x = 1, deveria ser x = 2
    uint x = decode(idx / block_size);
    uint b = block_size;
    while (b > 0) {
        idx++;
        if(x & (1 << (b - 1))) r++;
        if (r == i) break;
        b--;
    }

    return idx;
}

uint CompressedBitvector::rank0_binary_search(uint lo_idx, uint hi_idx, uint rank)
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
    // else if (rank == offset - R[mid_idx])
    return mid_idx - 1;
}

uint CompressedBitvector::select0(uint i)
{
    if (i > zeros)
        return 0;

    uint idx = rank0_binary_search(0, SUPER_BLOCK_NUM - 1, i);
    uint c, r = idx * SUPER_BLOCK_SIZE - R[idx];
    idx = idx * SUPER_BLOCK_SIZE;

    while (true)
    {
        c = block_size - C->read(idx / block_size); // walk in C blocks
        if (r + c >= i) break;
        r += c;
        idx += block_size;
    }

    uint x = decode(idx / block_size);
    uint b = block_size;
    while (b > 0)
    {
        idx++;
        if (!(x & (1 << (b - 1)))) r++;
        if (r == i) break;
        b--;
    }

    return idx;
}

void CompressedBitvector::print_blocks()
{
    for (uint i = 0; i < length; i++)
    {
        printf("%u ", decode(i));
    }
    puts("");
}

void CompressedBitvector::print()
{
    for (uint i = 1; i <= block_num; i++)
        cout << access(i);
    puts("");
}

ulong CompressedBitvector::size()
{
    return this->sz + this->C->size() + this->O->size();
}

ulong CompressedBitvector::count() // return the number of elements
{
    return this->length;
}