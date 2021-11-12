#include <bits/stdc++.h>
#include "../libs/csa.h"
using namespace std;

#define END numeric_limits<uint>::max()

CompactSuffixArray::CompactSuffixArray(uint sequence_size, uint *sequence) {
    try {
        this->sequence_size = sequence_size; 
        this->sequence = new WaveletTreeInterface(sequence, sequence +  sequence_size);
        uint *suffix_array = (uint *)malloc(sequence_size * sizeof(uint));

        if (sequence_size <= 0 || sequence == NULL || suffix_array == NULL) throw;

        for (int i = 0; i < sequence_size; i++)
            suffix_array[i] =  i;

        sort_suffix_array(suffix_array);

        free(suffix_array);
    } catch (...) {
        printf("Error on CSA creation\n");
    }
}

CompactSuffixArray::~CompactSuffixArray() {}

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
            psi[i] = END; // maybe be best not store END in psi
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

void CompactSuffixArray::sort_suffix_array(uint *suffix_array)
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

    this->psi = new CompactPsi(psi);

    // testes
    uint x;
    // for (int i = 0; i < sequence_size; i++)
    //     x = get_psi(i);
    for (int i = 0; i < sequence_size; i++)
        printf("%d%c", get_sa(i), i != sequence_size - 1 ? ' ' : '\n');
   
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

    the $ ensures comparation even if pattern_size is greater than text_size
*/
int is_prefix(uint *pattern, uint pattern_size, uint *text, uint text_size)
{
    for (int i = 0; i < pattern_size - 1; i++)
    {
        if (pattern[i] < text[i])
            return -1;
        else if (pattern[i] > text[i])
            return 1;
    }

    return 0;
}

/*
    return the SA idx where the pattern was found first
*/
int CompactSuffixArray::findOne(uint *pattern, uint pattern_size) {
    uint lo = 0, hi = sequence_size - 1, mid;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        uint sa = get_sa(mid);
        int aux = is_prefix(
            pattern, pattern_size, get_suffix(sa),
            sequence_size - sa
        );

        if (aux == 0) return mid;
        else if (aux < 0) hi = mid - 1;
        else lo = mid + 1; 
    }

    return -1;
}

/*
    walk up and down in the SA checking the pattern

    return sorted array with all the positions where the pattern was found
*/
vector<uint> CompactSuffixArray::findAll(uint *pattern, uint pattern_size) {
    int start = findOne(pattern, pattern_size);
    if (start == -1) return vector<uint>();

    priority_queue<int> pq;

    // go up
    for (int i = start; i >= 0; i--)
    {
        int sa = get_sa(i);
        if (is_prefix(pattern, pattern_size, get_suffix(sa), sequence_size - sa) == 0)
            pq.push(-sa);
        else break;
    }
    
    // go down
    for (int i = start + 1; i < sequence_size; i++)
    {
        int sa = get_sa(i);
        if (is_prefix(pattern, pattern_size, get_suffix(sa), sequence_size - sa) == 0)
            pq.push(-sa);
        else break;
    }

    vector<uint> positions(pq.size());
    int idx = 0;
    while (!pq.empty())
    {
        positions[idx++] = -pq.top();
        pq.pop();
    }

    return positions;
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

/*
PSI tem várias sequencias crescentes
precisamos armazenar as posições de inicio de cada sequencia

Psi(x):
    seq = posicoes_subsequencia.find(x) -> retorna a sequencia a qual a posicao x esta contida
    seq.ef_get(x)

    ef_get(k):
        return this.bitvector.select1(k) - k
*/

uint CompactSuffixArray::get_sa(uint x)
{
    uint hops = 0;
    uint psi_x = get_psi(x);
    while (psi_x != END)  {
        hops++;
        x = psi_x;
        psi_x = get_psi(x);
    }
    return sequence_size - hops - 1;
}

uint CompactSuffixArray::get_psi(uint x) {
    if (x == 0) return END;

    /*TODO binary search subsequences_idx*/
    uint seq_idx = 0;
    for (int i = 0; i < psi->subsequences_qtt; i++) { // get the sequence of the element x
        if (x >= psi->subsequences_idx[i])
            seq_idx = i;
        else break;
    }

    const uint offset = seq_idx > 0 ? psi->subsequences_idx[seq_idx] - 1 : 0;
    x -= offset;
    return psi->subsequences[seq_idx]->select1(x) - x;
}

CompactPsi::CompactPsi(vector<uint> dummy_psi) {
   try {
        vector<uint> positions; // store the positon of the start of each increasing subsequence
        // position 0 will allways have the END caracter

        if (dummy_psi.size() >= 2) positions.push_back(1);

        for (int i = 2; i < dummy_psi.size(); i++)
            if (dummy_psi[i] < dummy_psi[i - 1])
                positions.push_back(i);

        subsequences_qtt = positions.size();
        subsequences_idx = (uint*) malloc(subsequences_qtt * sizeof(uint));
        if (subsequences_idx == NULL) throw;

        for (int i = 0; i < subsequences_qtt; i++)
            subsequences_idx[i] = positions[i];
    
        subsequences = (CompressedBitvector**) malloc(subsequences_qtt * sizeof(CompressedBitvector*));
        if (subsequences == NULL) throw;

        for (int i = 0; i < subsequences_qtt; i++)
        {
            const uint seq_start = subsequences_idx[i];
            const uint seq_end = i + 1 < subsequences_qtt ? subsequences_idx[i + 1] : dummy_psi.size();
            uint max_n = 0;
            for (int j = seq_start; j < seq_end; j++) max_n = max(max_n, dummy_psi[j]);
            max_n += 1;
            vector<bool> elias_fano(max_n + max_n / 2, false);
            for (int idx = 0, j = seq_start; idx < seq_end - seq_start; idx++, j++) {
                const uint val = dummy_psi[j];
                elias_fano[idx + val] = 1;
            }

            subsequences[i] = new CompressedBitvector(BITVECTOR_BLOCK_SIZE, max_n + max_n / 2, elias_fano);
        }

   } catch (...) {
       printf("Error creating PSI\n");
   }
}

CompactPsi::~CompactPsi() {
    if (subsequences_idx != NULL) free(subsequences_idx);
    if (subsequences != NULL) delete[] subsequences;
}