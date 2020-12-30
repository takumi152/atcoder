#include <iostream>
#include <iomanip>
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

ll ncr(ll n, ll r, ll m = mod) {
  ll x = 1;
  for (ll i = n; i > n - r; i--) x = (x * i) % m;
  for (ll i = 1; i <= r; i++) x = (x * modinv(i)) % m;
  return x;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll d;
  cin >> d;

  cout << ncr(2*d-1, d-1) << endl;

  return 0;
}
