#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

unordered_map<ll, int> primeFactorize(ll a) {
  ll i = 2;
  unordered_map<ll, int> factor;
  while (i * i <= a) {
    if (a % i == 0) {
      factor[i]++;
      a /= i;
    }
    else i++;
  }
  factor[a]++;
  return factor;
}

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

  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  vector<unordered_map<ll, int> > primeFactor(n);
  for (int i = 0; i < n; i++) primeFactor[i] = primeFactorize(a[i]);

  unordered_map<ll, int> primeMax;
  for (auto &x: primeFactor) {
    for (auto &y: x) primeMax[y.first] = max(primeMax[y.first], y.second);
  }

  ll target = 1;
  for (auto &x: primeMax) {
    for (int j = 0; j < x.second; j++) target = (target * x.first) % mod;
  }

  vector<ll> b(n, 1);
  for (int i = 0; i < n; i++) b[i] = (target * modinv(a[i])) % mod;

  ll ans = 0;
  for (auto &x: b) ans = (ans + x) % mod;

  cout << ans << endl;

  return 0;
}
