#include <bits/stdc++.h>
using namespace std;

#ifndef SAMPLE_POINTERS
#define SAMPLE_POINTERS

int calcL(int x);

class SamplePointers
{
private:
    const int w = 32; // amount of bits in a unsigned
    unsigned *B, *P;
    int n; // number of elements
    int k; // arbitrary number
    int l; // max size of bits that a element will have
    long unsigned sz = 0;
    string gammaCode;

public:
    SamplePointers(int n, int k, int max);
    SamplePointers(int n, int k, int max, unsigned *B);
    ~SamplePointers();
    static string gammaEncode(int x);
    void toGamma(unsigned *arr);
    string getGammaCode();
    int findPos(int pos);
    void compress(unsigned *arr);
    unsigned gammaDecode(int i);
    long unsigned size();
};

#endif