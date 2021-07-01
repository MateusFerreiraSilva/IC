#include <bits/stdc++.h>
#include "suffix_array.h"

#define V 10

using namespace std;

void addEdge(string &str, char u, char v, char tBegin, char tEnd)
{
    u = u - 'a' + 1;
    v = v - 'a' + V + 1;
    tBegin = tBegin + 2 * V + 1;
    tEnd = tEnd + 3 * V + 1;
    str.push_back(u);
    str.push_back(v);
    str.push_back(tBegin);
    str.push_back(tEnd);
}

class Graph
{
public:
    Suffix_Array *csa;

    Graph(string str)
    {
        csa = new Suffix_Array(str);
    }

    bool has_edge(char u, char v)
    {
        u = u - 'a' + 1;
        v = v - 'a' + V + 1;

        string pat;
        pat.push_back(u);
        pat.push_back(v);

        int ans = 0;

        ans = csa->search(pat);

        return ans;
    }

    bool has_edge(char u, char v, int tBegin, int tEnd)
    {
        u = u - 'a' + 1;
        v = v - 'a' + V + 1;
        tBegin = tBegin + 2 * V + 1;
        tEnd = tEnd + 3 * V + 1;

        string pat;
        pat.push_back(u);
        pat.push_back(v);
        pat.push_back(tBegin);
        pat.push_back(tEnd);

        int ans = 0;

        ans = csa->search(pat);

        return ans;
    }
};

int main()
{
    string str = "";
    addEdge(str, 'a', 'b', 1, 3);
    addEdge(str, 'a', 'd', 2, 3);
    addEdge(str, 'a', 'd', 4, 6);
    addEdge(str, 'b', 'c', 3, 6);
    addEdge(str, 'b', 'e', 3, 5);
    addEdge(str, 'd', 'b', 0, 6);
    addEdge(str, 'e', 'd', 3, 5);

    Graph g(str);
    char u, v;
    int t1, t2;
    while (1)
    {
        int x;
        // cin >> u >> v;
        // x = g.has_edge(u, v);

        cin >> u >> v >> t1 >> t2;
        x = g.has_edge(u, v, t1, t2);
        if (x)
            cout << "YES\n";
        else
            cout << "NO\n";
    }

    return 0;
}