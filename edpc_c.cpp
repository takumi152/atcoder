#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a, b, c;
  for (int i = 0; i < n; i++) {
    int x, y, z;
    cin >> x >> y >> z;
    a.push_back(x);
    b.push_back(y);
    c.push_back(z);
  }

  vector<vector<int> > dp(n+2, vector<int>(3));
  for (int i = 0; i < n; i++) {
    dp[i+1][0] = a[i] + max(dp[i][1], dp[i][2]);
    dp[i+1][1] = b[i] + max(dp[i][0], dp[i][2]);
    dp[i+1][2] = c[i] + max(dp[i][0], dp[i][1]);
  }

  cout << max(max(dp[n][0], dp[n][1]), dp[n][2]) << endl;
  return 0;
}
