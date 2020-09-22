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

  int t;
  cin >> t;
  vector<int> a(t);
  for (auto &x: a) cin >> x;

  vector<vector<ll> > dp(401, vector<ll>(601));
  dp[0][0] = 1;
  for (int i = 0; i < 400; i++) {
    if (i < t) {
      for (int j = 300; j >= 0; j--) {
        for (int k = 1; k <= a[i]; k++) {
          dp[i][j+k] = (dp[i][j+k] + dp[i][j]) % mod;
        }
      }
    }
    for (int j = 0; j <= 600; j += 2) {
      dp[i+1][j/2] = dp[i][j];
    }
  }

  ll ans = 0;
  for (int i = 0; i <= 400; i++) ans = (ans + dp[i][1]) % mod;

  cout << ans << endl;

  return 0;
}
