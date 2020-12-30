#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <numeric>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<ll> n(t), s(t), k(t);
  for (int i = 0; i < t; i++) cin >> n[i] >> s[i] >> k[i];

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    ll g = gcd(n[i], k[i]);
    if (s[i] % g != 0) {
      ans[i] = -1;
      continue;
    }
    n[i] /= g;
    s[i] /= g;
    k[i] /= g;
    ans[i] = ((n[i] - s[i]) * modinv(k[i], n[i])) % n[i];
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
