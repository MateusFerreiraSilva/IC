#include "../../libs/structures_test/sample_pointers_test.h"

#define SAMPLE_POINTES_SIZE 10

void sample_pointers_mem_test()
{
    // cout << "Sample pointers\n";
    ifstream file;
    const uint n = 10000 * 5;
    const uint k = 4;
    uint* O = new uint[n];

    string str = "../../input/sample_pointers.txt";
    file.open(str);
    for (int i = 0; i < n; i++)
        file >> O[i];
    file.close();
    // cout << "Uncompact:\n";
    malloc_count_print_status();

    SamplePointers sp(n, k, O);
    delete O;
    // cout << "Compact:\n";
    malloc_count_print_status();
    // for (int i = 0; i < n; i++)
    //     printf("%u\n", sp.read(i));
    return;
}