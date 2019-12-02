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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, ma, mb;
  cin >> n >> ma >> mb;
  vector<int> a(n), b(n), c(n);
  for (int i = 0; i < n; i++) cin >> a[i] >> b[i] >> c[i];

  const int inf = 1000000007;

  vector<vector<int> > dp(411, vector<int>(411, inf));
  dp[0][0] = 0;
  for (int k = 0; k < n; k++) {
    for (int i = 400; i >= 0; i--) {
      for (int j = 400; j >= 0; j--) {
        dp[i+a[k]][j+b[k]] = min(dp[i+a[k]][j+b[k]], dp[i][j] + c[k]);
      }
    }
  }

  int alpha = ma;
  int beta = mb;
  int ans = inf;
  while (alpha <= 400 && beta <= 400) {
    ans = min(ans, dp[alpha][beta]);
    alpha += ma;
    beta += mb;
  }

  if (ans == inf) cout << -1 << endl; // impossible
  else cout << ans << endl;

  return 0;
}
