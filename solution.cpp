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
        
        // Add divisors of X and Y (up to reasonable limit)
        for (long long i = 1; i * i <= X && i <= 100000; i++) {
            if (X % i == 0) {
                candidates.insert(i);
                if (i != X / i && X / i <= 100000) {
                    candidates.insert(X / i);
                }
            }
        }
        
        for (long long i = 1; i * i <= Y && i <= 100000; i++) {
            if (Y % i == 0) {
                candidates.insert(i);
                if (i != Y / i && Y / i <= 100000) {
                    candidates.insert(Y / i);
                }
            }
        }
        
        // Add X, Y, gcd(X,Y) if they're reasonable
        if (X <= 100000) candidates.insert(X);
        if (Y <= 100000) candidates.insert(Y);
        candidates.insert(gcd(X, Y));
        
        // Add values around original Z
        for (long long delta = -50000; delta <= 50000; delta++) {
            long long newZ = Z + delta;
            if (newZ >= 1 && newZ <= 100000) {
                candidates.insert(newZ);
            }
        }
        
        // Add some powers of 2 and small primes
        for (long long p = 1; p <= 100000; p *= 2) {
            candidates.insert(p);
        }
        for (long long p = 1; p <= 100000; p *= 3) {
            candidates.insert(p);
        }
        for (long long p = 1; p <= 100000; p *= 5) {
            candidates.insert(p);
        }
        
        // For each candidate Z value, try operations
        for (long long newZ : candidates) {
            long long cost_change_Z = abs(newZ - Z);
            
            // Try single operations
            // Operation 2a: X := gcd(X, newZ), Y := lcm(Y, newZ)
            long long newX1 = gcd(X, newZ);
            long long newY1 = lcm(Y, newZ);
            if (newX1 == newY1) {
                min_cost = min(min_cost, cost_change_Z + C);
            }
            
            // Operation 2b: X := lcm(X, newZ), Y := gcd(Y, newZ)
            long long newX2 = lcm(X, newZ);
            long long newY2 = gcd(Y, newZ);
            if (newX2 == newY2) {
                min_cost = min(min_cost, cost_change_Z + C);
            }
            
            // Try double operations (same Z)
            if (cost_change_Z + 2 * C < min_cost) {
                // Apply 2a then 2b
                long long tempX = newX1, tempY = newY1;
                long long finalX = lcm(tempX, newZ);
                long long finalY = gcd(tempY, newZ);
                if (finalX == finalY) {
                    min_cost = min(min_cost, cost_change_Z + 2 * C);
                }
                
                // Apply 2b then 2a
                tempX = newX2; tempY = newY2;
                finalX = gcd(tempX, newZ);
                finalY = lcm(tempY, newZ);
                if (finalX == finalY) {
                    min_cost = min(min_cost, cost_change_Z + 2 * C);
                }
                
                // Apply 2a twice
                tempX = newX1; tempY = newY1;
                finalX = gcd(tempX, newZ);
                finalY = lcm(tempY, newZ);
                if (finalX == finalY) {
                    min_cost = min(min_cost, cost_change_Z + 2 * C);
                }
                
                // Apply 2b twice
                tempX = newX2; tempY = newY2;
                finalX = lcm(tempX, newZ);
                finalY = gcd(tempY, newZ);
                if (finalX == finalY) {
                    min_cost = min(min_cost, cost_change_Z + 2 * C);
                }
            }
        }
        
        cout << min_cost << "\n";
    }
    
    return 0;
}