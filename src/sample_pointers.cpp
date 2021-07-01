#include <bits/stdc++.h>
#include "../libs/sample_pointers.h"
using namespace std;

int calcL(int x)
{
    return (2 * floor(log2(x + 1)) + 1);
}

SamplePointers::SamplePointers(int n, int k, int max)
{
    this->n = n;
    this->k = k;
    this->l = calcL(max);
    B = (unsigned *)calloc(l * n / w, sizeof(unsigned));
    P = (unsigned *)malloc(ceil(n / (float)k) * sizeof(unsigned));
    this->sz = (l * n / w + ceil(n / (float)k)) * sizeof(unsigned);
}

SamplePointers::SamplePointers(int n, int k, int max, unsigned *B) : SamplePointers(n, k, max)
{
    compress(B);
}

SamplePointers::~SamplePointers()
{
    free(B);
    free(P);
}

string SamplePointers::gammaEncode(int x)
{
    int N = 0;
    while ((1 << (N + 1)) <= x)
        N++;

    int aux = x - (1 << N);

    string str;
    for (int i = 0; i < N; i++)
        str.push_back('0');
    str.push_back('1');

    for (int i = N - 1; i >= 0; i--)
    {
        if (aux & (1 << i))
            str.push_back('1');
        else
            str.push_back('0');
    }

    return str;
}

void SamplePointers::toGamma(unsigned *arr)
{
    gammaCode = "";
    for (int i = 0; i < n; i++)
        gammaCode += gammaEncode(arr[i] + 1);
}

string SamplePointers::getGammaCode()
{
    return gammaCode;
}

int SamplePointers::findPos(int pos)
{
    int N = 0, currPos = 0;
    for (int i = 0; i < 32 * sizeof(B) / sizeof(unsigned); i++)
    {
        if (B[i / 32] & (1 << (31 - (i % 32))))
        {
            if (currPos == pos)
                return i - N;
            i += N;
            N = 0;
            currPos++;
        }
        else
            N++;
    }
}

void SamplePointers::compress(unsigned *arr)
{
    toGamma(arr);
    for (int i = 0; i < gammaCode.size(); i++)
        if (gammaCode[i] - '0')
            B[i / 32] |= (1 << (31 - (i % 32)));

    gammaCode.clear();

    int pos;
    for (int i = 0; i < ceil(n / (float)k); i++)
    {
        pos = i * k;
        P[i] = findPos(pos);
    }
}

unsigned SamplePointers::gammaDecode(int i)
{
    int aux = numeric_limits<int>::max(), idx;
    for (int j = 0; j < ceil(n / (float)k); j++)
    {
        if (abs(i - j * k) < aux && j * k <= i) // catch lower distance pointer of P to i
        {
            aux = abs(i - j * k);
            idx = j;
        }
    }

    int N = 0;
    int currPos = idx * k;
    for (int j = P[idx]; j < 32 * sizeof(B) / sizeof(unsigned); j++)
    {
        if (B[j / 32] & (1 << (31 - (j % 32))))
        {
            if (currPos == i)
            {
                unsigned ans;
                ans = 1 << N;
                for (int k = N - 1, l = j + 1; k >= 0; k--, l++)
                    if (B[l / 32] & (1 << (31 - (l % 32))))

                        ans += 1 << k;

                return ans - 1;
            }
            j += N;
            N = 0;
            currPos++;
        }
        else
            N++;
    }
}

unsigned long SamplePointers::size()
{
    return this->sz;
}