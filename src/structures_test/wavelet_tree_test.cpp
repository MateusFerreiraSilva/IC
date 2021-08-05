#include "../../libs/structures_test/wavelet_tree_test.h"

#define sz 9

unsigned rank_test[10][10];
set<int> s;

void calc_ranks_test(unsigned arr[], unsigned arrSize) {
    for(int i = 0; i < arrSize; i++)
        s.insert(arr[i]);

    for(auto it : s) {
        rank_test[it][0] = arr[0] == it;
        for(int i = 1; i < arrSize; i++)
            rank_test[it][i] = rank_test[it][i - 1] + (arr[i] == it);

        printf("rank_test %d: ", it);
        for(int i = 0; i < arrSize; i++)
            printf("%d%c", rank_test[it][i], i == arrSize - 1 ? '\n' : ' ');
    }

}

void wavelet_tree_test()
{
    unsigned arr[] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
    unsigned arrCopy[] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
    int arrSize = sizeof(arr) / sizeof(unsigned);
    WaveletTree wt(arr, arr + arrSize, "", NULL);

    int count = 0;
    for (int i = 1; i <= arrSize; i++) {
        unsigned x = wt.access(i);
        printf("%u ", x);
        if(x == arrCopy[i - 1]) count++;
    }
    if(count == arrSize) puts("OK");
    else puts("");

    calc_ranks_test(arrCopy, arrSize);
    printf("----TEST RANK----\n\n");
    int c;
    for (auto it : s)
    {
        c = 0;
        for (int i = 0; i <= arrSize; i++)
            if (wt.rank(it, i + 1) == rank_test[it][i])
                c++;
        if (c == arrSize)
            printf("rank_test(%u) OK\n", it);
    }

    while (1)
    {
        int c, i;
        scanf("%d%d", &c, &i);
        printf("rank-%u(%u) = %d\n", c, i, wt.rank(c, i));
    }

    // while (1)
    // {
    //     int c, x;
    //     scanf("%d%d", &c, &x);
    //     printf("%u\n", wt.select(c, x));
    // }
}

// #define N 10

// int main()
// {
//     unsigned arr[N] = {8, 2, 0, 6, 0, 10, 0, 11, 8, 1};
//     CompressedBitvector bitvector(4, N, arr);
    
//     for (int i = 0; i < N * 4; i++) {
//         printf("%u", bitvector.rank1(i + 1));
//         if((i + 1) % 4 == 0) printf("\n");
//         else printf(" ");
//     }

//     printf("\nrank zero\n\n");
//     for (int i = 0; i < N * 4; i++)
//     {
//         printf("%u", bitvector.rank0(i + 1));
//         if ((i + 1) % 4 == 0)
//             printf("\n");
//         else
//             printf(" ");
//     }
// }