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

  vector<vector<ll> > dp(N+2, vector<ll>(N * 1000 + 2, (ll) 9e18));
  for (int i = 0; i <= N; i++) dp[i][0] = 0;
  for (int j = 1; j < N * 1000 + 2; j++) {
    for (int i = 0; i < N; i++) {
      if (v[i] > j) dp[i+1][j] = min(dp[i][j], w[i]);
      else dp[i+1][j] = min(dp[i][j], dp[i][j - v[i]] + w[i]);
    }
  }

  int limit = 0;
  while (dp[N][limit+1] <= W) limit++;
  cout << limit << endl;
  return 0;
}
