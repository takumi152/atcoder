// WIP

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

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

  int n;
  cin >> n;
  vector<ll> c(n);
  for (auto &x: c) cin >> x;

  sort(c.begin(), c.end());

  ll ans = 0;
  for (int i = 0; i < n; i++) {
    ans = ((modpow(2, (n-i)-1) * (n + 2)) % mod * c[i]) % mod;
  }
  ans = (ans + c[n-1]) % mod;
  ans = ((ans * modpow(2, n)) % mod * (modpow(2, n) - 1) % mod) % mod;

  cout << ans << endl;

  return 0;
}
