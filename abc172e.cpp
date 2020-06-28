// WIP

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

  ll n, m;
  cin >> n >> m;

  if (m == 1) {
    cout << 0 << endl;
    return 0;
  }

  vector<ll> factorial(1000001);
  factorial[0] = 1;
  for (ll i = 1; i <= 1000000; i++) {
    factorial[i] = (factorial[i-1] * i) % mod;
  }

  vector<ll> montmort(1000001);
  montmort[0] = 0;
  montmort[1] = m - 1;
  montmort[2] = ((m - 1) + (m - 2) * (m - 2)) % mod;
  for (int i = 3; i <= 1000000; i++) {
    montmort[i] = ((m - 1) * (montmort[i-1] + montmort[i-2])) % mod;
  }

  ll permutation_count = (factorial[m] * modinv(factorial[m - n])) % mod;
  ll ans = (permutation_count * montmort[m-1]) % mod;

  cout << ans << endl;

  return 0;
}
