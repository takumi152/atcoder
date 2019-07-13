#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, W;
  cin >> N >> W;
  vector<ll> w, v;
  for (int i = 0; i < N; i++) {
    int x, y;
    cin >> x >> y;
    w.push_back(x);
    v.push_back(y);
  }

  vector<vector<ll> > dp(N+2, vector<ll>(W+2));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j <= W; j++) {
      if (w[i] > j) dp[i+1][j] = dp[i][j];
      else dp[i+1][j] = max(dp[i][j], v[i] + dp[i][j - w[i]]);
    }
  }

  cout << dp[N][W] << endl;
  return 0;
}
