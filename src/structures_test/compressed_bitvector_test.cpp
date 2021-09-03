#include "../../libs/structures_test/compressed_bitvector_test.h"

void compressed_bitvector_test() {
    int n = 10, b = 4;
    unsigned arr[10] = {8, 2, 0, 6, 0, 10, 0, 11, 8, 1};
    CompressedBitvector cb(4, 10, arr);    

    // unsigned rank_test[10 * 4] = {0};
    // int c = 0;
    // for(int i = 0; i < n; i++) {
    //     for (int j = 0; j < b; j++)
    //     {
    //         unsigned idx = i * b + j;
    //         if (idx > 0) rank_test[idx] = rank_test[idx - 1];
    //         rank_test[idx] += arr[i] & (1 << (b - 1) - j) ? 1 : 0; 
    //         printf("rank[%d] = %d, rank_teste[%d] =  %d\n", idx, cb.rank1(idx + 1), idx, rank_test[idx]);
    //         if (cb.rank1(idx + 1) == rank_test[idx]) c++;
    //     }
    // }
    // if(c == n * b) printf("OK\n");

    unsigned rank_test[10 * 4] = {0};
    int c = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < b; j++)
        {
            unsigned idx = i * b + j;
            if (idx > 0)
                rank_test[idx] = rank_test[idx - 1];
            rank_test[idx] += arr[i] & (1 << (b - 1) - j) ? 0 : 1;
            printf("rank[%d] = %d, rank_teste[%d] =  %d\n", idx, cb.rank0(idx + 1), idx, rank_test[idx]);
            if (cb.rank0(idx + 1) == rank_test[idx])
                c++;
        }
    }
    if (c == n * b)
        printf("OK\n");

    for (int i = 0; i < 40; i++)
    {
        printf("select(%d) =  %d\n", i + 1 , cb.select1(i + 1));
    }
    

}