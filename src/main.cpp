#include <bits/stdc++.h>

#include "../libs/bitarray.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"

using namespace std;

#define sz 9

pair<unsigned, unsigned> findLimits(unsigned *seq, unsigned seq_size)
{
    unsigned seqMin, seqMax;
    if (seq_size >= 1)
        seqMin = seqMax = seq[0];
    for (int i = 1; i < seq_size; i++)
    {
        seqMin = min(seqMin, seq[i]);
        seqMax = max(seqMax, seq[i]);
    }
    return make_pair(seqMin, seqMax);
}

// int main()
// {
//     unsigned arr[] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
//     unsigned arrCopy[] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
//     int arrSize = sizeof(arr) / sizeof(unsigned);
//     pair<unsigned, unsigned> limits = findLimits(arr, arrSize);
//     WaveletTree wt(arr, arr + arrSize, limits.first, limits.second, "", NULL);

//     int count = 0;
//     for (int i = 1; i <= arrSize; i++) {
//         unsigned x = wt.access(i);
//         printf("%u ", x);
//         if(x == arrCopy[i]) count++;
//     }
//     if(count == arrSize) puts("OK");
//     else puts("");

//     // while (1)
//     // {
//     //     int c, i;
//     //     scanf("%d%d", &c, &i);
//     //     printf("rank-%u(%u) = %d\n", c, i, wt.rank(c, i));
//     // }

//     while (1)
//     {
//         int c, x;
//         scanf("%d%d", &c, &x);
//         printf("%u\n", wt.select(c, x));
//     }
// }

#define N 10

int main()
{
    unsigned arr[N] = {8, 2, 0, 6, 0, 10, 0, 7, 8, 1};
    CompressedBitvector bitvector(4, N, arr); 
}