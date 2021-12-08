// #include <bits/stdc++.h>

// #include "../libs/bitarray.h"
// #include "../libs/sample_pointers.h"
// #include "../libs/compressed_bitvector.h"
// #include "../libs/wavelet_tree.h"
// #include "../libs/csa.h"
#include "../libs/tests.h"

// #include "../libs/uncompressed_structures/adjListTemporalGraph.h"

using namespace std;


int main() {
    // bitarray_mem_test();
    // sample_pointers_mem_test();
    // compressed_bitvector_mem_test();
    // wavelet_tree_mem_test();
    csa_test();    
}


// void adjList() {
//     uint n = 1000;
//     AdjListTemporalGraph graph(n);
//     graph.addEdge(0, 1, 0, 900);
//     graph.addEdge(0, 2, 10, 25);
//     graph.addEdge(3, 2, 500, 500);
//     graph.addEdge(6, 60, 12, 19);
//     graph.addEdge(6, 61, 13, 20);
//     graph.addEdge(6, 62, 14, 21);
// }

// int main() {
//    malloc_count_print_status();
//    adjList();
//    malloc_count_print_status();
// }