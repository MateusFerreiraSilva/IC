#include "../../libs/uncompressed_structures/adjListTemporalGraph.h"

AdjListTemporalGraph::AdjListTemporalGraph(uint v_num) {
    adj.assign(v_num, vector<vector<uint>>());
    malloc_count_print_status();
}

AdjListTemporalGraph::~AdjListTemporalGraph() {}

void AdjListTemporalGraph::addEdge(uint u, uint v, uint t1, uint t2) {
    const vector<uint> x = {v, t1, t2};
    const vector<uint> y = {u, t1, t2};
    adj[u].push_back(x);
    adj[v].push_back(y);
}