#include <bits/stdc++.h>
#include "../libs/sample_pointers.h"
using namespace std;

SamplePointers::SamplePointers(uint n, uint k) {
    try {
        this->n = n;
        this->k = k;
        this->B = NULL;
        this->P = NULL;
        this->B_SIZE = 0;
        this->P_SIZE = ceil(n / (float) k);
        this->rear = 0;
        this->sz = 0;

        this->P = (uint *) calloc(P_SIZE, sizeof(uint));

        if (P == NULL) throw;

    } catch (...) {
        printf("Error on SamplePointer creation\n");
    }
}

SamplePointers::SamplePointers(uint n, uint k, uint *arr) : SamplePointers(n, k)
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

    if (i % k == 0)
        P[i / k] = rear;

    rear += gammaCodeSize;
}

uint SamplePointers::read(uint i) {
    uint bit_idx = 0;
    if (i / k >= P_SIZE) bit_idx = P[P_SIZE - 1]; // last pointers
    else bit_idx = P[i /  k];

    uint current_idx = (i / k) * k; // floor(i / k) * k

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

// string SamplePointers::gammaEncode(int x)
// {
    // int N = 0;
    // while ((1 << (N + 1)) <= x)
    //     N++;

    // int aux = x - (1 << N);

    // string str;
    // for (int i = 0; i < N; i++)
    //     str.push_back('0');
    // str.push_back('1');

    // for (int i = N - 1; i >= 0; i--)
    // {
    //     if (aux & (1 << i))
    //         str.push_back('1');
    //     else
    //         str.push_back('0');
    // }

    // return str;
// }

// void SamplePointers::toGamma(uint *arr)
// {
//     gammaCode = "";
//     for (int i = 0; i < n; i++)
//         gammaCode += gammaEncode(arr[i] + 1);
// }

// string SamplePointers::getGammaCode()
// {
//     return gammaCode;
// }

// int SamplePointers::findPos(int pos)
// {
//     int N = 0, currPos = 0;
//     for (int i = 0; i < 32 * B_SIZE; i++)
//     {
//         if (B[i / 32] & (1 << (31 - (i % 32))))
//         {
//             if (currPos == pos)
//                 return i - N;
//             i += N;
//             N = 0;
//             currPos++;
//         }
//         else
//             N++;
//     }
// }

// void SamplePointers::compress(uint *arr)
// {
//     toGamma(arr);
//     for (int i = 0; i < gammaCode.size(); i++)
//         if (gammaCode[i] - '0')
//             B[i / 32] |= (1 << (31 - (i % 32)));

//     gammaCode.clear();

//     int pos;
//     for (int i = 0; i < ceil(n / (float)k); i++)
//     {
//         pos = i * k;
//         P[i] = findPos(pos);
//     }
// }

// uint SamplePointers::gammaDecode(int i)
// {
    // int aux = numeric_limits<int>::max(), idx;
    // for (int j = 0; j < ceil(n / (float)k); j++)
    // {
    //     if (abs(i - j * k) < aux && j * k <= i) // catch lower distance pointer of P to i
    //     {
    //         aux = abs(i - j * k);
    //         idx = j;
    //     }
    // }

    // int N = 0;
    // int currPos = idx * k;
    // for (int j = P[idx]; j < 32 * B_SIZE; j++)
    // {
    //     if (B[j / 32] & (1 << (31 - (j % 32))))
    //     {
    //         if (currPos == i)
    //         {
    //             uint ans;
    //             ans = 1 << N;
    //             for (int k = N - 1, l = j + 1; k >= 0; k--, l++)
    //                 if (B[l / 32] & (1 << (31 - (l % 32))))

    //                     ans += 1 << k;

    //             return ans - 1;
    //         }
    //         j += N;
    //         N = 0;
    //         currPos++;
    //     }
    //     else
    //         N++;
    // }
// }

ulong SamplePointers::size()
{
    return this->sz;
}