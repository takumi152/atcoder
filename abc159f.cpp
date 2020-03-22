#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, s;
  cin >> n >> s;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<vector<ll> > dp(n+1, vector<ll>(s+1));
  dp[0][0] = 1;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= s; j++) {
      if (j+a[i] <= s) dp[i+1][j+a[i]] = (dp[i][j] + dp[i+1][j+a[i]]) % mod;
      dp[i+1][j] = (dp[i][j] + dp[i+1][j]) % mod;
    }
    if (a[i] <= s) dp[i+1][a[i]] = (dp[i+1][a[i]] + i) % mod;
  }

  ll ans = 0;
  for (int i = 0; i < n; i++) {
    ans = (ans + dp[i+1][s]) % mod;
  }

  cout << ans << endl;

  return 0;
}
