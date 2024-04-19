#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<ll> a(n);
  for (auto &y: a) cin >> y;
  vector<ll> c(n);
  for (auto &y: c) cin >> y;
  vector<ll> x(m);
  for (auto &y: x) cin >> y;

  vector<bool> is_in_wishlist(n);
  for (auto &y: x) is_in_wishlist[y-1] = true;

  vector<vector<ll>> c_min(n, vector<ll>(n+1, 9e18));
  {
    for (int i = 0; i < n; i++) {
      for (int j = i; j < n; j++) {
        c_min[i][j+1] = min(c_min[i][j], c[j]);
      }
    }
  }

  vector<vector<ll>> dp(n+1, vector<ll>(n+1, 9e18));
  dp[0][0] = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i+1; j++) {
      if (!is_in_wishlist[i]) dp[i+1][j] = min(dp[i+1][j], dp[i][j]);
      dp[i+1][j+1] = min(dp[i+1][j+1], dp[i][j] + a[i] + c_min[i-j][i+1]);
    }
  }
  // for (auto &x: dp) {
  //   for (auto &y: x) {
  //     cout << setw(4) << y << " ";
  //   }
  //   cout << endl;
  // }

  ll ans = 9e18;
  for (int j = 0; j <= n; j++) ans = min(ans, dp[n][j]);

  cout << ans << endl;

  return 0;
}
