using namespace __gnu_pbds;
const int N = 3e5 + 9, mod = 998244353;
template <const int32_t MOD>
struct modint {
    int32_t value;
    modint() = default;
    modint(int32_t value_) : value(value_) {}
    inline modint<MOD> operator+(modint<MOD> other) const {
        int32_t c = this->value + other.value;
        return modint<MOD>(c >= MOD ? c - MOD : c);
    }
    inline modint<MOD> operator-(modint<MOD> other) const {
        int32_t c = this->value - other.value;
        return modint<MOD>(c < 0 ? c + MOD : c);
    }
    inline modint<MOD> operator*(modint<MOD> other) const {
        int32_t c = (int64_t)this->value * other.value % MOD;
        return modint<MOD>(c < 0 ? c + MOD : c);
    }

    inline modint<MOD> &operator+=(modint<MOD> other) {
        this->value += other.value;
        if (this->value >= MOD)
            this->value -= MOD;
        return *this;
    }

    inline modint<MOD> &operator-=(modint<MOD> other) {
        this->value -= other.value;
        if (this->value < 0)
            this->value += MOD;
        return *this;
    }

    inline modint<MOD> &operator*=(modint<MOD> other) {
        this->value = (int64_t)this->value * other.value %
                      MOD;
        if (this->value < 0) this->value += MOD;
        return *this;
    }
    inline modint<MOD> operator-() const { return modint<MOD>(this->value ? MOD - this->value : 0); }
    modint<MOD> pow(uint64_t k) const {
        modint<MOD> x =
                        *this,
                    y = 1;
        for (; k; k >>= 1) {
            if (k & 1) y *= x;
            x *= x;
        }
        return y;
    }

    modint<MOD> inv() const { return pow(MOD - 2); }  // MOD must be a prime
    inline modint<MOD> operator/(modint<MOD> other) const { return *this * other.inv(); }
    inline modint<MOD> operator/=(modint<MOD> other) { return *this *= other.inv(); }
    inline bool operator==(modint<MOD> other) const { return value == other.value; }
    inline bool operator!=(modint<MOD> other) const { return value != other.value; }
    inline bool operator<(modint<MOD> other) const { return value < other.value; }
    inline bool operator>(modint<MOD> other) const { return value > other.value; }
};
template <int32_t MOD>
modint<MOD> operator*(int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <int32_t MOD>
modint<MOD> operator*(int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }

template <int32_t MOD>
istream &operator>>(istream &in, modint<MOD> &n) { return in >> n.value; }
template <int32_t MOD>
ostream &operator<<(ostream &out, modint<MOD> n) { return out << n.value; }
using mint = modint<mod>;
namespace Dirichlet {
// solution for f(x) = phi(x)
const int T = 1e7 + 9;
long long phi[T];
gp_hash_table<long long, mint> mp;
mint dp[T], inv;
int sz, spf[T], prime[T];
void init() {
    memset(spf, 0, sizeof spf);
    phi[1] = 1;
    sz = 0;
    for (int i = 2; i < T; i++) {
        if (spf[i] == 0) phi[i] = i - 1, spf[i] = i, prime[sz++] = i;
        for (int j = 0; j < sz && i * prime[j] < T && prime[j] <= spf[i]; j++) {
            spf[i * prime[j]] = prime[j];
            if (i % prime[j] == 0)
                phi[i * prime[j]] = phi[i] * prime[j];
            else
                phi[i * prime[j]] = phi[i] * (prime[j] - 1);
        }
    }
    dp[0] = 0;
    for (int i = 1; i < T; i++) dp[i] = dp[i - 1] + phi[i] % mod;
    inv = 1;  // g(1)
}
mint p_c(long long n) {
    if (n % 2 == 0) return n / 2 % mod * ((n + 1) % mod) % mod;
    return (n + 1) / 2 % mod * (n % mod) % mod;
}
mint p_g(long long n) {
    return n % mod;
}
mint solve(long long x) {
    if (x < T) return dp[x];
    if (mp.find(x) != mp.end()) return mp[x];
    mint ans = 0;
    for (long long i = 2, last; i <= x; i = last + 1) {
        last = x / (x / i);
        ans += solve(x / i) * (p_g(last) - p_g(i - 1));
    }
    ans = p_c(x) - ans;
    ans /= inv;
    return mp[x] = ans;
}
};  // namespace Dirichlet