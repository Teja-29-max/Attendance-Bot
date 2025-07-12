#include <bits/stdc++.h>
using namespace std;

const int MAXN = 505;
const int LOG = 17;

vector<pair<int, int>> tree[MAXN];  // Adjacency list storing (child, weight)
int up[MAXN][LOG];                  // Binary lifting table
int maxEdge[MAXN][LOG];            // Max edge weight in 2^i jump
int minEdge[MAXN][LOG];            // Min edge weight in 2^i jump
int depth[MAXN];                   // Depth of each node

// ✅ CORRECTED: DFS to fill binary lifting tables properly
void dfs(int u, int parent) {
    up[u][0] = parent;
    
    // For root node, set edge weights to invalid values
    if (parent == u) {
        maxEdge[u][0] = -1e9;
        minEdge[u][0] = 1e9;
    } else {
        // Find the edge weight from parent to u
        int edgeWeight = 0;
        for (auto [v, w] : tree[u]) {
            if (v == parent) {
                edgeWeight = w;
                break;
            }
        }
        maxEdge[u][0] = minEdge[u][0] = edgeWeight;
    }

    // Fill binary lifting tables
    for (int i = 1; i < LOG; i++) {
        if (up[u][i - 1] == -1 || up[u][i - 1] == u) {
            up[u][i] = -1;
            maxEdge[u][i] = maxEdge[u][i - 1];
            minEdge[u][i] = minEdge[u][i - 1];
        } else {
            int mid = up[u][i - 1];
            up[u][i] = up[mid][i - 1];

            // Take max/min through both jump segments
            maxEdge[u][i] = max(maxEdge[u][i - 1], maxEdge[mid][i - 1]);
            minEdge[u][i] = min(minEdge[u][i - 1], minEdge[mid][i - 1]);
        }
    }

    for (auto [v, w] : tree[u]) {
        if (v != parent) {
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }
}

// ✅ CORRECTED: Query LCA and return max/min edge weight on the path
pair<int, int> query(int u, int v) {
    int maxW = -1e9, minW = 1e9;

    // Ensure u is deeper
    if (depth[u] < depth[v]) swap(u, v);

    // Lift u up to the depth of v
    for (int i = LOG - 1; i >= 0; i--) {
        if (up[u][i] != -1 && depth[up[u][i]] >= depth[v]) {
            maxW = max(maxW, maxEdge[u][i]);
            minW = min(minW, minEdge[u][i]);
            u = up[u][i];
        }
    }

    // If same after lifting
    if (u == v) return {maxW, minW};

    // Lift both until common ancestor
    for (int i = LOG - 1; i >= 0; i--) {
        if (up[u][i] != -1 && up[u][i] != up[v][i]) {
            maxW = max({maxW, maxEdge[u][i], maxEdge[v][i]});
            minW = min({minW, minEdge[u][i], minEdge[v][i]});
            u = up[u][i];
            v = up[v][i];
        }
    }

    // Final edge to LCA
    maxW = max({maxW, maxEdge[u][0], maxEdge[v][0]});
    minW = min({minW, minEdge[u][0], minEdge[v][0]});

    return {maxW, minW};
}

// ✅ CORRECTED: Main function with proper initialization
vector<int> calc(int N, vector<int> P, vector<int> W) {
    // Reset for multiple test cases
    for (int i = 0; i < N; i++) {
        tree[i].clear();
        depth[i] = 0;
        for (int j = 0; j < LOG; j++) {
            up[i][j] = -1;
            maxEdge[i][j] = -1e9;  // ✅ CORRECTED: Initialize to -INF
            minEdge[i][j] = 1e9;
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

    dfs(0, 0);  // ✅ CORRECTED: Start DFS from root node 0 without weight

    long long maxSum = 0, minSum = 0;

    // Query each pair of distinct nodes (u, v)
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
KEY FIXES MADE:

1. **Fixed DFS function**: 
   - Removed the weight parameter from dfs() call
   - Properly find edge weights by searching the adjacency list
   - Handle root node correctly (no parent edge)

2. **Fixed initialization**:
   - Initialize maxEdge to -1e9 instead of 0
   - This ensures proper max operations

3. **Fixed edge weight storage**:
   - maxEdge[u][i] now correctly stores the maximum edge weight on the path from u to its 2^i ancestor
   - minEdge[u][i] stores the minimum edge weight on the same path

4. **Fixed query function**:
   - Initialize maxW to -1e9 to handle edge cases properly
   - The logic now correctly computes max/min edge weights on paths

5. **Root node handling**:
   - Root node (0) is handled specially since it has no parent edge
   - Its edge weights are set to invalid values that won't affect the results
*/