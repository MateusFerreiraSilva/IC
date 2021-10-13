#include <bits/stdc++.h>
#include "../libs/csa.h"
using namespace std;

#define END numeric_limits<uint>::max()

CompactSuffixArray::CompactSuffixArray(uint sequence_size, uint *sequence) {
    try {
        this->sequence_size = sequence_size; 
        this->sequence = new WaveletTreeInterface(sequence, sequence +  sequence_size);
        this->suffix_array = (uint *)malloc(sequence_size * sizeof(uint));

        if (sequence_size <= 0 || sequence == NULL || suffix_array == NULL) throw;

        for (int i = 0; i < sequence_size; i++)
            suffix_array[i] =  i;
        
        sort_suffix_array();
    } catch (...) {
        printf("Error on CSA creation\n");
    }
}

CompactSuffixArray::~CompactSuffixArray() {
    free(suffix_array);
}

bool compare_suffixes(Suffix a, Suffix b) {
    if (a.suff.size() > b.suff.size()) swap(a, b);
    for (int i = 0; i < a.suff.size(); i++)
    {
        if (a.suff[i] < b.suff[i]) return true;
        else if (a.suff[i] > b.suff[i]) return false;
    }
    return true;
}

vector<uint> build_psi(vector<Suffix> suffixes) {
    vector<uint> psi(suffixes.size());
    for (int i = 0; i < suffixes.size(); i++)
    {
        if (suffixes[i].idx == suffixes.size() - 1)
        {
            psi[i] = END;
            continue;
        }

        for (int j = 0; j < suffixes.size(); j++)
        {
            if (suffixes[j].idx == suffixes[i].idx + 1)
            {
                psi[i] = j;
                break;
            }
        }
    }
    return psi;
}

void CompactSuffixArray::sort_suffix_array()
{
    vector<Suffix> suffixes(sequence_size);
    for (int i = 0; i < sequence_size; i++) {
        for (int j = i; j < sequence_size; j++)
            suffixes[i].suff.push_back(sequence->access(j + 1)); 
        suffixes[i].idx = i;
    }
        
    sort(suffixes.begin(), suffixes.end(), compare_suffixes);

    vector<uint> psi = build_psi(suffixes);

    print_suffix_array_info(suffixes, psi);

    for (int i = 0; i < sequence_size; i++)
        suffix_array[i] = suffixes[i].idx;
}

uint *CompactSuffixArray::get_suffix(uint idx)
{
    uint suffix_size = sequence_size - idx;
    try {
        uint *suffix = (uint*) malloc(suffix_size * sizeof(uint));
        if (suffix == NULL) throw;

        for (int i = idx, j = 0; i < sequence_size; i++, j++)
            suffix[j] = sequence->access(i + 1) - 1;
        
        return suffix;
    } catch (...) {
        printf("Error getting suffix %d\n", idx);
        return NULL;
    }
}

/*
    verify i a is a prefix of b
    return -1 if a is less than B, 1 if a is greather than b or 0 if a is a prefix of b
*/
int is_prefix(uint *a, uint a_size, uint *b, uint b_size)
{
    uint min_size = min(a_size, b_size);

    for (int i = 0; i < min_size; i++)
    {
        if (a[i] < b[i]) return -1;
        else if (a[i] > b[i]) return 1;
    }

    return 0;
}

int CompactSuffixArray::find(uint *pattern, uint pattern_size) {
    uint lo = 0, hi = sequence_size - 1, mid;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        int aux = is_prefix(
            pattern, pattern_size, get_suffix(suffix_array[mid]),
            sequence_size - suffix_array[mid]
        );

        if (aux == 0) return suffix_array[mid];
        else if (aux < 0) hi = mid - 1;
        else lo = mid + 1; 
    }

    return -1;
}

void CompactSuffixArray::print_suffix_array_info(vector<Suffix> suffixes, vector<uint> psi) {
    printf("Sequence:\n");
    for (int i = 0; i < sequence_size; i++)
        printf("%c", sequence->access(i + 1) - 1 == 0 ? '$' : sequence->access(i + 1) - 1 + '0');
    puts("\n");

    printf("Sorted Suffix Array:\n");
    for (int i = 0; i < suffixes.size(); i++)
    {
        printf("[%d] ", i);
        for (int j = 0; j < suffixes[i].suff.size(); j++)
            printf("%c", suffixes[i].suff[j] == 1 ? '$' : suffixes[i].suff[j] - 1 + '0');
        puts("");
    }
    puts("");

    printf("Psi:\n");
    for (int i = 0; i < psi.size(); i++)
        printf("%d%c", psi[i], i != psi.size() - 1 ? ' ' : '\n');
}