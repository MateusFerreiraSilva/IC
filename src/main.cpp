#include <bits/stdc++.h>

#include "../libs/compressed_array.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
// #include "../libs/wavelet_tree.h"

using namespace std;

#define sz 9

// int main() {
//     int arr[sz] = {2, 1, 9, 3, 8, 4, 7, 5, 6};
//     WaveletTree wt(arr, sz);

//     int n;
//     for (int i = 0; i < sz; i++)
//     {
//         wt.access(i);
//         cout << wt.access(i) << " ";
//     }
//     cout << endl;

//     return 0;
// }

int main()
{
    unsigned B[10] = {8, 2, 0, 6, 0, 10, 0, 13, 8, 1};

    CompressedBitVector cbv(4, 10, B);
    cbv.print();

    printf("B - size: %lu bytes\n", (long unsigned)(sizeof(B) / sizeof(unsigned)));
    printf("Compressed B - size: %lu bytes\n", cbv.size());

    // int x;
    // while (1)
    // {
    //     scanf("%d", &x);
    //     printf("rank(%d) = %d\n", x, cbv.rank1(x));
    // }

    int x;
    while (1)
    {
        scanf("%d", &x);
        printf("select(%d) = %d\n", x, cbv.select0(x));
    }

    return 0;
}