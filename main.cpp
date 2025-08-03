#include <bits/stdc++.h>
using namespace std;

const int A = 26;
const long long INF = 4e18; // Large sentinel for 64-bit cost values

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

    vector<int> p(n + 1, (int)INF);          // min pieces
    vector<long long> cst(n + 1, INF);       // min cost for corresponding p
    p[0] = 0;
    cst[0] = 0;

    for (int i = 0; i < n; ++i) {
        if (p[i] == (int)INF) continue;
        for (int t = 0; t < 2; ++t) {
            const T &m = (t == 0 ? a : b);
            int u = 0;
            for (int j = i; j < n; ++j) {
                u = m.s(u, x[j]);
                if (u == -1) break;
                int k = j + 1;
                int np = p[i] + 1;
                long long nc = cst[i] + (t == 0 ? s : r);
                if (np < p[k] || (np == p[k] && nc < cst[k])) {
                    p[k] = np;
                    cst[k] = nc;
                }
            }
        }
    }

    if (p[n] == (int)INF) {
        cout << "Impossible\n";
    } else {
        cout << cst[n] << '\n';
    }
    return 0;
}