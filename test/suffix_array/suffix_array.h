#include <bits/stdc++.h>
using namespace std;

int prefixCheck(string pat, string str)
{
    // if (pat.size() < str.size())
    //     return -1;
    for (int i = 0; i < pat.size(); i++)
    {
        if (pat[i] == str[i])
            continue;
        else if (pat[i] < str[i])
            return -1;
        return 1;
    }
    return 0;
}

class Suffix
{
public:
    int index;
    string suff;
};

bool cmp(Suffix a, Suffix b)
{
    return a.suff < b.suff;
}
class Suffix_Array
{
public:
    string str;
    vector<Suffix> suffixes;

    Suffix_Array(string str)
    {
        this->str = str;

        suffixes.resize(str.size() - 1);
        for (int i = 1; i < str.size(); i++)
        {
            suffixes[i - 1].suff = str.substr(i, str.size() - i);
            suffixes[i - 1].index = i - 1;
        }

        sort(suffixes.begin(), suffixes.end(), cmp);

        // for (int i = 0; i < suffixes.size(); i++)
        //     printf("[%d] %s\n", suffixes[i].index, suffixes[i].suff.c_str());
    }

    int search(string pat)
    {
        int l = 0, r = suffixes.size() - 1;
        int m = pat.size();
        while (l <= r)
        {
            int mid = l + (r - l) / 2;
            int res = prefixCheck(pat, suffixes[mid].suff);

            if (res == 0)
            {
                int up, down, aux;
                up = down = aux = mid;
                while (--aux >= 0)
                {
                    if (prefixCheck(pat, suffixes[aux].suff) == 0)
                        up = aux;
                    else
                        break;
                }

                aux = mid;
                while (++aux <= r)
                {
                    if (prefixCheck(pat, suffixes[aux].suff) == 0)
                        down = aux;
                    else
                        break;
                }

                // for (int i = up; i <= down; i++)
                //     cout << suffixes[i].suff << endl;

                return 1;
            }

            if (res <= 0)
                r = mid - 1;
            else
                l = mid + 1;
        }

        if (prefixCheck(pat, this->str) == 0)
            return 1;

        return 0;
    }
};