#include <bits/stdc++.h>
#include "../malloc_count.h"
using namespace std;

#ifndef ADJ_LIST_TEMPORAL_GRAPH
#define ADJ_LIST_TEMPORAL_GRAPH

class AdjListTemporalGraph {
    private:
        vector<vector<vector<uint>>> adj;
    public:
        AdjListTemporalGraph(uint v_num);
        ~AdjListTemporalGraph();
        void addEdge(uint u, uint v, uint t1, uint t2);
};

#endif