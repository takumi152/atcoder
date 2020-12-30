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

  int a, b, c;
  cin >> a >> b >> c;

  vector<vector<vector<double> > > dp(101, vector<vector<double> >(101, vector<double>(101)));
  dp[a][b][c] = 1.0;
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      for (int k = 0; k < 100; k++) {
        if (i == 0 && j == 0 && k == 0) continue;
        dp[i+1][j][k] += dp[i][j][k] * ((double) i / (double) (i + j + k));
        dp[i][j+1][k] += dp[i][j][k] * ((double) j / (double) (i + j + k));
        dp[i][j][k+1] += dp[i][j][k] * ((double) k / (double) (i + j + k));
      }
    }
  }

  double ans = 0.0;
  for (int i = 0; i <= 100; i++) {
    for (int j = 0; j <= 100; j++) {
      for (int k = 0; k <= 100; k++) {
        if (!(i == 100 || j == 100 || k == 100)) continue;
        ans += (double) ((i + j + k) - (a + b + c)) * dp[i][j][k];
      }
    }
  }

  cout << setprecision(16) << ans << endl;

  return 0;
}
