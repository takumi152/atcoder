#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <set>

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

  int n, a, b, p, q;
  cin >> n >> a >> b >> p >> q;

  // turn, takahashi_pos, aoki_pos
  vector<vector<vector<ll>>> dp(2, vector<vector<ll>>(n+1, vector<ll>(n+1)));
  dp[0][a][b] = 1;
  for (int k = 0; k < n; k++) {
    for (int j = 0; j < n; j++) {
      if (k+a < n-1) {
        for (int x = 1; x <= p; x++) {
          dp[1][min(n, k+a+x)][j] = (dp[1][min(n, k+a+x)][j] + dp[0][k+a][j]) % mod;
        }
      }
      else if (k+a == n-1) {
        dp[1][n][j] = (dp[1][n][j] + dp[0][k+a][j]) % mod;
      }
    }
    for (int i = 0; i < n; i++) {
      if (k+b < n-1) {
        for (int y = 1; y <= q; y++) {
          dp[0][i][min(n, k+b+y)] = (dp[0][i][min(n, k+b+y)] + dp[1][i][k+b]) % mod;
        }
      }
      else if (k+b == n-1) {
        dp[0][i][n] = (dp[0][i][n] + dp[1][i][k+b]) % mod;
      }
    }
  }

  ll total_case = 0;
  ll takahashi_case = 0;
  for (int j = 0; j < n; j++) {
    total_case = (total_case + dp[1][n][j]) % mod;
    takahashi_case = (takahashi_case + dp[1][n][j]) % mod;
  }
  for (int i = 0; i < n; i++) {
    total_case = (total_case + dp[0][i][n]) % mod;
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j <= n; j++) {
      for (int k = 0; k <= n; k++) {
        cerr << setw(4) << dp[i][j][k] << " ";
      }
      cerr << endl;
    }
    cerr << endl;
  }

  cout << (takahashi_case * modinv(total_case)) % mod << endl;

  return 0;
}
