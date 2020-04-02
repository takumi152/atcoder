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
  string a, b, c;
  cin >> a >> b >> c;

  vector<vector<int> > dp(n+2, vector<int>(26));
  for (int i = 0; i < n; i++) {
    int minimum = 1000000007;
    for (int j = 0; j < 26; j++) minimum = min(minimum, dp[i][j]);
    for (int j = 0; j < 26; j++) {
      dp[i+1][j] = minimum;
      if (a[i] != 'a'+j) dp[i+1][j]++;
      if (b[i] != 'a'+j) dp[i+1][j]++;
      if (c[i] != 'a'+j) dp[i+1][j]++;
    }
  }
  int ans = 1000000007;
  for (int j = 0; j < 26; j++) ans = min(ans, dp[n][j]);

  cout << ans << endl;

  return 0;
}
