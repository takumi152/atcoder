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

  vector<ll> ncr_precalc(50);
  for (int i = 0; i < 50; i++) ncr_precalc[i] = ncr(n-1, i);

  vector<vector<ll> > count(m+1, vector<ll>(20));

  // auto dfs = [&](auto self, int prod, int len) {
  //   if (false) return;
  //
  //   count[prod][len]++;
  //
  //   int f = 2;
  //   while (prod * f <= m) {
  //     self(self, prod * f, len + 1);
  //     f++;
  //   }
  // };
  //
  // dfs(dfs, 1, 0);

  // count[1][0] = 1;
  // for (int i = 1; i <= m; i++) {
  //   for (int f = 2; i * f <= m; f++) {
  //     for (int j = 0; j < 18; j++) {
  //       count[i*f][j+1] += count[i][j];
  //     }
  //   }
  // }
  //
  // ll ans = 0;
  // for (int i = 1; i <= m; i++) {
  //   for (int j = 0; j < 20; j++) {
  //     ans = (ans + ((count[i][j] * ncr_precalc[j]) % mod) * (m / i)) % mod;
  //   }
  // }
  //
  // cout << ans << endl;

  auto dfs = [&](auto self, int prod, int len) {
    if (false) return 0;

    int num = 1;
    int f = 2;
    while (prod * f <= m) {
      num += self(self, prod * f, len + 1);
      f++;
    }

    return num;
  };

  cerr << dfs(dfs, 1, 0) << endl;;

  return 0;
}
