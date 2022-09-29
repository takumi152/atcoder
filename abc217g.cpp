#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

#include <atcoder/convolution>

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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;

  vector<ll> factorial(n + 1);
  factorial[0] = 1;
  for (int i = 1; i <= n; i++) factorial[i] = (factorial[i-1] * i) % mod;

  vector<ll> dp(n + 1);
  dp[(n + m - 1) / m] = 1;
  for (int x = 0; x < 10; x++) cerr << dp[x] << " ";
  cerr << endl;
  for (int i = 1; i < m; i++) {
    vector<ll> dp_next(n + 1);
    int element = ((n - i) + m - 1) / m;
    for (int j = 0; j <= n; j++) {
      for (int k = 0; k <= element; k++) {
        if (j + k > n) continue;
        dp_next[j + k] = (dp_next[j + k] + (((dp[j] * ((((factorial[j] * modinv(factorial[j - k])) % mod) * modinv(factorial[k])) % mod)) % mod) * ((factorial[element] * modinv(factorial[k])) % mod)) % mod) % mod;
      }
    }
    dp = dp_next;
    for (int x = 0; x < 10; x++) cerr << dp[x] << " ";
    cerr << endl;
  }

  for (int i = 1; i <= n; i++) cout << dp[i] << endl;

  return 0;
}
