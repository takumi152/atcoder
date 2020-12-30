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

ll solve(int n, int l, int lim, vector<vector<ll> > &dp) {
  if (n > 12 || l > lim) return 0;
  if (dp[n][l] >= 0) return dp[n][l];
  ll ans = 0;
  if (n == 12 && l == lim) ans = 1;
  else if (n == 12) ans = 0;
  else {
    for (int i = l+1; i <= lim - (11 - n); i++) {
      ans += solve(n+1, i, lim, dp);
    }
  }
  dp[n][l] = ans;
  return ans;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  vector<vector<ll> > dp(13, vector<ll>(n+1, -1));
  ll ans = solve(0, 0, n, dp);

  cout << ans << endl;

  return 0;
}
