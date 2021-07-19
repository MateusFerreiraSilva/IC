#include <bits/stdc++.h>
using namespace std;

class Bitvectorr
{
public:
    vector<bool> b;
    Bitvectorr(int n);
    int access(int i);
    int rank1(int i);
    int rank0(int i);
    int select1(int i);
    int select0(int i);
};

Bitvectorr::Bitvectorr(int n) {
    b.assign(n, 0);
}
int Bitvectorr::access(int i) {
    return b[i];
}
int Bitvectorr::rank1(int i) {
    int acc = 0;
    for (int j = 0; j < i; j++)
        if(b[j])
            acc++;
    return acc;
}
int Bitvectorr::rank0(int i) {
    i - this->rank1(i);
}
int Bitvectorr::select1(int i) {
    int acc = 0;
    for (int j = 0; j < b.size(); j++)
    {
        if(acc == i) return j;
        if(b[j]) acc++;
    }
    return -1;
}
int Bitvectorr::select0(int i) {
    int acc = 0;
    for (int j = 0; j < b.size(); j++)
    {
        if (acc == i)
            return j;
        if (b[j] == 0)
            acc++;
    }
    return -1;
}

int main() {
    int sz = 10;
    Bitvectorr x(sz);
    for (int i = 0; i < sz; i++)
        if(i % 2 != 0) x.b[i] = 1; 
    
    for (int i = 0; i < x.b.size(); i++)
    {
        cout << x.access(i) << " ";
    } cout << endl;

    for (int i = 0; i < x.b.size(); i++)
    {
        cout << x.rank1(i) << " ";
    }
    cout << endl;

    for (int i = 0; i < x.b.size(); i++)
    {
        cout << x.select1(i) << " ";
    }
    cout << endl;

    return 0;
}