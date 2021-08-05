#include <bits/stdc++.h>
#include "../libs/combination.h"

using namespace std;

int *memo;

int fat(int x)
{
    if (x == 1 || x == 0)
        return 1;
    else if (memo[x])
        return memo[x];
    return memo[x] = x * fat(x - 1);
}

int comb(int a, int b)
{
    if (a < b)
        return 0;
    else if (a == b)
        return 1;
    return fat(a) / (fat(a - b) * fat(b));
}

int **precompComb(int **K, int x)
{
    K = (int **)malloc((x + 1) * sizeof(int *));
    for (int i = 0; i <= x; i++)
        K[i] = (int *)malloc((x + 1) * sizeof(int));

    memo = (int *)malloc((x + 1) * sizeof(int));
    memset(memo, 0, sizeof(memo));
    for (int i = x; i >= 0; i--)
        for (int j = x; j >= 0; j--)
            K[i][j] = comb(i, j);

    free(memo);
    return K;
}