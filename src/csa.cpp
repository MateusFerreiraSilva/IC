#include <bits/stdc++.h>
#include "../libs/csa.h"
using namespace std;

CompactSuffixArray::CompactSuffixArray(uint sequence_size, uint *sequence) {
    try {
        this->sequence_size = sequence_size;
        this->sequence = sequence;
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
            suffixes[i].first.push_back(sequence[j]); 
        suffixes[i].second = i;
    }
        
    sort(suffixes.begin(), suffixes.end(), compare_suffixes);

    // string str = "bananaban$";
    // for (int i = 0; i < suffixes.size(); i++)
    // {
    //     for (int j = 0; j < suffixes[i].first.size(); j++)
    //         printf("%c", suffixes[i].first[j] + '0');
    //     puts("");
    // }

    for (int i = 0; i < sequence_size; i++)
        suffix_array[i] = suffixes[i].second;
}

int CompactSuffixArray::find(uint *pattern)
{
    return 0;
}