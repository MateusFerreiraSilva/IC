#include <bits/stdc++.h>
#include "../libs/combination.h"

using namespace std;

unsigned *memo;

unsigned fat(unsigned x)
{
    if (x == 1 || x == 0)
        return 1;
    else if (memo[x])
        return memo[x];
    return memo[x] = x * fat(x - 1);
}

unsigned comb(unsigned a, unsigned b)
{
    if (a < b)
        return 0;
    else if (a == b)
        return 1;
    return fat(a) / (fat(a - b) * fat(b));
}

unsigned **precompComb(unsigned **Comb, unsigned x)
{
    try {
        Comb = (unsigned **)malloc((x + 1) * sizeof(unsigned *));
        if(Comb == NULL) throw;
        for (int i = 0; i <= x; i++)
            Comb[i] = (unsigned *)malloc((x + 1) * sizeof(unsigned));

        memo = (unsigned *)malloc((x + 1) * sizeof(unsigned));
        if (memo == NULL) throw;
        memset(memo, 0, sizeof(memo));
        for (int i = x; i >= 0; i--)
            for (int j = x; j >= 0; j--)
                Comb[i][j] = comb(i, j);
        free(memo);
    } catch (...) {
        printf("Malloc error on precompComb\n");
        return NULL;
    }

    return Comb;
}