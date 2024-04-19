#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

inline double euclid_distance(double x1, double y1, double x2, double y2) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<double> x(n), y(n);
  for (int i = 0; i < n; i++) cin >> x[i] >> y[i];
  vector<double> p(m), q(m);
  for (int i = 0; i < m; i++) cin >> p[i] >> q[i];

  const vector<double> speed_multi = {1.0, 2.0, 4.0, 8.0, 16.0, 32.0};

  vector<vector<double>> dp(1 << (n + m), vector<double>(n + m, 1e100));
  for (int i = 0; i < n + m; i++) dp[1 << i][i] = euclid_distance(0.0, 0.0, i < n ? x[i] : p[i-n], i < n ? y[i] : q[i-n]);
  for (int i = 1; i < (1 << (n + m)); i++) {
    for (int j = 0; j < n + m; j++) {
      if (!(i & (1 << j))) continue;
      for (int k = 0; k < n + m; k++) {
        if (i & (1 << k)) continue;
        dp[i|(1<<k)][k] = min(dp[i|(1<<k)][k], dp[i][j] + euclid_distance(j < n ? x[j] : p[j-n], j < n ? y[j] : q[j-n], k < n ? x[k] : p[k-n], k < n ? y[k] : q[k-n]) / speed_multi[__builtin_popcount(i & ~((1 << n) - 1))]);
      }
    }
  }

  double ans = 1e100;
  for (int i = (1 << n) - 1; i < (1 << (n + m)); i += (1 << n)) {
    for (int j = 0; j < n + m; j++) {
      ans = min(ans, dp[i][j] + euclid_distance(0.0, 0.0, j < n ? x[j] : p[j-n], j < n ? y[j] : q[j-n]) / speed_multi[__builtin_popcount(i & ~((1 << n) - 1))]);
    }
  }

  cout << setprecision(20) << ans << endl;

  return 0;
}
