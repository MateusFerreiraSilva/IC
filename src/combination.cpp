#include <bits/stdc++.h>
#include "../libs/combination.h"
#include "../libs/memprofile.h"


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

void Combination::free(uint **Comb, uint x) {
    if(Comb != NULL) {
        for (uint i = 0; i <= x; i++)
            if(Comb[i] != NULL) delete[] Comb[i];
        delete[] Comb;
    }
}

uint** Combination::precomp(uint x) {
    const uint size_aux = x + 1;
    uint **Comb = (uint **) new uint*[size_aux];

    for (uint i = 0; i <= x; i++)
        Comb[i] = (uint *) new uint[size_aux];

    memo = (uint*) new uint[size_aux];
    memset(memo, 0, size_aux * sizeof(uint));

    for (int i = x; i >= 0; i--)
        for (int j = x; j >= 0; j--)
            Comb[i][j] = comb(i, j);

    delete[] memo;

    cout << "comb: " << size(x) << endl;
    // malloc_count_print_status();
    
    return Comb;
}

uint Combination::size(uint x) {
    x++;
    return x * x * sizeof(uint*) + x * sizeof(uint*);
}