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
const ll inf = (ll) 9e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<string> s(m);
  vector<ll> c(m);
  for (int i = 0; i < m; i++) cin >> s[i] >> c[i];

  vector<int> mask(m);
  for (int i = 0; i < m; i++) {
    int k = 1;
    for (int j = 0; j < n; j++) {
      if (s[i][j] == 'Y') mask[i] |= k;
      k <<= 1;
    }
  }

  vector<vector<ll> > dp(m+2, vector<ll>((1 << n), inf));
  dp[0][0] = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < (1 << n); j++) {
      dp[i+1][j] = dp[i][j];
    }
    for (int j = 0; j < (1 << n); j++) {
      dp[i+1][j|mask[i]] = min(dp[i+1][j|mask[i]], dp[i][j] + c[i]);
    }
  }

  ll ans = dp[m][(1 << n) - 1];
  if (ans == inf) ans = -1;
  cout << ans << endl;

  return 0;
}
