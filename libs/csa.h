#include <bits/stdc++.h>
#include "../libs/wavelet_tree.h"

using namespace std;

#ifndef CSA
#define CSA

class CompactSuffixArray {
public:
    uint* sequence;
    uint sequence_size;
    uint* suffix_array;

    CompactSuffixArray(uint sequence_size, uint *sequence);
    void sort_suffix_array();
    int find(uint *pattern);
};

#endif