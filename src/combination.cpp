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
        for (uint i = 0; i <= x; i++)
            if(Comb[i] != NULL) delete[] Comb[i];
        delete[] Comb;
    }
}

uint **precompComb(uint **Comb, uint x)
{
    try {
        const uint size_aux = x + 1;
        Comb = (uint **) new uint*[size_aux];
        if(Comb == NULL) throw;

        for (uint i = 0; i <= x; i++) {
            Comb[i] = (uint *) new uint[size_aux];
            if(Comb[i] == NULL) throw;
        }

        // memo = (uint *) calloc((x + 1), sizeof(uint));
        memo = (uint*) new uint[size_aux];
        memset(memo, 0, size_aux * sizeof(uint));

        if (memo == NULL) throw;

        for (int i = x; i >= 0; i--)
            for (int j = x; j >= 0; j--)
                Comb[i][j] = comb(i, j);

        // free(memo);
        delete[] memo;

    } catch (...) {
        printf("Malloc error on precompComb\n");
        return NULL;
    }

    return Comb;
}