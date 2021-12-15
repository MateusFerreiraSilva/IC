#include "../libs/test_csa.h"
#include "../libs/malloc_count.h"

uint* string_to_uint_seq(string str) {
    uint *seq = NULL;
    try {
        seq = (uint *)malloc(str.size() * sizeof(uint));
        if (seq == NULL)
            throw;

        for (int i = 0; i < str.size(); i++)
            seq[i] = 1 + (str[i] - '0' > 0 ? str[i] - '0' : 0);
   } catch (...) {
       printf("Error parsing string to uint sequence\n");
       return NULL;
   }

   return seq;
}

string uint_seq_to_string(uint *seq, uint seq_size) {
    string str = "";
    for (int i = 0; i < seq_size; i++)
        str.push_back(seq[i] > 0 ?  seq[i] + '0' : '$');

    return str;
}


void csa_test() {
    uint *aux = NULL;
    string seq = "bananaban$", pattern;
    aux = string_to_uint_seq(seq);
    malloc_count_print_status();
    CompactSuffixArray csa(seq.size(), aux);
    free(aux);
    malloc_count_print_status();

    // printf("Suffixes:\n");
    // for (int i = 0; i < seq.size(); i++) {
    //     uint *suffix = csa.get_suffix(i);
    //     string str_suffix = uint_seq_to_string(suffix, seq.size() - i);
    //     printf("%s\n", str_suffix.c_str());
    //     free(suffix);
    // }
    // puts("");
    
    while (printf("Enter a patter: "), cin >> pattern) {
        pattern.push_back('$');
        aux = string_to_uint_seq(pattern);
        for (int i = 0; i < pattern.size(); i++) aux[i] -= 1;

        // int ans = csa.get_sa(csa.findOne(aux, pattern.size()));
        // if (ans >= 0) printf("Pattern find at position %d\n", ans);
        // else printf("Pattern not found\n");
        // puts("");

        vector<uint> ans = csa.findAll(aux, pattern.size());
        if (!ans.empty()) {
            printf("Pattern find at positions:");
            for (int i = 0; i < ans.size(); i++)
                printf(" %u%c", ans[i], i != ans.size() - 1 ? ',' : '\n');
        } else
            printf("Pattern not found\n");
        puts("");
        free(aux);
    }

}