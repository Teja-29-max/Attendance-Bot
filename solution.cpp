#include <bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

long long lcm(long long a, long long b) {
    return a / gcd(a, b) * b;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        long long X, Y, Z, C;
        cin >> X >> Y >> Z >> C;
        
        if (X == Y) {
            cout << 0 << "\n";
            continue;
        }
        
        long long min_cost = LLONG_MAX;
        
        // Collect important Z values to try
        set<long long> candidates;
        
        // Add divisors of X and Y (limited for efficiency)
        for (long long i = 1; i * i <= X && i <= 50000; i++) {
            if (X % i == 0) {
                candidates.insert(i);
                if (i != X / i && X / i <= 50000) {
                    candidates.insert(X / i);
                }
            }
        }
        
        for (long long i = 1; i * i <= Y && i <= 50000; i++) {
            if (Y % i == 0) {
                candidates.insert(i);
                if (i != Y / i && Y / i <= 50000) {
                    candidates.insert(Y / i);
                }
            }
        }
        
        // Add X, Y, gcd(X,Y) if reasonable
        if (X <= 50000) candidates.insert(X);
        if (Y <= 50000) candidates.insert(Y);
        candidates.insert(gcd(X, Y));
        
        // Reduced range around original Z for efficiency
        for (long long delta = -5000; delta <= 5000; delta++) {
            long long newZ = Z + delta;
            if (newZ >= 1) {
                candidates.insert(newZ);
            }
        }
        
        // Add some powers of small primes (reduced range)
        for (long long p = 1; p <= 50000; p *= 2) {
            candidates.insert(p);
        }
        for (long long p = 1; p <= 50000; p *= 3) {
            candidates.insert(p);
        }
        for (long long p = 1; p <= 50000; p *= 5) {
            candidates.insert(p);
        }
        for (long long p = 1; p <= 50000; p *= 7) {
            candidates.insert(p);
        }
        
        // Try each candidate Z value
        for (long long newZ : candidates) {
            long long cost_change_Z = abs(newZ - Z);
            
            // Early pruning: if just changing Z costs too much, skip
            if (cost_change_Z + C >= min_cost) continue;
            
            // Operation 2a: X := gcd(X, newZ), Y := lcm(Y, newZ)
            long long newX1 = gcd(X, newZ);
            long long newY1 = lcm(Y, newZ);
            if (newX1 == newY1) {
                min_cost = min(min_cost, cost_change_Z + C);
                continue; // Found solution, try next Z
            }
            
            // Operation 2b: X := lcm(X, newZ), Y := gcd(Y, newZ)
            long long newX2 = lcm(X, newZ);
            long long newY2 = gcd(Y, newZ);
            if (newX2 == newY2) {
                min_cost = min(min_cost, cost_change_Z + C);
                continue; // Found solution, try next Z
            }
            
            // Try double operations only if single operations didn't work
            if (cost_change_Z + 2 * C < min_cost) {
                // Apply 2a then 2b
                long long finalX = lcm(newX1, newZ);
                long long finalY = gcd(newY1, newZ);
                if (finalX == finalY) {
                    min_cost = min(min_cost, cost_change_Z + 2 * C);
                    continue;
                }
                
                // Apply 2b then 2a
                finalX = gcd(newX2, newZ);
                finalY = lcm(newY2, newZ);
                if (finalX == finalY) {
                    min_cost = min(min_cost, cost_change_Z + 2 * C);
                    continue;
                }
                
                // Apply 2a twice
                finalX = gcd(newX1, newZ);
                finalY = lcm(newY1, newZ);
                if (finalX == finalY) {
                    min_cost = min(min_cost, cost_change_Z + 2 * C);
                    continue;
                }
                
                // Apply 2b twice
                finalX = lcm(newX2, newZ);
                finalY = gcd(newY2, newZ);
                if (finalX == finalY) {
                    min_cost = min(min_cost, cost_change_Z + 2 * C);
                }
            }
        }
        
        cout << min_cost << "\n";
    }
    
    return 0;
}