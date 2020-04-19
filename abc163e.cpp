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
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  vector<ll> idx(n);
  for (int i = 0; i < n; i++) idx[i] = i;
  sort(idx.rbegin(), idx.rend(), [&](auto &x, auto &y){return a[x] < a[y];});

  vector<vector<ll> > dp(n+1, vector<ll>(n+1));
  dp[0][0] = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= i; j++) {
      dp[j+1][i-j] = max(dp[j+1][i-j], dp[j][i-j] + a[idx[i]] * abs(idx[i] - j));
      dp[j][i-j+1] = max(dp[j][i-j+1], dp[j][i-j] + a[idx[i]] * abs(idx[i] - (n-(i-j)-1)));
    }
  }

  ll ans = 0;
  for (int i = 0; i <= n; i++) {
    if (dp[i][n-i] > ans) ans = dp[i][n-i];
  }

  cout << ans << endl;

  return 0;
}
