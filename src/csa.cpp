#include <bits/stdc++.h>
#include "../libs/csa.h"
using namespace std;

CompactSuffixArray::CompactSuffixArray(uint sequence_size, uint *sequence) {
    try {
        this->sequence_size = sequence_size;
        for (int i = 0; i < sequence_size; i++)
        {
            printf("%d ", sequence[i]);
        }
        puts("\n");
        
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

    // for (int i = 0; i < sequence_size; i++)
    //     printf("%c", sequence->access(i) == 1 ? '$' : sequence->access(i + 1) - 1 + '0');
    // puts("\n");
    

    // for (int i = 0; i < suffixes.size(); i++)
    // {
    //     for (int j = 0; j < suffixes[i].first.size(); j++)
    //         printf("%c", suffixes[i].first[j] == 1 ? '$' : suffixes[i].first[j] - 1 + '0');
    //     puts("");
    // }

    for (int i = 0; i < sequence_size; i++)
        suffix_array[i] = suffixes[i].second;
}

uint *CompactSuffixArray::get_suffix(uint idx)
{
    uint suffix_size = sequence_size - idx;
    try {
        uint *suffix = (uint*) malloc(suffix_size * sizeof(uint));
        if (suffix == NULL) throw;

        for (int i = idx; i < sequence_size; i++)
            suffix[i] = sequence->access(i + 1) - 1;
        
        return suffix;
    } catch (...) {
        printf("Error getting suffix %d\n", idx);
        return NULL;
    }
}

bool compare_sequences(uint *a, uint *b) {
}

int CompactSuffixArray::find(uint *pattern)
{
    uint lo = 0, hi = sequence_size, mid;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        compare_sequences(pattern, get_suffix(mid)); // incompleto
    }

    return 0;
}