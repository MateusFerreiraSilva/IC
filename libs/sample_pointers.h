#include <bits/stdc++.h>
using namespace std;

#ifndef SAMPLE_POINTERS
#define SAMPLE_POINTERS

typedef unsigned long long ull;

#define BITS 32

class SamplePointers
{
private:
    uint *B; // where the bits sequences are stored
    uint *P; // array of pointer to fast travel B array
    uint n; // number of elements
    uint k; // arbitrary number
    uint B_SIZE;
    uint P_SIZE;
    uint rear; // position of the last bit
    ulong sz;

public:
    SamplePointers(uint n, uint k);
    SamplePointers(uint n, uint k, uint *arr);
    ~SamplePointers();

    void bitset(uint i, bool val);
    bool bitread(uint i);

    /*
        > given a value return it encoded in gamma
        > can make gamma codes with the max of 64 bits of length    
    */
    pair<uint, ull> toGamma(uint val); 
    uint gammaDecode(uint bit_start_position);

    void write(uint i, uint val);
    uint read(uint i);

    void expand(uint extra_size);

    static string gammaEncode(int x);
    void toGamma(uint *arr);
    string getGammaCode();
    int findPos(int pos);
    void compress(uint *arr);
    uint gammaDecode(int i);
    ulong size();
};

#endif