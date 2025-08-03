#include <bits/stdc++.h>
using namespace std;

const int A = 26;
const int I = 1e9;

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
        int x = c - 'a', y = v.size();
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
                int c = v.size();
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

    void debug_print() const {
        cout << "Automaton states: " << v.size() << endl;
        for (int i = 0; i < v.size(); i++) {
            cout << "State " << i << ": L=" << v[i].L << ", l=" << v[i].l << endl;
            for (int j = 0; j < A; j++) {
                if (v[i].n[j] != -1) {
                    cout << "  " << char('a' + j) << " -> " << v[i].n[j] << endl;
                }
            }
        }
    }

private:
    vector<N> v;
    int z;

public:
    const N& operator[](int i) const { return v[i]; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string x, y;
    if (!(cin >> x)) return 0;
    cin >> y;
    int s, r;
    cin >> s >> r;

    cout << "Input: x=" << x << ", y=" << y << ", s=" << s << ", r=" << r << endl;

    int n = x.size();

    T a(y.size()), b(y.size());
    for (char c : y) a.e(c);
    cout << "Forward automaton:" << endl;
    a.debug_print();
    
    reverse(y.begin(), y.end());
    for (char c : y) b.e(c);
    cout << "Reverse automaton:" << endl;
    b.debug_print();

    vector<int> p(n + 1, I), c(n + 1, I);
    p[0] = 0;
    c[0] = 0;

    for (int i = 0; i < n; ++i) {
        if (p[i] == I) continue;
        cout << "Processing position " << i << " (p=" << p[i] << ", c=" << c[i] << ")" << endl;
        
        for (int t = 0; t < 2; ++t) {
            const T& m = (t == 0 ? a : b);
            cout << "  Using " << (t == 0 ? "forward" : "reverse") << " automaton" << endl;
            int u = 0;
            for (int j = i; j < n; ++j) {
                u = m.s(u, x[j]);
                cout << "    j=" << j << ", char=" << x[j] << ", state=" << u << endl;
                if (u == -1) break;
                int k = j + 1;
                int np = p[i] + 1;
                int nc = c[i] + (t == 0 ? s : r);
                cout << "      k=" << k << ", np=" << np << ", nc=" << nc << endl;
                if (np < p[k] || (np == p[k] && nc < c[k])) {
                    cout << "      Updating: p[" << k << "]=" << p[k] << "->" << np << ", c[" << k << "]=" << c[k] << "->" << nc << endl;
                    p[k] = np;
                    c[k] = nc;
                }
            }
        }
    }

    if (p[n] == I) {
        cout << "Impossible\n";
    } else {
        cout << c[n] << '\n';
    }
    return 0;
}