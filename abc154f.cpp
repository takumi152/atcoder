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

  ll r1, c1, r2, c2;
  cin >> r1 >> c1 >> r2 >> c2;
  r1--;
  c1--;

  vector<ll> factorial(5000000);
  factorial[0] = 1;
  factorial[1] = 1;
  for (int i = 2; i < 5000000; i++) factorial[i] = (factorial[i-1] * i) % mod;

  ll r1c1 = ((((c1 + 2) * ((factorial[r1+c1+2] * modinv((factorial[r1] * factorial[c1+2]) % mod)) % mod)) % mod - r1 - 1) * modinv(r1 + 1)) % mod;
  ll r2c1 = ((((c1 + 2) * ((factorial[r2+c1+2] * modinv((factorial[r2] * factorial[c1+2]) % mod)) % mod)) % mod - r2 - 1) * modinv(r2 + 1)) % mod;
  ll r1c2 = ((((c2 + 2) * ((factorial[r1+c2+2] * modinv((factorial[r1] * factorial[c2+2]) % mod)) % mod)) % mod - r1 - 1) * modinv(r1 + 1)) % mod;
  ll r2c2 = ((((c2 + 2) * ((factorial[r2+c2+2] * modinv((factorial[r2] * factorial[c2+2]) % mod)) % mod)) % mod - r2 - 1) * modinv(r2 + 1)) % mod;

  ll ans = (r2c2 - r1c2 - r2c1 + r1c1) % mod;
  if (ans < 0) ans += mod;

  cout << ans << endl;

  return 0;
}
