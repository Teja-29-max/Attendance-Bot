#include <bits/stdc++.h>
using namespace std;

// Function to calculate MEX of a set
int mex(const set<int>& s) {
    int mex_val = 0;
    for (int x : s) {
        if (x == mex_val) {
            mex_val++;
        }
    }
    return mex_val;
}

// Function to find path between two vertices using BFS
vector<int> findPath(const vector<vector<int>>& adj, int start, int end, int n) {
    vector<int> parent(n + 1, -1);
    vector<bool> visited(n + 1, false);
    queue<int> q;
    
    q.push(start);
    visited[start] = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        if (u == end) break;
        
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    
    // Reconstruct path
    vector<int> path;
    int current = end;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    reverse(path.begin(), path.end());
    return path;
}

// Function to calculate score for a given permutation
int calculateScore(const vector<vector<int>>& adj, const vector<int>& permutation, int n) {
    int score = 0;
    
    for (int u = 1; u <= n; u++) {
        for (int v = u; v <= n; v++) {
            // Find path from u to v
            vector<int> path = findPath(adj, u, v, n);
            
            // Collect labels on the path
            set<int> labels;
            for (int vertex : path) {
                labels.insert(permutation[vertex - 1]); // vertex is 1-indexed, permutation is 0-indexed
            }
            
            // Calculate MEX
            score += mex(labels);
        }
    }
    
    return score;
}

// Function to find minimum score using greedy approach
int findMinScore(const vector<vector<int>>& adj, int n) {
    // Strategy: Place 0 at the center, then place consecutive numbers
    // to minimize gaps in paths
    
    // Find the center of the tree (vertex with minimum maximum distance)
    vector<int> maxDist(n + 1, 0);
    
    for (int start = 1; start <= n; start++) {
        vector<bool> visited(n + 1, false);
        vector<int> dist(n + 1, 0);
        queue<int> q;
        
        q.push(start);
        visited[start] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        
        // Find maximum distance from start
        int maxD = 0;
        for (int i = 1; i <= n; i++) {
            maxD = max(maxD, dist[i]);
        }
        maxDist[start] = maxD;
    }
    
    // Find center (vertex with minimum maximum distance)
    int center = 1;
    for (int i = 2; i <= n; i++) {
        if (maxDist[i] < maxDist[center]) {
            center = i;
        }
    }
    
    // BFS from center to assign labels
    vector<int> permutation(n);
    vector<bool> visited(n + 1, false);
    queue<int> q;
    
    q.push(center);
    visited[center] = true;
    int label = 0;
    permutation[center - 1] = label++;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                permutation[v - 1] = label++;
                q.push(v);
            }
        }
    }
    
    return calculateScore(adj, permutation, n);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        int N;
        cin >> N;
        
        vector<vector<int>> adj(N + 1);
        
        for (int i = 0; i < N - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        int minScore = findMinScore(adj, N);
        cout << minScore << "\n";
    }
    
    return 0;
}