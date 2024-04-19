#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_set>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

inline double euclidean_distance(int x1, int y1, int x2, int y2) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<Pii> xy(n);
  for (auto &[x, y]: xy) cin >> x >> y;

  vector<vector<double>> dp(n, vector<double>(30, 1e100));
  dp[0][0] = 0.0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 30; j++) {
      for (int k = 0; (i + 1) + k < n && j + k < 30; k++) {
        dp[(i + 1) + k][j + k] = min(dp[(i + 1) + k][j + k], dp[i][j] + euclidean_distance(xy[i].first, xy[i].second, xy[(i + 1) + k].first, xy[(i + 1) + k].second));
      }
    }
  }

  double ans = 1e308;
  for (int i = 0; i < 30; i++) {
    double penalty = (i == 0) ? 0 : pow(2.0, (double) i - 1.0);
    double score = dp[n - 1][i] + penalty;
    if (score < ans) ans = score;
  }

  cout << setprecision(20) << ans << endl;

  return 0;
}
