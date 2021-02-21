#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

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

  ll n, m, k;
  cin >> n >> m >> k;

  if (n == 1 && m == 1) {
    cout << k << endl;
    return 0;
  }
  else if (n == 1 || m == 1) {
    ll ans = 1;
    for (ll i = 2; i <= k; i++) {
      ll c = modpow(i, max(n, m)) - modpow(i-1, max(n, m));
      if (c < 0) c += mod;
      ans = (ans + c) % mod;
    }
    cout << ans << endl;
    return 0;
  }
  else {
    ll ans = modpow(k, m);
    for (ll i = 2; i <= k; i++) {
      ll cr = modpow(i, n) - modpow(i-1, n);
      if (cr < 0) cr += mod;
      ll cc = modpow(k-i+1, m);
      ans = (ans + cr * cc) % mod;
    }
    cout << ans << endl;
    return 0;
  }

  return 0;
}
