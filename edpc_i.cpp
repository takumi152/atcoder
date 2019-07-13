#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<double> p(N);
  for (auto &x: p) cin >> x;

  vector<vector<double> > dp(N+2, vector<double>(N+2));
  dp[0][0] = 1.0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j <= i; j++) {
      dp[i+1][j] += dp[i][j] * (1.0 - p[i]);
      dp[i+1][j+1] += dp[i][j] * p[i];
    }
  }

  double prob = 0.0;
  for (int j = N/2+1; j <= N; j++) prob += dp[N][j];

  cout << setprecision(16) << prob << endl;

  return 0;
}
