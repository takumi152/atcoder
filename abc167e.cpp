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

  ll n, m, k;
  cin >> n >> m >> k;

  vector<ll> factorial(400001);
  factorial[0] = 1;
  for (int i = 1; i <= 400000; i++) factorial[i] = (factorial[i-1] * i) % mod;

  vector<ll> n1ci(k+1);
  for (int i = 0; i <= k; i++) n1ci[i] = (((factorial[n-1] * modinv(factorial[(n-1)-i])) % mod) * modinv(factorial[i])) % mod;

  ll ans = 0;
  for (int i = 0; i <= k; i++) {
    ans = (ans + (((n1ci[i] * m) % mod) * modpow(m - 1, n - i - 1)) % mod) % mod;
  }

  cout << ans << endl;

  return 0;
}
