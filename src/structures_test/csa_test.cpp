#include "../../libs/structures_test/csa_test.h"

uint* string_to_uint_seq(string str) {
    uint *seq = NULL;
    try {
        seq = (uint *)malloc(str.size() * sizeof(uint));
        if (seq == NULL)
            throw;

        for (int i = 0; i < str.size(); i++)
            seq[i] = str[i] - '0' >=  0 ? str[i] - '0' : 0;
   } catch (...) {
       printf("Error parsing string to uint sequence\n");
       return NULL;
   }

   return seq;
}

void csa_test() {
    string seq = "bananaban$", pattern;
    CompactSuffixArray csa(seq.size(), string_to_uint_seq(seq));

    while (cin >> pattern) {
        int ans = csa.find(string_to_uint_seq(pattern)); 
    }

}