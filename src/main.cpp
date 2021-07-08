#include <bits/stdc++.h>

#include "../libs/compressed_array.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"

using namespace std;

int main() {

    int arr[10] = {2, 1, 9, 3, 8, 4, 7, 5, 9, 6};
    WaveletTree wt(arr, 10);

    int n;
    for (int i = 0; i < 10; i++)
    {
        wt.access(i);
        cout << wt.access(i) << " ";
    }
    cout << endl;

    return 0;
}