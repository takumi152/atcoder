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
  vector<string> s(5);
  for (auto &x: s) cin >> x;

  vector<vector<int> > si(5, vector<int>(n));
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < n; j++) {
      if (s[i][j] == 'R') si[i][j] = 0;
      else if (s[i][j] == 'B') si[i][j] = 1;
      else if (s[i][j] == 'W') si[i][j] = 2;
      else si[i][j] = 3;
    }
  }

  vector<vector<int> > dp(n+2, vector<int>(3, 1000000007));
  dp[0][0] = 0;
  dp[0][1] = 0;
  dp[0][2] = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        if (j == k) continue;
        int now = dp[i][j];
        for (int l = 0; l < 5; l++) {
          if (si[l][i] != k) now++;
        }
        dp[i+1][k] = min(dp[i+1][k], now);
      }
    }
  }

  cout << min(min(dp[n][0], dp[n][1]), dp[n][2]) << endl;

  return 0;
}
