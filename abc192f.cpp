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

  ll n, x;
  cin >> n >> x;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  ll ans = (ll) 9e18;
  for (int k = 1; k <= n; k++) {
    vector<vector<ll> > dp(k+1, vector<ll>(k, (ll) -1e18));
    dp[0][0] = 0;
    for (int i = 0; i < n; i++) {
      for (int j = k-1; j >= 0; j--) {
        for (int m = 0; m < k; m++) {
          ll next = dp[j][m] + a[i];
          dp[j+1][next%k] = max(dp[j+1][next%k], next);
        }
      }
    }
    if (dp[k][x%k] > 0) ans = min(ans, (x - dp[k][x%k]) / k);
  }

  cout << ans << endl;

  return 0;
}
