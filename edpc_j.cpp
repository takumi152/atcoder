// WIP

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdlib>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<double> a(N);
  for (auto &x: a) cin >> x;

  // dp[i][j][k] := i回目の操作後にj個乗った皿がk枚ある確率
  vector<int> count(4);
  for (int i = 0; i < N; i++) count[a[i]]++;
  int total = accumulate(count.begin(), count.end(), 0);
  vector<vector<vector<double> > > dp(total+2, vector<vector<double> >(3+1, vector<double>(N+2)));
  for (int j = 0; j < 4; i++) dp[0][j][a[j]] = 1.0;
  for (int i = 1; i <= total; i++) {
    for (int k = N; k >= 1; k--) {
      for (int j = 3; j >= 1; j--) {
        dp[i][j][k] += dp[i-1][j][k] - dp[i-1][j][k] / max(1, total - j - k + 1);
        for (int l = N; l >= 0; l--) dp[i][l][k-1] += dp[i-1][l][k-1] * dp[i-1][j][k] / max(1, total - j - k + 1);
        //dp[i][j][k] = dp[i][j-1][k] + dp[i][j-1][k+1] / max(1, total - j - k + 1);
        //dp[i][j][k+1] -= dp[i][j-1][k+1] / max(1, total - j - k + 1);
      }
    }
  }

  cout << setprecision(16) << expc << endl;

  return 0;
}
