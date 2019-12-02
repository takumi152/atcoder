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

  int n;
  cin >> n;
  vector<int> s(n);
  for (auto &x: s) cin >> x;

  vector<vector<int> > dp(n+2, vector<int>(10, -1000000007));
  dp[0][0] = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 10; j++) dp[i+1][j] = dp[i][j];
    for (int j = 0; j < 10; j++) {
      dp[i+1][(dp[i][j] + s[i]) % 10] = max(dp[i+1][(dp[i][j] + s[i]) % 10], dp[i][j] + s[i]);
    }
  }

  int ans = 0;
  for (int i = 1; i < 10; i++) ans = max(ans, dp[n][i]);

  cout << ans << endl;

  return 0;
}
