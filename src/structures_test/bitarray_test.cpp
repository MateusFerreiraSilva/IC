#include "../../libs/structures_test/bitarray_test.h"

void bitarray_test() {

    // cout << "Bitarray\n";
    ifstream file;
    const uint n = 10000;
    // números entre 1 e 15, então só precisamos 4 bits
    const uint l = 4; 
    uint* arr = new uint[n];

    string str = "../../input/bitarray.txt";
    file.open(str);
    for (int i = 0; i < n; i++)
        file >> arr[i];
    file.close();
    // cout << "Uncompact:\n";
    malloc_count_print_status();

    Bitarray bitarray(l, n, arr);
    delete arr;
    // cout << "Compact:\n";
    malloc_count_print_status();
    return;
}