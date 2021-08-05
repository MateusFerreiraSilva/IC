#include <bits/stdc++.h>

#include "../libs/bitarray.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"
#include "../libs/tests.h"

using namespace std;

#define sz 9

int main()
{
    wavelet_tree_test();
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