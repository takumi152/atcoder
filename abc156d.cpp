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

  ll n, a, b;
  cin >> n >> a >> b;

  ll comb = modpow(2, n) - 1;

  if (comb < 0) comb += mod;

  vector<ll> factorial(400000);
  factorial[0] = 1;
  for (int i = 1; i < 400000; i++) {
    factorial[i] = (factorial[i-1] * i) % mod;
  }

  vector<ll> ncr(400000);
  ncr[0] = 1;
  for (int i = 1; i < 400000; i++) {
    ncr[i] = (((((ncr[i-1] * (n - i + 1)) % mod) * factorial[i-1]) % mod) * modinv(factorial[i])) % mod;
  }

  comb -= ncr[a];
  if (comb < 0) comb += mod;
  comb -= ncr[b];
  if (comb < 0) comb += mod;

  cout << comb << endl;

  return 0;
}
