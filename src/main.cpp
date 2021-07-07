#include <bits/stdc++.h>

#include "../libs/compressed_array.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"

using namespace std;

int main() {

    int arr[10] = {2, 1, 9, 1, 7, 9, 1, 5, 9, 1};
    WaveletTree wt(arr, 10);

    // int n;
    // while(1) {
    //     scanf("%d", &n);
    //     printf("%d\n", wt.access(n));
    // }

    return 0;
}