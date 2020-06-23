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

  int a, b, c, d;
  cin >> a >> b >> c >> d;

  vector<vector<ll> > dp(c+1, vector<ll>(d+1, 1));
  for (int i = a+1; i <= c; i++) dp[i][b] = (dp[i-1][b] * b) % mod;
  for (int j = b+1; j <= d; j++) dp[a][j] = (dp[a][j-1] * a) % mod;
  for (int i = a+1; i <= c; i++) {
    for (int j = b+1; j <= d; j++) {
      dp[i][j] = (((dp[i-1][j] + dp[i][j-1]) % mod) + ((((dp[i-1][j] * dp[i][j-1]) % mod) * modinv(dp[i-1][j-1])) % mod)) % mod;
    }
  }

  cout << dp[c][d] << endl;

  return 0;
}
