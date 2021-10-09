#include <bits/stdc++.h>
#include "../libs/csa.h"
using namespace std;

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

bool compare_suffixes(pair<vector<uint>, uint> a, pair<vector<uint>, uint> b) {
    if (a.first.size() > b.first.size()) swap(a, b);
    for (int i = 0; i < a.first.size(); i++)
    {
        if (a.first[i] < b.first[i]) return true;
        else if (a.first[i] > b.first[i]) return false;
    }
    return true;
}

void CompactSuffixArray::sort_suffix_array()
{
    vector<pair<vector<uint>, uint>> suffixes(sequence_size);
    for (int i = 0; i < sequence_size; i++) {
        for (int j = i; j < sequence_size; j++)
            suffixes[i].first.push_back(sequence->access(j + 1)); 
        suffixes[i].second = i;
    }
        
    sort(suffixes.begin(), suffixes.end(), compare_suffixes);

    printf("Sequence:\n");
    for (int i = 0; i < sequence_size; i++)
        printf("%c", sequence->access(i) == 1 ? '$' : sequence->access(i + 1) - 1 + '0');
    puts("\n");
    

    printf("Sorted Suffix Array:\n");
    for (int i = 0; i < suffixes.size(); i++)
    {
        printf("[%d] ", i);
        for (int j = 0; j < suffixes[i].first.size(); j++)
            printf("%c", suffixes[i].first[j] == 1 ? '$' : suffixes[i].first[j] - 1 + '0');
        puts("");
    }
    puts("");

    for (int i = 0; i < sequence_size; i++)
        suffix_array[i] = suffixes[i].second;
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
    uint lo = 0, hi = sequence_size, mid;
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