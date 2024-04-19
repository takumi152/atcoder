#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<ll> x(n), y(n);
  for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

  vector<vector<ll>> dp(n+1, vector<ll>(2, (ll) -9e18));
  dp[0][0] = 0;
  for (int i = 0; i < n; i++) {
    dp[i+1][0] = dp[i][0];
    dp[i+1][1] = dp[i][1];
    if (x[i] == 0) {
      dp[i+1][0] = max(dp[i+1][0], dp[i][0] + y[i]);
      dp[i+1][0] = max(dp[i+1][0], dp[i][1] + y[i]);
    }
    else {
      dp[i+1][1] = max(dp[i+1][1], dp[i][0] + y[i]);
    }
  }

  cout << max(dp[n][0], dp[n][1]) << endl;

  return 0;
}
