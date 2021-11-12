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

class PsiSubsequence {
    public:
        static const uint BITVECTOR_BLOCK_SIZE = 4;
        static const uint ALPHABET_SIZE = 1024;

        uint avg_space_between; 
        CompressedBitvector *quotients = NULL; // Elias-Fano bitvector
        Bitarray *remainders = NULL;

        PsiSubsequence(uint seq_size);
        ~PsiSubsequence();
};

/*
    Psi is the successor array
    Psi stores increasing subsequences with Elias-Fano and quotienting
*/
class CompactPsi {
    public:
        uint *subsequences_idx = NULL, subsequences_qtt = 0;
        PsiSubsequence **subsequences = NULL;

        CompactPsi(uint *dummy_psy, uint dummy_psi_size);
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
    uint* get_suffix(uint idx);
    uint get_sa(uint x);
    uint get_psi(uint x);

    /*
        return the starting position of the suffix or -1 case not found
    */
    int SA_binary_search(uint *pattern, uint pattern_size);
    vector<uint> find(uint *pattern, uint pattern_size);

    void print_suffix_array_info(vector<Suffix> suffixes, vector<uint> psi);
};

#endif