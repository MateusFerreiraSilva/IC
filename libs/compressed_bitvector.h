#include <bits/stdc++.h>
#include "../libs/compressed_array.h"
#include "../libs/sample_pointers.h"
using namespace std;

#ifndef COMPRESSED_BITVECTOR
#define COMPRESSED_BITVECTOR

#define k 4

class CompressedBitVector
{
private:
    int n, b;
    CompArray *C;
    SamplePointers *O;
    int **K;
    int *R;
    int *P;
    CompArray *S;
    int m; // number of 1's
    long unsigned sz = 0;

    pair<unsigned, unsigned> encode(CompArray &B, int i);
    unsigned decode(int i);
    void precompR();

public:
    CompressedBitVector(int b, int n);
    CompressedBitVector(int b, int n, unsigned *B);
    ~CompressedBitVector();
    void compress(CompArray B);
    int access(int i);
    int rank1(int i);
    int rank0(int i);
    int select(int i);
    void print();
    long unsigned size();
};

#endif