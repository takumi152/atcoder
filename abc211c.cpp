#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;

  int n = s.size();

  vector<vector<ll> > dp(n+1, vector<ll>(9));
  dp[0][0] = 1;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 9; j++) dp[i+1][j] = dp[i][j];
    if (s[i] == 'c') dp[i+1][1] = (dp[i][0] + dp[i+1][1]) % mod;
    if (s[i] == 'h') dp[i+1][2] = (dp[i][1] + dp[i+1][2]) % mod;
    if (s[i] == 'o') dp[i+1][3] = (dp[i][2] + dp[i+1][3]) % mod;
    if (s[i] == 'k') dp[i+1][4] = (dp[i][3] + dp[i+1][4]) % mod;
    if (s[i] == 'u') dp[i+1][5] = (dp[i][4] + dp[i+1][5]) % mod;
    if (s[i] == 'd') dp[i+1][6] = (dp[i][5] + dp[i+1][6]) % mod;
    if (s[i] == 'a') dp[i+1][7] = (dp[i][6] + dp[i+1][7]) % mod;
    if (s[i] == 'i') dp[i+1][8] = (dp[i][7] + dp[i+1][8]) % mod;
  }

  cout << dp[n][8] << endl;

  return 0;
}
