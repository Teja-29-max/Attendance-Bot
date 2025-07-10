#include <bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

long long lcm(long long a, long long b) {
    long long g = gcd(a, b);
    // Check for overflow
    if (a / g > LLONG_MAX / b) return LLONG_MAX;
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
        
        // Try values around original Z first (most likely to be optimal)
        for (long long delta = -50000; delta <= 50000; delta++) {
            long long newZ = Z + delta;
            if (newZ <= 0) continue;
            
            long long cost_change_Z = abs(delta);
            
            // Skip if already more expensive than current best
            if (cost_change_Z + C >= min_cost) continue;
            
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
        
        // Also try some specific important values for Z
        vector<long long> special_values;
        
        // Add divisors of X and Y (but limit to reasonable size)
        for (long long i = 1; i * i <= min(X, (long long)1000000); i++) {
            if (X % i == 0) {
                special_values.push_back(i);
                if (i != X / i) special_values.push_back(X / i);
            }
        }
        
        for (long long i = 1; i * i <= min(Y, (long long)1000000); i++) {
            if (Y % i == 0) {
                special_values.push_back(i);
                if (i != Y / i) special_values.push_back(Y / i);
            }
        }
        
        // Add X, Y, gcd(X,Y)
        special_values.push_back(X);
        special_values.push_back(Y);
        special_values.push_back(gcd(X, Y));
        
        for (long long newZ : special_values) {
            if (newZ <= 0) continue;
            
            long long cost_change_Z = abs(newZ - Z);
            
            // Skip if already more expensive than current best
            if (cost_change_Z + C >= min_cost) continue;
            
            // Try operation 2a
            long long gcd_X_Z = gcd(X, newZ);
            long long lcm_Y_Z = lcm(Y, newZ);
            if (gcd_X_Z == lcm_Y_Z) {
                min_cost = min(min_cost, cost_change_Z + C);
            }
            
            // Try operation 2b  
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