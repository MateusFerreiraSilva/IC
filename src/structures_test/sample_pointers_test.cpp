#include "../../libs/structures_test/sample_pointers_test.h"

#define SAMPLE_POINTES_SIZE 10

void sample_pointers_test()
{
    cout << "Sample pointers\n";
    ifstream file;
    const uint n = 10000;
    const uint k = 1;
    uint* O = new uint[n];

    file.open("../../input/sample_pointers.txt");
    for (int i = 0; i < n; i++)
        file >> O[i];
    file.close();
    cout << "Uncompact:\n";
    malloc_count_print_status();
    delete O;

    SamplePointers sp(n, k, O);
    cout << "Compact:\n";
    malloc_count_print_status();
    // for (int i = 0; i < n; i++)
    //     printf("%u\n", sp.read(i));
    return;
}