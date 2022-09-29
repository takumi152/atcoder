#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m, k;
  cin >> n >> m >> k;
  vector<vector<int> > removed_edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    removed_edge[a-1].push_back(b-1);
    removed_edge[b-1].push_back(a-1);
  }

  vector<vector<ll> > dp(k+1, vector<ll>(n));
  dp[0][0] = 1;
  for (int i = 0; i < k; i++) {
    ll total = 0;
    for (int j = 0; j < n; j++) total = (total + dp[i][j]) % mod;
    for (int j = 0; j < n; j++) {
      dp[i+1][j] = total - dp[i][j];
      if (dp[i+1][j] < 0) dp[i+1][j] += mod;
    }
    for (int j = 0; j < n; j++) {
      for (auto &x: removed_edge[j]) {
        dp[i+1][j] -= dp[i][x];
        if (dp[i+1][j] < 0) dp[i+1][j] += mod;
      }
    }
  }

  cout << dp[k][0] << endl;

  return 0;
}
