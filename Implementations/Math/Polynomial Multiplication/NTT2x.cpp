#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

const int MOD = 998244353;

inline void add(int &a, int b) { a += b; if (a >= MOD) a -= MOD; }
inline void sub(int &a, int b) { a -= b; if (a < 0) a += MOD; }


inline int mul(int a, int b) {
#if !defined(_WIN32) || defined(_WIN64)
    return (int) ((long long) a * b % MOD);
#endif
    unsigned long long x = (long long) a * b;
    unsigned xh = (unsigned) (x >> 32), xl = (unsigned) x, d, m;
    asm(
        "divl %4; \n\t"
        : "=a" (d), "=d" (m)
        : "d" (xh), "a" (xl), "r" (MOD)
    );
    return m;
}

inline int power(int a, long long b) {
    int res = 1;
    while (b > 0) {
        if (b & 1) {
          res = mul(res, a);
        }
        a = mul(a, a);
        b >>= 1;
    }
    return res;
}

inline int inv(int a) {
    a %= MOD;
    if (a < 0) a += MOD;
    int b = MOD, u = 0, v = 1;
    while (a) {
        int t = b / a;
        b -= t * a; swap(a, b);
        u -= t * v; swap(u, v);
    }
    assert(b == 1);
    if (u < 0) u += MOD;
    return u;
}

namespace ntt {
    int base = 1;
    vector<int> roots = {0, 1};
    vector<int> rev = {0, 1};
    int max_base = -1;
    int root = -1;

    void init() {
        int tmp = MOD - 1;
        max_base = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            max_base++;
        }
        root = 2;
        while (true) {
            if (power(root, 1 << max_base) == 1) {
                if (power(root, 1 << (max_base - 1)) != 1)  break;
            }
            root++;
        }
    }

    void ensure_base(int nbase) {
        if (max_base == -1) init();
        if (nbase <= base) return;
        assert(nbase <= max_base);
        rev.resize(1 << nbase);
        for (int i = 0; i < (1 << nbase); i++) {
            rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
        }
        roots.resize(1 << nbase);
        while (base < nbase) {
            int z = power(root, 1 << (max_base - 1 - base));
            for (int i = 1 << (base - 1); i < (1 << base); i++) {
                roots[i << 1] = roots[i];
                roots[(i << 1) + 1] = mul(roots[i], z);
            } 
            base++;
        }
    }

    void fft(vector<int> &a) {
        int n = (int) a.size();
        assert((n & (n - 1)) == 0);
        int zeros = __builtin_ctz(n);
        ensure_base(zeros);
        int shift = base - zeros;
        for (int i = 0; i < n; i++) {
            if (i < (rev[i] >> shift)) {
                swap(a[i], a[rev[i] >> shift]);
            }
        }
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += 2 * k) {
                for (int j = 0; j < k; j++) {
                    int x = a[i + j];
                    int y = mul(a[i + j + k], roots[j + k]);
                    a[i + j] = x + y - MOD;
                    if (a[i + j] < 0) a[i + j] += MOD;
                    a[i + j + k] = x - y + MOD;
                    if (a[i + j + k] >= MOD) a[i + j + k] -= MOD;
                }
            }
        }
    }

    vector<int> multiply(vector<int> &a, vector<int> &b, int eq = 0) {
        int need = (int) (a.size() + b.size() - 1);
        int nbase = 0;
        while ((1 << nbase) < need) nbase++;
        ensure_base(nbase);
        int sz = 1 << nbase;
        a.resize(sz);
        b.resize(sz);
        fft(a);
        if (eq) b = a; else fft(b);
        int inv_sz = inv(sz);
        for (int i = 0; i < sz; i++) {
            a[i] = mul(mul(a[i], b[i]), inv_sz);
        }
        reverse(a.begin() + 1, a.end());
        fft(a);
        a.resize(need);
        while (!a.empty() && a.back() == 0) {
            a.pop_back();
        }
        return a;
    }

    vector<int> square(vector<int> a) {
        return multiply(a, a, 1);
    }
};

int main() {
    // (x + 2)(x + 3) = x^2 + 5x + 6
    vector <int> a = {2, 1};
    vector <int> b = {3, 1};
    vector <int> c = ntt::multiply(a, b);
    for (int x : c) cout << x << " "; cout << endl;
    return 0;
}
