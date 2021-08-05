#include <bits/stdc++.h>

#include "../libs/bitarray.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"

using namespace std;

#define sz 9

int main()
{
    unsigned arr[] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
    unsigned arrCopy[] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
    int arrSize = sizeof(arr) / sizeof(unsigned);
    WaveletTree wt(arr, arr + arrSize, "", NULL);

    int count = 0;
    for (int i = 1; i <= arrSize; i++) {
        unsigned x = wt.access(i);
        printf("%u ", x);
        if(x == arrCopy[i]) count++;
    }
    if(count == arrSize) puts("OK");
    else puts("");

    // while (1)
    // {
    //     int c, i;
    //     scanf("%d%d", &c, &i);
    //     printf("rank-%u(%u) = %d\n", c, i, wt.rank(c, i));
    // }

    while (1)
    {
        int c, x;
        scanf("%d%d", &c, &x);
        printf("%u\n", wt.select(c, x));
    }
}

// #define N 10

// int main()
// {
//     unsigned arr[N] = {8, 2, 0, 6, 0, 10, 0, 11, 8, 1};
//     CompressedBitvector bitvector(4, N, arr);
    
//     for (int i = 0; i < N * 4; i++) {
//         printf("%u", bitvector.rank1(i + 1));
//         if((i + 1) % 4 == 0) printf("\n");
//         else printf(" ");
//     }

//     printf("\nrank zero\n\n");
//     for (int i = 0; i < N * 4; i++)
//     {
//         printf("%u", bitvector.rank0(i + 1));
//         if ((i + 1) % 4 == 0)
//             printf("\n");
//         else
//             printf(" ");
//     }
// }