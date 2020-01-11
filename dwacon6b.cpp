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

  int n;
  cin >> n;
  vector<ll> x(n);
  for (auto &y: x) cin >> y;

  vector<ll> factorial(n+1);
  factorial[0] = 1;
  for (int i = 1; i <= n; i++) factorial[i] = (factorial[i-1] * i) % mod;

  vector<ll> mul(n-1);
  mul[0] = factorial[n-1];
  for (int i = 1; i < n-1; i++) {
    mul[i] = (mul[i-1] + (factorial[n-1] * modinv(i+1)) % mod) % mod;
  }

  ll total = 0;
  for (int i = 0; i < n-1; i++) {
    total = (total + ((x[i+1] - x[i]) * mul[i]) % mod) % mod;
  }

  cout << total << endl;

  return 0;
}
