#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

unordered_map<ll, int> prime_factorize(ll x) {
  ll i = 2;
  unordered_map<ll, int> factor;
  while (x % i == 0 && i * i <= x) { // i == 2
    factor[i]++;
    x /= i;
  }
  i++;
  while (x % i == 0 && i * i <= x) { // i == 3
    factor[i]++;
    x /= i;
  }
  i += 2;
  while (i * i <= x) { // i >= 5
    while (x % i == 0 && i * i <= x) {
      factor[i]++;
      x /= i;
    }
    i += 2;
    while (x % i == 0 && i * i <= x) {
      factor[i]++;
      x /= i;
    }
    i += 4;
  }
  factor[x]++;
  return factor;
}

ll modpow(ll a, ll b, ll m = mod) {
  ll r = 1;
  while (b > 0) {
    if (b & 1) r = (r * a) % m;
    a = (a * a) % m;
    b >>= 1;
  }
  return r;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m;
  cin >> n >> m;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  unordered_map<ll, int> m_factor = prime_factorize(m);
  int factor_num = m_factor.size();
  vector<bool> can_choose(n, true);
  vector<int> factor_bit(n);
  for (int i = 0; i < n; i++) {
    if (m == 1LL) {
      can_choose[i] = (a[i] == 1LL) ? true : false;
      factor_bit[i] = (a[i] == 1LL) ? 1 : 0;
      continue;
    }

    ll val = a[i];
    int b = 0;
    for (auto &[f, c]: m_factor) {
      int div_count = 0;
      while (val % f == 0) {
        val /= f;
        div_count++;
      }
      if (div_count == c) {
        factor_bit[i] |= (1 << b);
      }
      else if (div_count > c) {
        can_choose[i] = false;
        break;
      }
      b++;
    }

    if (val != 1) can_choose[i] = false;
  }

  vector<ll> num_elements(1 << factor_num, 0);
  for (int i = 0; i < n; i++) {
    if (can_choose[i]) {
      num_elements[factor_bit[i]]++;
    }
  }

  vector<ll> dp(1 << factor_num, 0);
  vector<ll> dp_next(1 << factor_num, 0);
  dp[0] = 1;
  for (int i = 0; i < (1 << factor_num); i++) {
    for (int j = 0; j < (1 << factor_num); j++) {
      dp_next[j] = dp[j];
    }
    if (num_elements[i] > 0) {
      for (int j = 0; j < (1 << factor_num); j++) {
        dp_next[j | i] += dp[j] * (modpow(2, num_elements[i]) - 1LL);
        dp_next[j | i] %= mod;
      }
    }
    dp.swap(dp_next);
  }

  ll ans = dp[(1 << factor_num) - 1];

  cout << ans << endl;

  return 0;
}
