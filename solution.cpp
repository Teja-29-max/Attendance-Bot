#include <bits/stdc++.h>
using namespace std;

bool is_good(const vector<int>& A) {
    int sum = 0;
    for (int x : A) {
        sum += x;
        if (sum % 3 == 0) return false;
    }
    return true;
}

bool try_reverse(const vector<int>& A, int prefix_len) {
    int N = A.size();
    if (prefix_len < 1 || prefix_len > N) return false;
    
    vector<int> B(N);
    
    // Reverse the first prefix_len elements
    for (int i = 0; i < prefix_len; i++) {
        B[i] = A[prefix_len - 1 - i];
    }
    
    // Copy the remaining elements
    for (int i = prefix_len; i < N; i++) {
        B[i] = A[i];
    }
    
    return is_good(B);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        vector<int> A(N);
        for (int i = 0; i < N; ++i) cin >> A[i];

        // First check if array is already good
        if (is_good(A)) {
            cout << "Yes\n";
            continue;
        }

        // Try all possible prefix reversals
        bool ok = false;
        for (int prefix_len = 1; prefix_len <= N && !ok; ++prefix_len) {
            if (try_reverse(A, prefix_len)) {
                ok = true;
            }
        }

        cout << (ok ? "Yes\n" : "No\n");
    }

    return 0;
}