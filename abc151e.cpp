#include <iostream>
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

  int n, k;
  cin >> n >> k;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  k--;
  sort(a.begin(), a.end());

  vector<ll> factorial(n*2+2);
  factorial[0] = 1;
  for (int i = 1; i < 2 * n; i++) factorial[i] = (factorial[i-1] * i) % mod;
  vector<ll> nck(n*2+2);
  nck[k] = 1;
  for (int i = k+1; i < 2 * n; i++) nck[i] = (factorial[i] * modinv((factorial[k] * factorial[i-k]) % mod)) % mod;

  vector<ll> maxfactor(n);
  for (int i = 0; i < n; i++) {
    maxfactor[i] = nck[i];
  }
  vector<ll> minfactor = maxfactor;
  reverse(minfactor.begin(), minfactor.end());

  ll mintotal = 0;
  ll maxtotal = 0;
  for (int i = 0; i < n; i++) {
    mintotal = (mintotal + (a[i] * minfactor[i]) % mod) % mod;
    maxtotal = (maxtotal + (a[i] * maxfactor[i]) % mod) % mod;
  }

  ll ans = (maxtotal - mintotal) % mod;
  if (ans < 0) ans += mod;

  cout << ans << endl;

  return 0;
}
