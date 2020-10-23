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

  int n, m, l;
  cin >> n >> m >> l;

  vector<ll> factorial(1001);
  factorial[0] = 1;
  for (int i = 1; i <= 1000; i++) factorial[i] = (factorial[i-1] * i) % mod;

  auto nPr = [&](ll n, ll r) {
    return (factorial[n] * modinv(factorial[n-r])) % mod;
  };

  vector<vector<vector<vector<ll> > > > dp(n+1, vector<vector<vector<ll> > >(n+1, vector<vector<ll> >(m+1, vector<ll>(2))));
  dp[0][0][0][0] = 1;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= i; j++) {
      cerr << i << " " << j << endl;
      for (int k = 0; k <= m; k++) {
        for (int r = 0; r < 2; r++) {
          if (dp[i-1][j-1][k][r] == 0) continue;
          dp[i][j][k][r] = (dp[i][j][k][r] + dp[i-1][j-1][k][r]) % mod;
          for (ll p = 2; p <= min(j, l); p++) {
            if (k+p-1 > m) break;
            int t = (p == l) ? 1 : r;
            dp[i][j-p][k+p-1][t] = (dp[i][j-p][k+p-1][t] + dp[i-1][j-1][k][r] * (((nPr(j-1, p-1) * p) % mod * modinv(2)) % mod)) % mod;
            if (k+p > m) continue;
            if (p == 2) dp[i][j-p][k+p][t] = (dp[i][j-p][k+p][t] + dp[i-1][j-1][k][r] * (((nPr(j-1, p-1)) % mod) % mod)) % mod;
            else dp[i][j-p][k+p][t] = (dp[i][j-p][k+p][t] + dp[i-1][j-1][k][r] * (((nPr(j-1, p-1)) % mod * modinv(2)) % mod)) % mod;
          }
        }
      }
    }
  }

  ll ans = 0;
  for (int i = 0; i < n; i++) {
    ans = (ans + dp[n][i][m][1]) % mod;
  }

  cout << ans << endl;

  return 0;
}
