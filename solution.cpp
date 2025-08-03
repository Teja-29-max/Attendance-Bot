#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    string X, Y;
    int S, R;
    
    cin >> X >> Y >> S >> R;
    
    int n = X.length();
    int m = Y.length();
    
    string rev_Y = Y;
    reverse(rev_Y.begin(), rev_Y.end());
    
    // dp[i] = {min_substrings, min_cost} to form X[0...i-1]
    vector<pair<int, int>> dp(n + 1, {INT_MAX, INT_MAX});
    dp[0] = {0, 0}; // empty string requires 0 substrings and 0 cost
    
    for (int i = 0; i < n; i++) {
        if (dp[i].first == INT_MAX) continue;
        
        // Try all possible substrings starting from position i in X
        for (int len = 1; len <= n - i; len++) {
            string target = X.substr(i, len);
            
            // Check if target exists as substring in Y
            bool found_in_Y = false;
            for (int j = 0; j <= m - len; j++) {
                if (Y.substr(j, len) == target) {
                    found_in_Y = true;
                    break;
                }
            }
            
            // Check if target exists as substring in reversed Y
            bool found_in_rev_Y = false;
            for (int j = 0; j <= m - len; j++) {
                if (rev_Y.substr(j, len) == target) {
                    found_in_rev_Y = true;
                    break;
                }
            }
            
            int next_pos = i + len;
            
            // Update dp[next_pos] if we found the substring
            if (found_in_Y || found_in_rev_Y) {
                int new_substrings = dp[i].first + 1;
                
                // Calculate costs for both options
                int cost_normal = (found_in_Y) ? dp[i].second + S : INT_MAX;
                int cost_reversed = (found_in_rev_Y) ? dp[i].second + R : INT_MAX;
                
                int best_cost = min(cost_normal, cost_reversed);
                
                // Update if we found a better solution
                if (new_substrings < dp[next_pos].first || 
                    (new_substrings == dp[next_pos].first && best_cost < dp[next_pos].second)) {
                    dp[next_pos] = {new_substrings, best_cost};
                }
            }
        }
    }
    
    if (dp[n].first == INT_MAX) {
        cout << "Impossible" << endl;
    } else {
        cout << dp[n].second << endl;
    }
    
    return 0;
}