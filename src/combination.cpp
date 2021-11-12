#include <bits/stdc++.h>
#include "../libs/combination.h"

using namespace std;

uint *memo;

uint fat(uint x)
{
    if (x == 1 || x == 0)
        return 1;
    else if (memo[x])
        return memo[x];
    return memo[x] = x * fat(x - 1);
}

uint comb(uint a, uint b)
{
    if (a < b)
        return 0;
    else if (a == b || b == 0)
        return 1;
    return fat(a) / (fat(a - b) * fat(b));
}

void freeComb(uint **Comb, uint x) {
    if(Comb != NULL) {
        for (int i = 0; i <= x; i++)
            free(Comb[i]);
        free(Comb);
    }
}

uint **precompComb(uint **Comb, uint x)
{
    try {
        Comb = (uint **) malloc((x + 1) * sizeof(uint *));
        if(Comb == NULL) throw;

        for (int i = 0; i <= x; i++) {
            Comb[i] = (uint *) malloc((x + 1) * sizeof(uint));
            if(Comb[i] == NULL) throw;
        }

        memo = (uint *) calloc((x + 1), sizeof(uint));
        if (memo == NULL) throw;

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