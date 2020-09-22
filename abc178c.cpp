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

  vector<vector<ll> > dp(n+2, vector<ll>(4));
  dp[0][0] = 1;
  for (int i = 0; i < n; i++) {
    dp[i+1][0] = (dp[i][0] * 8) % mod;
    dp[i+1][1] = (dp[i][0] * 1 + dp[i][1] * 9) % mod;
    dp[i+1][2] = (dp[i][0] * 1 + dp[i][2] * 9) % mod;
    dp[i+1][3] = (dp[i][1] * 1 + dp[i][2] * 1 + dp[i][3] * 10) % mod;
  }

  cout << dp[n][3] << endl;

  return 0;
}
