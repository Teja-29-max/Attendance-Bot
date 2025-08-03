#include <bits/stdc++.h>
using namespace std;

const int A = 26;
const long long INF = 4e18; // Large sentinel for 64-bit cost values
const int INF_INT = 1e9;      // sentinel for piece counts (fits in 32-bit)

struct N {
    int n[A], l, L;
    N() : l(-1), L(0) { fill(begin(n), end(n), -1); }
};

class T {
public:
    explicit T(size_t R = 0) {
        v.reserve(R ? 2 * R : 256);
        v.emplace_back();
        z = 0;
    }

    void e(char c) {
        int x = c - 'a', y = (int)v.size();
        v.emplace_back();
        v[y].L = v[z].L + 1;
        int p = z;
        while (p != -1 && v[p].n[x] == -1) {
            v[p].n[x] = y;
            p = v[p].l;
        }
        if (p == -1) {
            v[y].l = 0;
        } else {
            int q = v[p].n[x];
            if (v[p].L + 1 == v[q].L) {
                v[y].l = q;
            } else {
                int c = (int)v.size();
                v.push_back(v[q]);
                v[c].L = v[p].L + 1;
                while (p != -1 && v[p].n[x] == q) {
                    v[p].n[x] = c;
                    p = v[p].l;
                }
                v[q].l = c;
                v[y].l = c;
            }
        }
        z = y;
    }

    inline int s(int u, char c) const {
        return v[u].n[c - 'a'];
    }

private:
    vector<N> v;
    int z;

public:
    const N &operator[](int i) const { return v[i]; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string x, y;
    if (!(cin >> x)) return 0;
    cin >> y;
    long long s, r;
    cin >> s >> r;

    int n = (int)x.size();

    T a(y.size()), b(y.size());
    for (char c : y) a.e(c);
    reverse(y.begin(), y.end());
    for (char c : y) b.e(c);

    vector<int> pieces(n + 1, INF_INT);      // minimum number of substrings
    vector<long long> cost(n + 1, INF);      // minimum cost for that # substrings
    pieces[0] = 0;
    cost[0] = 0;

    for (int i = 0; i < n; ++i) {
        if (pieces[i] == INF_INT) continue;
        for (int t = 0; t < 2; ++t) {
            const T &m = (t == 0 ? a : b);
            int u = 0;
            for (int j = i; j < n; ++j) {
                u = m.s(u, x[j]);
                if (u == -1) break;
                int k = j + 1;
                int np = pieces[i] + 1;
                long long nc = cost[i] + (t == 0 ? s : r);
                if (np < pieces[k] || (np == pieces[k] && nc < cost[k])) {
                    pieces[k] = np;
                    cost[k] = nc;
                }
            }
        }
    }

    if (pieces[n] == INF_INT) {
        cout << "Impossible\n";
    } else {
        cout << cost[n] << '\n';
    }
    return 0;
}