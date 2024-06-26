/*Description: A polynomial of degree d can be uniquely identified
given its values on d + 1 unique points. O(n) to pre-calculate given
the first n points (x=0 to n-1) of the polynomial. Then answer each
query to interpolate the x’th term in O(n). All values are done modulo
mod, which needs to be a prime as we need its inverse modulo. Also
includes an additional helper function called find_degree(terms, mod).
Given at least the first d+2 points of a polynomial of degree d, it finds
d in roughly O(n log d). Note, n should not exceed mod due to the
way modular inverse is used. In such cases, we can use interpolation
without modulo in big integers and take the remainder later
Time: O (n) */
using namespace std;
struct Lagrange {
    vector<int> terms, dp;
    int mod, n;
    Lagrange() {}
    Lagrange(const vector<int>& terms, int mod) : terms(terms), mod(mod) {
        n = terms.size();
        assert(n <= mod);
        int i, v, f;
        for (f = 1, i = 1; i < n; i++) f = (long long)f * i % mod;
        v = expo(f, mod - 2);
        vector<int> inv(n, v);
        for (i = n - 1; i > 0; i--) {
            inv[i - 1] = (long long)inv[i] * i % mod;
        }
        dp.resize(n, 1);
        for (i = 0; i < n; i++) {
            dp[i] = (long long)inv[i] * inv[n - i - 1] % mod;
            dp[i] = (long long)dp[i] * terms[i] % mod;
        }
    }
    int expo(int a, int b) {
        int res = 1;
        while (b) {
            if (b & 1) res = (long long)res * a % mod;
            a = (long long)a * a % mod;
            b >>= 1;
        }
        return res;
    }
    int interpolate(long long x) {
        if (x < n) return terms[x] % mod;
        x %= mod;
        int i, w;
        vector<int> X(n, 1), Y(n, 1);
        for (i = 1; i < n; i++) {
            X[i] = (long long)X[i - 1] * (x - i + 1) % mod;
            if (X[i] < 0) X[i] += mod;
        }
        for (i = n - 2; i >= 0; i--) {
            Y[i] = (long long)Y[i + 1] * (x - i - 1) % mod;
            if (Y[i] < 0) Y[i] += mod;
        }
        long long res = 0;
        for (i = 0; i < n; i++) {
            w = ((long long)X[i] * Y[i] % mod) * dp[i] % mod;
            if ((n - i + 1) & 1) w = mod - w;
            res += w;
        }
        return res % mod;
    }
};
vector<int> get_terms(const vector<int>& terms, int mod, int l, int r) {
    auto lagrange = Lagrange(terms, mod);
    vector<int> res;
    for (int i = l; i <= r; i++) {
        res.push_back(lagrange.interpolate(i));
    }
    return res;
}
int find_degree(const vector<int>& terms, int mod) {
    long long v = mod;
    int k = 1, n = terms.size();
    while (v < INT_MAX) {
        v *= mod;
        k++;
    }
    int l = 1 << 30, r = l + k - 1;
    auto expected = get_terms(terms, mod, l, r);
    int low = 1, high = n - 1;
    while ((low + 1) < high) {
        int mid = (low + high) >> 1;
        vector<int> v(terms.begin(), terms.begin() + mid);
        if (get_terms(v, mod, l, r) == expected)
            high = mid;
        else
            low = mid;
    }
    for (int d = low; d <= high; d++) {
        vector<int> v(terms.begin(), terms.begin() + d);
        if (get_terms(v, mod, l, r) == expected) return d - 1;
    }
    return -1;
}
int main() {
    const int mod = 1000000007;
    vector<int> terms = vector<int>{0, 1, 5, 14, 30};
    auto lagrange = Lagrange(terms, mod);
    assert(lagrange.interpolate(5) == 55);
    assert(lagrange.interpolate(6) == 91);
    assert(lagrange.interpolate(1 << 30) == 300663155);
    assert(lagrange.interpolate(1LL << 60) == 717860166);
    assert(find_degree(terms, mod) == 3);
    terms.pop_back();
    assert(find_degree(terms, mod) == -1);
    return 0;
}