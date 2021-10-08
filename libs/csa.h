#include <bits/stdc++.h>
#include "../libs/wavelet_tree.h"

using namespace std;

#ifndef CSA
#define CSA

class CompactSuffixArray {
public:
    uint sequence_size;
    WaveletTreeInterface *sequence;
    uint *suffix_array;

    CompactSuffixArray(uint sequence_size, uint *sequence);
    void sort_suffix_array();
    uint *get_suffix(uint idx);
     int find(uint *pattern);
};

#endif