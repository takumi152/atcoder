#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  string s;
  cin >> s;

  vector<vector<vector<ll>>> dp(n+1, vector<vector<ll>>(26, vector<ll>(26)));
  dp[0][0][0] = 1;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 26; j++) {
      for (int k = 0; k < 26; k++) {
        if (j == k && i >= 2) continue;
        for (int l = 0; l < 26; l++) {
          if ((j == l && i >= 2) || (k == l && i >= 1)) continue;
          if (s[i] == 'a' + l || s[i] == '?') {
            dp[i+1][k][l] = (dp[i+1][k][l] + dp[i][j][k]) % mod;
          }
        }
      }
    }
  }

  ll ans = 0;
  for (int k = 0; k < 26; k++) {
    for (int l = 0; l < 26; l++) ans = (ans + dp[n][k][l]) % mod;
  }

  cout << ans << endl;

  return 0;
}
