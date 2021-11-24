#include <bits/stdc++.h>
#include "../malloc_count.h"
using namespace std;

#ifndef EDGE_LOG
#define EDGE_LOG

class EdgeLog
{
private:
    vector<vector<uint>> log;

public:
    EdgeLog();
    ~EdgeLog();
    void addEdge(uint u, uint v, uint t1, uint t2);
};

#endif