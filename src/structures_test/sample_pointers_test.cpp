#include "../../libs/structures_test/sample_pointers.h"

#define SAMPLE_POINTES_SIZE 10

void sample_pointers_test()
{
    const uint n = 10000;
    const uint k = 4;

    uint O[n];
    for (int i = 0; i < n; i++)
        scanf("%d", &O[i]);
    

    SamplePointers sp(n, k, O);
    for (int i = 0; i < n; i++)
        printf("%u\n", sp.read(i));

    return;
}

// void sample_pointers_test()
// {
//     const uint n = 3;
//     const uint k = 4;

//     uint arr[] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
//     uint O[SAMPLE_POINTES_SIZE] = {3, 1, 0};
//     uint BLOCKS[3] = { 2, 5, 0 };

//     SamplePointers sp(n, 4, O);
//     for (int i = 0; i < n; i++) {
//         printf("%u", sp.read(i));
//         if (i != n - 1) printf(", "); else puts("");
//     }

//     return;
// }