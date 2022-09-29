#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

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

  ll n, k;
  cin >> n >> k;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  // 1以外を入れたときにあまる1の数はどのような入れ方でも同じ
  // よって(余った1の入れ方) * (1以外の入れ方)で求まる

  ll spare_one = a[0] - k;
  for (int i = 1; i < n; i++) spare_one -= a[i];
  if (spare_one < 0) {
    cout << 0 << endl; // impossible
    return 0;
  }

  ll comb_one = ncr(spare_one + k - 1, k - 1);

  ll comb_rest = 1;
  for (int i = 1; i < n; i++) {
    comb_rest = (comb_rest * ncr(a[i] + k - 1, k - 1)) % mod;
  }

  ll ans = (comb_one * comb_rest) % mod;

  cout << ans << endl;

  return 0;
}
