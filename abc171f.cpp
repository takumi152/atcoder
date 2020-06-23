#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

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

  ll k;
  cin >> k;
  string s;
  cin >> s;

  ll n = s.size();

  vector<ll> factorial(2000001);
  factorial[0] = 1;
  for (ll i = 1; i <= 2000000; i++) {
    factorial[i] = (factorial[i-1] * i) % mod;
  }

  vector<ll> kn1ck(k+1);
  for (ll i = 0; i <= k; i++) {
    kn1ck[i] = (factorial[i+n-1] * (modinv((factorial[n-1] * factorial[i]) % mod))) % mod;
  }

  ll factor = 1;
  for (int i = 0; i < k; i++) {
    factor = (factor * 26) % mod;
  }

  ll ans = 0;
  for (int i = 0; i <= k; i++) {
    ans = (ans + (factor * kn1ck[i]) % mod) % mod;
    factor = (((factor * modinv(26)) % mod) * 25) % mod;
  }

  cout << ans << endl;

  return 0;
}
