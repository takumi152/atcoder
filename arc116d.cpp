#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

ll modinv(ll x, ll m = mod) {
  ll b = m;
  ll u = 1;
  ll v = 0;
  ll tmp;
  while (b) {
    ll t = x / b;
    x -= t * b;
    tmp = x;
    x = b;
    b = tmp;
    u -= t * v;
    tmp = u;
    u = v;
    v = tmp;
  }
  u %= m;
  if (u < 0) u += m;
  return u;
}

ll ncr(ll n, ll r, ll m = mod) {
  ll x = 1;
  for (ll i = n; i > n - r; i--) x = (x * i) % m;
  for (ll i = 1; i <= r; i++) x = (x * modinv(i)) % m;
  return x;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m;
  cin >> n >> m;

  if (m % 2 != 0) {
    cout << 0 << endl;
    return 0;
  }

  vector<ll> ncr_precalc(5001);
  for (int i = 0; i <= 5000; i++) ncr_precalc[i] = ncr(n, i);

  vector<vector<ll> > dp(m+1, vector<ll>(16, -1));

  auto dfs = [&](auto self, int bit, int rem) {
    if (dp[rem][bit] >= 0) return dp[rem][bit];

    if (bit == 0) {
      if (rem == 0) dp[rem][bit] = 1;
      else dp[rem][bit] = 0;
    }
    else {
      dp[rem][bit] = 0;

      int onebit = 0;
      int next = rem;
      while (next >= 0) {
        dp[rem][bit] = (dp[rem][bit] + (ncr_precalc[onebit*2] * self(self, bit-1, next))) % mod;

        onebit++;
        next -= 1 << bit;
      }
    }

    return dp[rem][bit];
  };

  cout << dfs(dfs, 15, m) << endl;

  return 0;
}
