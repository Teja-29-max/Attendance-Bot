#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        int N;
        cin >> N;
        
        vector<int> P(N);
        for (int i = 0; i < N; i++) {
            cin >> P[i];
            P[i]--; // Convert to 0-indexed for easier processing
        }
        
        /* 
         * Key insight: A person P[i] at position i can be non-priority if and only if
         * for all positions j < i, we have P[j] <= P[i].
         * 
         * Reasoning: If P[i] is non-priority and gets called:
         * 1. P[i] moves to the front of the line
         * 2. P[i] swaps backwards with priority customers immediately behind
         *    until no priority customer is immediately behind P[i]
         * 
         * For P[i] to end up at position i, P[i] must swap with exactly i people.
         * If there exists P[j] > P[i] at position j < i, then P[j] originally came
         * from a position after P[i] in [1,2,3,...,N]. For P[j] to be in front of P[i]
         * in the final order while P[i] is non-priority, we get a contradiction.
         * 
         * Therefore, P[i] can be non-priority iff no P[j] > P[i] exists at positions j < i.
         */
        
        long long ans = 1;
        
        for (int i = 0; i < N; i++) {
            bool canBeNonPriority = true;
            
            // Check if P[i] can be non-priority
            for (int j = 0; j < i; j++) {
                if (P[j] > P[i]) {
                    canBeNonPriority = false;
                    break;
                }
            }
            
            // If P[i] can be either priority or non-priority, we have 2 choices
            if (canBeNonPriority) {
                ans = (ans * 2) % MOD;
            }
            // Otherwise, P[i] must be priority (no additional factor)
        }
        
        cout << ans << "\n";
    }
    
    return 0;
}