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
  string s;
  cin >> s;

  vector<int> c(n);
  for (int i = 0; i < n; i++) c[i] = s[i] - '0';

  vector<vector<int> > dp(n+2, vector<int>(1000));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 1000; j++) dp[i+1][j] = dp[i][j];
    for (int j = c[i] * 100; j < c[i] * 100 + 100; j++) {
      if (dp[i][j] == 0) dp[i+1][j] = 1;
    }
    for (int j = c[i] * 10; j < 1000; j += 100) {
      for (int k = j; k < j + 10; k++) {
        if (dp[i][k] == 1) dp[i+1][k] = 2;
      }
    }
    for (int j = c[i]; j < 1000; j += 10) {
      if (dp[i][j] == 2) dp[i+1][j] = 3;
    }
  }

  int ans = 0;
  for (int i = 0; i < 1000; i++) {
    if (dp[n][i] == 3) ans++;
  }

  cout << ans << endl;

  return 0;
}
