#include <bits/stdc++.h>

using namespace std;

#ifndef COMBINATION
#define COMBINATION

class Combination {
    public:
        static uint** precomp(uint x);
        static void free(uint **Comb, uint x);
        static uint size(uint x);
};

#endif