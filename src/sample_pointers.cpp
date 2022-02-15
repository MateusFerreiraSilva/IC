#include <bits/stdc++.h>
#include "../libs/sample_pointers.h"
using namespace std;

SamplePointers::SamplePointers(uint n) {
    try {
        this->n = n;
        this->B = NULL;
        this->P = NULL;
        this->B_SIZE = 0;
        this->P_SIZE = 5;
        this->rear = 0;
        this->sz = 0;

        this->P = (uint *) calloc(P_SIZE, sizeof(uint));

        if (P == NULL) throw;

    } catch (...) {
        printf("Error on SamplePointer creation\n");
    }
}

SamplePointers::SamplePointers(uint n, uint *arr) : SamplePointers(n)
{
    compress(arr);
}

SamplePointers::~SamplePointers()
{
    free(B);
    free(P);
}

void SamplePointers::expand(uint extra_size) {
    try {
        if (B == NULL)
            B = (uint *) malloc((B_SIZE + extra_size) * sizeof(uint));
        else
            B = (uint *) realloc(B, (B_SIZE + extra_size) * sizeof(uint));

        if (B == NULL) throw;
        B_SIZE += extra_size;
    } catch (...) {
        printf("Realloc error SamplePointers expand\n");
    }
}

void SamplePointers::bitset(uint i, bool val) {
    uint B_IDX = i / BITS;
    uint BIT_IDX = BITS - (i % BITS) - 1;
    
    if (val)
        B[B_IDX] |= 1 << BIT_IDX;
    else
        B[B_IDX] &= ~(1 << BIT_IDX); // verificar
}

bool SamplePointers::bitread(uint i) {
    uint B_IDX = i / BITS;
    uint BIT_IDX = BITS - (i % BITS) - 1;
    return B[B_IDX] & (1 << BIT_IDX);
}

pair<uint, ull> SamplePointers::toGamma(uint val) {
    uint N = floor(log2(val)); // greather power of 2 which less tha val 
    ull gammaCode = 0; // zeros in gamma code will be implicit
    gammaCode |= 1 << N;
    for (int i = N; i >= 0; i--)
        gammaCode |= val & (1 << i);
    
    const uint gamaCodeSize = N * 2 + 1;
   
   return make_pair(gamaCodeSize, gammaCode);   
}

uint SamplePointers::gammaDecode(uint bit_start_position) {
    uint idx = bit_start_position;
    uint N = 0;

    while (bitread(idx++) == 0) N++;

    uint binary_representation = 0;
    for (int i = N - 1; i >= 0; i--, ++idx) {
        if (bitread(idx))
            binary_representation |= 1 << i;
    }

    return (1 << N) +  binary_representation;
}

void SamplePointers::write(uint i, uint val) {
    pair<uint, ull> gamma = toGamma(val + 1);
    uint gammaCodeSize = gamma.first;
    ull gammaCode = gamma.second;
 
    if (((rear + gammaCodeSize) / BITS) >= B_SIZE)
        expand((((rear + gammaCodeSize) / BITS) - B_SIZE) + 1); // expand by the difference plus one

    uint offset = gammaCodeSize - 1;
    for (uint j = 0; j < gammaCodeSize; j++, offset--)
        bitset(rear + j, gammaCode & (1 << offset));

    if (i % Constants::SAMPLE_POINTERS_PARAM_K == 0)
        if (i / Constants::SAMPLE_POINTERS_PARAM_K < P_SIZE)
            P[i / Constants::SAMPLE_POINTERS_PARAM_K] = rear;

    rear += gammaCodeSize;
}

uint SamplePointers::read(uint i) {
    uint bit_idx = 0;
    if (i / Constants::SAMPLE_POINTERS_PARAM_K >= P_SIZE) bit_idx = P[P_SIZE - 1]; // last pointers
    else bit_idx = P[i /  Constants::SAMPLE_POINTERS_PARAM_K];

    uint current_idx = (i / Constants::SAMPLE_POINTERS_PARAM_K) * Constants::SAMPLE_POINTERS_PARAM_K; // floor(i / Constants::SAMPLE_POINTERS_PARAM_K) * Constants::SAMPLE_POINTERS_PARAM_K

    while (current_idx != i) { // go to i start position in B
        uint N = 0;
        while (bitread(bit_idx) == 0) {
            N++;
            bit_idx++;
        }
        bit_idx += N + 1;
        current_idx++;
    }

    return gammaDecode(bit_idx) - 1;
}

void SamplePointers::compress(uint *arr) {
    for (uint i = 0; i < n; i++)
        write(i, arr[i]);
}

ulong SamplePointers::size()
{
    return this->sz + (B_SIZE + P_SIZE) * sizeof(uint);
}