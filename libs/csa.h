#include <bits/stdc++.h>
#include "../libs/wavelet_tree.h"

using namespace std;

#ifndef CSA
#define CSA

class Suffix {
    public:
        vector<uint> suff;
        uint idx;
};

class CompactPsi {
    public:
        static const uint BITVECTOR_BLOCK_SIZE = 4;

        uint *subsequences_idx, subsequences_qtt;
        CompressedBitvector **subsequences;

        CompactPsi(vector<uint> dummy_psy);
        ~CompactPsi();
};

class CompactSuffixArray {
public:
    uint sequence_size;
    WaveletTreeInterface *sequence;
    CompactPsi *psi;

    CompactSuffixArray(uint sequence_size, uint *sequence);
    ~CompactSuffixArray();
    void sort_suffix_array(uint *suffix_array);
    uint *get_suffix(uint idx);
    uint get_sa(uint x);
    uint get_psi(uint x);

    /*
        return the starting position of the suffix or -1 case not found
    */
    int findOne(uint *pattern, uint pattern_size);
    vector<uint> findAll(uint *pattern, uint pattern_size);

    void print_suffix_array_info(vector<Suffix> suffixes, vector<uint> psi);
};

#endif