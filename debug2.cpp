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
        cout << "Adding character: " << c << endl;
        int x = c - 'a', y = v.size();
        cout << "  x=" << x << ", y=" << y << ", z=" << z << endl;
        v.emplace_back();
        v[y].L = v[z].L + 1;
        int p = z;
        cout << "  Initial: p=" << p << ", v[z].L=" << v[z].L << endl;
        
        while (p != -1 && v[p].n[x] == -1) {
            cout << "    Setting v[" << p << "].n[" << x << "] = " << y << endl;
            v[p].n[x] = y;
            p = v[p].l;
            cout << "    Moving to p=" << p << endl;
        }
        
        if (p == -1) {
            cout << "  Case 1: p == -1, setting v[" << y << "].l = 0" << endl;
            v[y].l = 0;
        } else {
            int q = v[p].n[x];
            cout << "  Case 2: p=" << p << ", q=" << q << endl;
            cout << "    v[p].L=" << v[p].L << ", v[q].L=" << v[q].L << endl;
            if (v[p].L + 1 == v[q].L) {
                cout << "  Case 2a: continuous, setting v[" << y << "].l = " << q << endl;
                v[y].l = q;
            } else {
                cout << "  Case 2b: non-continuous, cloning" << endl;
                int clone = v.size();
                v.push_back(v[q]);
                v[clone].L = v[p].L + 1;
                cout << "    Created clone " << clone << " with L=" << v[clone].L << endl;
                while (p != -1 && v[p].n[x] == q) {
                    cout << "    Redirecting v[" << p << "].n[" << x << "] from " << q << " to " << clone << endl;
                    v[p].n[x] = clone;
                    p = v[p].l;
                }
                v[q].l = clone;
                v[y].l = clone;
                cout << "    Set links: v[" << q << "].l=" << clone << ", v[" << y << "].l=" << clone << endl;
            }
        }
        z = y;
        cout << "  New z=" << z << endl;
        cout << "  States count: " << v.size() << endl;
    }

    inline int s(int u, char c) const {
        int result = v[u].n[c - 'a'];
        cout << "Query: s(" << u << ", " << c << ") = " << result << endl;
        return result;
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

    T a(y.size());
    cout << "\nBuilding forward automaton for: " << y << endl;
    for (char c : y) a.e(c);
    cout << "\nForward automaton:" << endl;
    a.debug_print();
    
    reverse(y.begin(), y.end());
    T b(y.size());
    cout << "\nBuilding reverse automaton for: " << y << endl;
    for (char c : y) b.e(c);
    cout << "\nReverse automaton:" << endl;
    b.debug_print();

    vector<int> p(n + 1, I), c(n + 1, I);
    p[0] = 0;
    c[0] = 0;

    cout << "\nDP phase:" << endl;
    for (int i = 0; i < n; ++i) {
        if (p[i] == I) continue;
        cout << "Processing position " << i << " (p=" << p[i] << ", c=" << c[i] << ")" << endl;
        
        for (int t = 0; t < 2; ++t) {
            const T& m = (t == 0 ? a : b);
            cout << "  Using " << (t == 0 ? "forward" : "reverse") << " automaton" << endl;
            int u = 0;
            for (int j = i; j < n; ++j) {
                u = m.s(u, x[j]);
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