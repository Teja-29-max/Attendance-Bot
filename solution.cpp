#include <bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

long long lcm(long long a, long long b) {
    long long g = gcd(a, b);
    return (a / g) * b;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        long long X, Y, Z, C;
        cin >> X >> Y >> Z >> C;
        
        long long min_cost = LLONG_MAX;
        
        // Key insight: We need to find Z' such that either:
        // 1. gcd(X, Z') = lcm(Y, Z'), or  
        // 2. lcm(X, Z') = gcd(Y, Z')
        
        // Collect candidate values for Z
        set<long long> candidates;
        
        // Add divisors of X and Y
        for (long long i = 1; i * i <= X && i <= 100000; i++) {
            if (X % i == 0) {
                candidates.insert(i);
                candidates.insert(X / i);
            }
        }
        
        for (long long i = 1; i * i <= Y && i <= 100000; i++) {
            if (Y % i == 0) {
                candidates.insert(i);
                candidates.insert(Y / i);
            }
        }
        
        // Add some multiples of gcd(X,Y)
        long long g = gcd(X, Y);
        for (long long i = 1; i <= 1000 && i * g <= 1000000; i++) {
            candidates.insert(i * g);
        }
        
        // Add X, Y themselves
        candidates.insert(X);
        candidates.insert(Y);
        candidates.insert(g);
        
        // Try values around original Z
        for (long long delta = -100000; delta <= 100000; delta++) {
            long long newZ = Z + delta;
            if (newZ >= 1) {
                candidates.insert(newZ);
            }
        }
        
        for (long long newZ : candidates) {
            if (newZ < 1) continue;
            
            long long cost_change_Z = abs(newZ - Z);
            
            // Try operation 2a: X := gcd(X,newZ), Y := lcm(Y,newZ)
            long long gcd_X_Z = gcd(X, newZ);
            long long lcm_Y_Z = lcm(Y, newZ);
            if (gcd_X_Z == lcm_Y_Z) {
                min_cost = min(min_cost, cost_change_Z + C);
            }
            
            // Try operation 2b: X := lcm(X,newZ), Y := gcd(Y,newZ)
            long long lcm_X_Z = lcm(X, newZ);
            long long gcd_Y_Z = gcd(Y, newZ);
            if (lcm_X_Z == gcd_Y_Z) {
                min_cost = min(min_cost, cost_change_Z + C);
            }
        }
        
        cout << min_cost << "\n";
    }
    
    return 0;
}