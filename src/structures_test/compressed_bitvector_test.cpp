#include "../../libs/structures_test/compressed_bitvector_test.h"

// void compressed_bitvector_test() {
//     int n = 10, b = 4;
//     unsigned arr[10] = {8, 2, 0, 6, 0, 10, 0, 11, 8, 1};
//     CompressedBitvector cb(4, 10, arr);

//     // ################# ONES #####################

//     unsigned rank_test[10 * 4] = {0};
//     int c = 0;
//     for(int i = 0; i < n; i++) {
//         for (int j = 0; j < b; j++)
//         {
//             unsigned idx = i * b + j;
//             if (idx > 0) rank_test[idx] = rank_test[idx - 1];
//             rank_test[idx] += arr[i] & (1 << (b - 1) - j) ? 1 : 0; 
//             printf("rank[%d] = %d, rank_teste[%d] =  %d\n", idx, cb.rank1(idx + 1), idx, rank_test[idx]);
//             if (cb.rank1(idx + 1) == rank_test[idx]) c++;
//         }
//     }
//     if(c == n * b) printf("OK\n");

//     int select_test[10 * 4];
//     memset(select_test, -1, sizeof select_test);
//     c = 0;
//     int r = 0;
//     for (int i = 0, j = 0; i < n * b; i++)
//     {
//         if (rank_test[i] > r)
//         {
//             select_test[j] = i + 1;
//             r++;
//             j++;
//         }
//     }

//     for (int i = 0; i < n * b; i++)
//     {
//         int select = cb.select1(i + 1);
//         printf("select[%d] =  %d, select_test[%d] = %d\n", i + 1, select, i + 1, select_test[i]);
//         if (select == select_test[i])
//             c++;
//     }
//     if (c == n * b)
//         printf("OK\n");


    // ################# ZERO #####################

    // unsigned rank_test[10 * 4] = {0};
    // int c = 0;
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < b; j++)
    //     {
    //         unsigned idx = i * b + j;
    //         if (idx > 0)
    //             rank_test[idx] = rank_test[idx - 1];
    //         rank_test[idx] += arr[i] & (1 << (b - 1) - j) ? 0 : 1;
    //         printf("rank[%d] = %d, rank_teste[%d] =  %d\n", idx, cb.rank0(idx + 1), idx, rank_test[idx]);
    //         if (cb.rank0(idx + 1) == rank_test[idx])
    //             c++;
    //     }
    // }
    // if (c == n * b)
    //     printf("OK\n");

    // int select_test[10 * 4];
    // memset(select_test, -1, sizeof select_test);
    // c = 0;
    // int r = 0;
    // for (int i = 0, j = 0; i < n * b; i++)
    // {
    //     if (rank_test[i] > r)
    //     {
    //         select_test[j] = i + 1;
    //         r++;
    //         j++;
    //     }
    // }

    // for (int i = 0; i < n * b; i++)
    // {
    //     int select = cb.select0(i + 1);
    //     printf("select[%d] =  %d, select_test[%d] = %d\n", i + 1, select, i + 1, select_test[i]);
    //     if (select == select_test[i])
    //         c++;
    // }
    // if (c == n * b)
    //     printf("OK\n");
// }

void compressed_bitvector_mem_test() {
    // cout << "Compact Bitvector\n";

    srand(time(NULL));

    const uint n = 10000 * 5;
    vector<bool>* bs = new vector<bool>(n);
    for (int i = 0; i < n; i++)
        bs->at(i) = (rand() % 100 + 1) <= 50;
    // cout << "Uncompact:\n";
    // malloc_count_print_status();
    const uint block_size = 32;
    const uint block_num = bs->size() / block_size + 1;
    CompressedBitvector bitvector(block_size, block_num, *bs);
    delete bs;
    // cout << "Compact:\n";
    // malloc_count_print_status();
}