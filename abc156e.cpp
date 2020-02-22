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

  ll n, k;
  cin >> n >> k;

  vector<ll> factorial(400000);
  factorial[0] = 1;
  for (int i = 1; i < 400000; i++) {
    factorial[i] = (factorial[i-1] * i) % mod;
  }

  vector<ll> ncr_box(n+1);
  ncr_box[0] = 1;
  for (int i = 1; i <= n; i++) {
    ncr_box[i] = (factorial[n] * modinv((factorial[i] * factorial[n-i]) % mod)) % mod;
  }

  vector<ll> ncr_human(n+1);
  ncr_human[0] = 0;
  for (int i = 1; i <= n; i++) {
    ncr_human[i] = (((factorial[(n-1)] * modinv((factorial[(i-1)] * factorial[(n-1)-(i-1)]) % mod)) % mod) * ncr_box[i]) % mod;
  }

  ll ans = 0;
  for (int i = n; i > max(0LL, n-k-1); i--) {
    ans = (ans + ncr_human[i]) % mod;
  }

  cout << ans << endl;

  return 0;
}
