#include <bits/stdc++.h>

#include "../libs/compressed_array.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"

using namespace std;

#define sz 9

int main() {
    int arr[sz] = {2, 1, 9, 3, 8, 4, 7, 5, 6};
    WaveletTree wt(arr, sz);

    int n;
    for (int i = 0; i < sz; i++)
    {
        wt.access(i);
        cout << wt.access(i) << " ";
    }
    cout << endl;

    return 0;
}