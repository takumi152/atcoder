#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s, t;
  cin >> s;
  cin >> t;

  int n = s.size(), m = t.size();
  vector<vector<int> > dp(n+2, vector<int>(m+2));
  //for (int j = 0; j < m+2; j++) dp[0][j] = j;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      dp[i+1][j+1] = max(max(dp[i+1][j], dp[i][j+1]), dp[i][j] + (int)(s[i] == t[j]));
    }
  }

  int x = n-1, y = m-1;
  string ans;
  while (x >= 0 && y >= 0) {
    if (s[x] == t[y]) {
       ans += s[x];
       x--;
       y--;
     }
     else if (dp[x+1][y+1] == dp[x][y+1]) x--;
     else y--;
  }

  reverse(ans.begin(), ans.end());
  cout << ans << endl;

  return 0;
}
