#include <bits/stdc++.h>
using namespace std;

const int MAXN = 505;
const int LOG = 17;

vector<pair<int, int>> tree[MAXN];  // Adjacency list storing (child, weight)
int up[MAXN][LOG];                  // Binary lifting table
int maxEdge[MAXN][LOG];            // Max edge weight in 2^i jump
int minEdge[MAXN][LOG];            // Min edge weight in 2^i jump
int depth[MAXN];                   // Depth of each node

// ISSUE 1: The DFS function has a logical error in how it handles edge weights
// The current code passes the weight of the edge from parent to current node,
// but it should be storing the weight of the edge from current node to its children
void dfs(int u, int parent, int wt) {
    up[u][0] = parent;
    maxEdge[u][0] = minEdge[u][0] = wt;  // ISSUE: This stores parent->u weight, not u->child weight

    for (int i = 1; i < LOG; i++) {
        if (up[u][i - 1] == -1) {
            up[u][i] = -1;
            maxEdge[u][i] = maxEdge[u][i - 1];
            minEdge[u][i] = minEdge[u][i - 1];
        } else {
            int mid = up[u][i - 1];
            up[u][i] = up[mid][i - 1];

            maxEdge[u][i] = max(maxEdge[u][i - 1], maxEdge[mid][i - 1]);
            minEdge[u][i] = min(minEdge[u][i - 1], minEdge[mid][i - 1]);
        }
    }

    for (auto [v, w] : tree[u]) {
        if (v != parent) {
            depth[v] = depth[u] + 1;
            dfs(v, u, w);  // ISSUE: This passes the weight from u->v, but the table should store u->v weight
        }
    }
}

// ISSUE 2: The query function has problems with edge weight handling
pair<int, int> query(int u, int v) {
    int maxW = 0, minW = 1e9;

    if (depth[u] < depth[v]) swap(u, v);

    // ISSUE: This logic is incorrect for edge weights
    // The maxEdge[u][i] stores the max weight on the path from u to its 2^i ancestor
    // But we need the max weight on the path from u to its 2^i ancestor
    for (int i = LOG - 1; i >= 0; i--) {
        if (up[u][i] != -1 && depth[up[u][i]] >= depth[v]) {
            maxW = max(maxW, maxEdge[u][i]);
            minW = min(minW, minEdge[u][i]);
            u = up[u][i];
        }
    }

    if (u == v) return {maxW, minW};

    for (int i = LOG - 1; i >= 0; i--) {
        if (up[u][i] != -1 && up[u][i] != up[v][i]) {
            maxW = max({maxW, maxEdge[u][i], maxEdge[v][i]});
            minW = min({minW, minEdge[u][i], minEdge[v][i]});
            u = up[u][i];
            v = up[v][i];
        }
    }

    maxW = max({maxW, maxEdge[u][0], maxEdge[v][0]});
    minW = min({minW, minEdge[u][0], minEdge[v][0]});

    return {maxW, minW};
}

// ISSUE 3: The main function has initialization problems
vector<int> calc(int N, vector<int> P, vector<int> W) {
    for (int i = 0; i < N; i++) {
        tree[i].clear();
        depth[i] = 0;
        for (int j = 0; j < LOG; j++) {
            up[i][j] = -1;
            maxEdge[i][j] = 0;      // ISSUE: Should be -INF for max
            minEdge[i][j] = 1e9;    // This is correct for min
        }
    }

    // Build tree from parent/weight arrays
    for (int i = 1; i < N; i++) {
        int u = P[i] - 1;
        int v = i;
        int wt = W[i];
        tree[u].push_back({v, wt});
        tree[v].push_back({u, wt});
    }

    dfs(0, 0, 0);  // ISSUE: This passes weight 0 for root, which is wrong

    long long maxSum = 0, minSum = 0;

    for (int u = 0; u < N; u++) {
        for (int v = u + 1; v < N; v++) {
            pair<int, int> res = query(u, v);
            maxSum += res.first;
            minSum += res.second;
        }
    }

    return {static_cast<int>(maxSum), static_cast<int>(minSum)};
}

/*
MAIN ISSUES IDENTIFIED:

1. **Edge Weight Storage Logic**: The current approach stores edge weights incorrectly.
   - maxEdge[u][i] should store the maximum edge weight on the path from u to its 2^i ancestor
   - But the current code stores the weight of the edge from u's parent to u

2. **DFS Initialization**: The root node (0) is initialized with weight 0, which is incorrect
   - The root has no parent, so it shouldn't have a weight associated with it

3. **Query Logic**: The query function doesn't properly handle edge weights on the path
   - It's trying to use node-based logic for edge-based queries

4. **Initialization Values**: maxEdge should be initialized to -INF, not 0

CORRECTED APPROACH:
- Store edge weights properly in the binary lifting table
- Handle the root node correctly (no parent edge)
- Fix the query logic to properly compute max/min edge weights on paths
*/