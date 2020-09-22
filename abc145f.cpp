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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  vector<ll> h(n);
  for (auto &x: h) cin >> x;

  h.push_back(0);

  vector<vector<ll> > dp(n+2, vector<ll>(k+1, (ll) 9e18));
  for (int j = k; j >= 0; j--) dp[0][j] = 0;
  for (int i = 0; i < n; i++) {
    for (int j = k; j >= 0; j--) {
      if (i == 0) dp[i+1][j] = h[i];
      else dp[i+1][j] = min(dp[i+1][j], dp[i][j] + max(0LL, h[i] - h[i-1]));

      for (int l = 1; l <= j; l++) {
        if (i+l+1 > n+1) break;

        if (i == 0) dp[i+l+1][j-l] = h[i+l];
        else dp[i+l+1][j-l] = min(dp[i+l+1][j-l], dp[i][j] + max(0LL, h[i+l] - h[i-1]));

        if (i+l+1 == n+1) {
          dp[n][j-l] = min(dp[n][j-l], dp[n+1][j-l]);
        }
      }
    }
  }

  ll ans = (ll) 9e18;
  for (int j = k; j >= 0; j--) ans = min(ans, dp[n][j]);

  cout << ans << endl;

  return 0;
}
