#include "../../libs/structures_test/csa_test.h"

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

void csa_test() {
    uint *aux = NULL;
    string seq = "bananaban$", pattern;
    aux = string_to_uint_seq(seq);
    CompactSuffixArray csa(seq.size(), aux);
    free(aux);

    while (cin >> pattern) {
        aux = string_to_uint_seq(pattern);
        int ans = csa.find(aux);
        free(aux);
    }

}