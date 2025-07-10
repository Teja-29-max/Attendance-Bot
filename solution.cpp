#include <bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    while (b) { long long t = a % b; a = b; b = t; }
    return a;
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
            cout << "0\n";
            continue;
        }
        
        long long ans = LLONG_MAX;
        
        // Try very limited range around Z
        for (long long i = max(1LL, Z - 50); i <= Z + 50; i++) {
            long long cost = abs(i - Z);
            if (cost + C >= ans) continue;
            
            // Try operation 2a
            long long g1 = gcd(X, i);
            long long l1 = lcm(Y, i);
            if (g1 == l1) {
                ans = min(ans, cost + C);
                continue;
            }
            
            // Try operation 2b
            long long l2 = lcm(X, i);
            long long g2 = gcd(Y, i);
            if (l2 == g2) {
                ans = min(ans, cost + C);
                continue;
            }
            
            // Try double operations only for very small costs
            if (cost + 2 * C < ans && cost <= 10) {
                if (lcm(g1, i) == gcd(l1, i)) ans = min(ans, cost + 2 * C);
                if (gcd(l2, i) == lcm(g2, i)) ans = min(ans, cost + 2 * C);
            }
        }
        
        // Try small divisors
        for (long long d = 1; d <= 100; d++) {
            if (X % d == 0) {
                long long cost = abs(d - Z);
                if (cost + C < ans) {
                    if (gcd(X, d) == lcm(Y, d)) ans = min(ans, cost + C);
                    if (lcm(X, d) == gcd(Y, d)) ans = min(ans, cost + C);
                }
                
                long long big_d = X / d;
                if (big_d != d && big_d <= 100) {
                    cost = abs(big_d - Z);
                    if (cost + C < ans) {
                        if (gcd(X, big_d) == lcm(Y, big_d)) ans = min(ans, cost + C);
                        if (lcm(X, big_d) == gcd(Y, big_d)) ans = min(ans, cost + C);
                    }
                }
            }
            
            if (Y % d == 0) {
                long long cost = abs(d - Z);
                if (cost + C < ans) {
                    if (gcd(X, d) == lcm(Y, d)) ans = min(ans, cost + C);
                    if (lcm(X, d) == gcd(Y, d)) ans = min(ans, cost + C);
                }
                
                long long big_d = Y / d;
                if (big_d != d && big_d <= 100) {
                    cost = abs(big_d - Z);
                    if (cost + C < ans) {
                        if (gcd(X, big_d) == lcm(Y, big_d)) ans = min(ans, cost + C);
                        if (lcm(X, big_d) == gcd(Y, big_d)) ans = min(ans, cost + C);
                    }
                }
            }
        }
        
        // Try gcd(X,Y)
        long long g = gcd(X, Y);
        long long cost = abs(g - Z);
        if (cost + C < ans) {
            if (gcd(X, g) == lcm(Y, g)) ans = min(ans, cost + C);
            if (lcm(X, g) == gcd(Y, g)) ans = min(ans, cost + C);
        }
        
        // Try powers of 2
        for (long long p = 1; p <= 64; p *= 2) {
            cost = abs(p - Z);
            if (cost + C < ans) {
                if (gcd(X, p) == lcm(Y, p)) ans = min(ans, cost + C);
                if (lcm(X, p) == gcd(Y, p)) ans = min(ans, cost + C);
            }
        }
        
        cout << ans << "\n";
    }
    
    return 0;
}