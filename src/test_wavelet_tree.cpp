#include "../libs/test_wavelet_tree.h"

#define sz 9

uint rank_test[10][10], select_test[10][10];
set<int> s;

void OK_TEST(int OK, int size) {
    if (OK == size) printf("OK\n");
}

void calc_rank_test(uint arr[], uint arrSize) {
    for(auto it : s) {
        rank_test[it][0] = arr[0] == it;
        for(int i = 1; i < arrSize; i++)
            rank_test[it][i] = rank_test[it][i - 1] + (arr[i] == it);

        printf("rank_test %d: ", it);
        for(int i = 0; i < arrSize; i++)
            printf("%d%c", rank_test[it][i], i == arrSize - 1 ? '\n' : ' ');
    }

}

void calc_select_test(uint arr[], uint arrSize)
{
    memset(select_test, 0, sizeof(select_test));
    for (auto it : s)
    {
        int rank = 0;
        for (int i = 1; i <= arrSize; i++)
            if(arr[i] == it) select_test[it][++rank] = i;
        

        printf("select_test %d: ", it);
        for(int i = 0; i < arrSize; i++)
            if (select_test[it][i])
                printf("%d ", select_test[it][i]);
        puts("");
    }
}

void wavelet_tree_test()
{
    // uint arr[] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
    // uint arrCopy[] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
    uint arr[] = {2, 1, 4, 1, 3, 7, 1, 5, 2, 10, 1, 2, 7};
    uint arrCopy[] = {2, 1, 4, 1, 3, 7, 1, 5, 2, 10, 1, 2, 7};
    // uint arr[] = {4, 4, 5};
    // uint arrCopy[] = {4, 4, 5};
    int arrSize = sizeof(arr) / sizeof(uint);
    WaveletTreeInterface wt(arrCopy, arrCopy + arrSize);

    int c = 0, OK;
    for (int i = 0; i < arrSize; i++) s.insert(arr[i]); // set to know the element in the wavelet tree

    printf("----TEST ACCESS----\n\n");
    OK = 0;
    for (int i = 1; i <= arrSize; i++) {
        uint x = wt.access(i);
        printf("%u%c", x, i == arrSize ? '\n': ' ');
        if(x == arr[i - 1]) {
            OK++;
            // printf("access_test(%u) OK\n", i);
        }
    }
    OK_TEST(OK, arrSize);

    // testes precisam corrigidos
    // os testes utilizam arrSize e arrCopy, nao faz sentido para o rank

    printf("\n----TEST RANK----\n\n");
    calc_rank_test(arr, arrSize);
    OK = 0;
    for (auto it : s)
    {
        c = 0;
        for (int i = 0; i < arrSize; i++) {
            uint x = wt.rank(it, i + 1);
            printf("rank(%u, %u): %u\n", it, i + 1, x);
            if (x == rank_test[it][i + 1])
                c++;
        }
        if (c == arrSize) {
            printf("rank_test(%u) OK\n", it);
            OK++;
        }
    }
    OK_TEST(OK, arrSize);

    printf("----TEST SELECT----\n\n");
    // calc_select_test(arr, arrSize);
    // OK = 0;
    // bool flag = true;
    // for (auto it : s)
    // {
    //     c = 0;
    //     for (int i = 0; i < arrSize; i++)
    //         if(arr[i] == it)
    //         if (wt.select(it, i + 1) == select_test[it][i + 1])
    //             c++;
    //     if (c == arrSize) {
    //         if (select_test)
    //             printf("select_test(%u) OK\n", it);
    //         else flag = false;
    //     }
    // }
    // if(flag) OK = arrSize;
    // OK_TEST(OK, arrSize);

    while (1)
    {
        uint c, x;
        scanf("%u%u", &c, &x);
        printf("%u\n", wt.select(c, x));
    }

    // printf("Size: %lu bytes\n", wt.size());
}

// #define N 10

// int main()
// {
//     uint arr[N] = {8, 2, 0, 6, 0, 10, 0, 11, 8, 1};
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