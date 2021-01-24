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

  int n;
  cin >> n;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  vector<vector<ll> > dp(n+1, vector<ll>(2));
  dp[0][0] = 1;
  dp[0][1] = 1;
  for (int i = 0; i < n; i++) {
    if (s[i] == "AND") {
      dp[i+1][0] = dp[i][0] * 2 + dp[i][1];
      dp[i+1][1] = dp[i][1];
    }
    else {
      dp[i+1][0] = dp[i][0];
      dp[i+1][1] = dp[i][0] + dp[i][1] * 2;
    }
  }

  cout << dp[n][1] << endl;

  return 0;
}
