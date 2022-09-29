#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
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

  vector<int> a(n);
  for (int i = 0; i < n; i++) a[i] = s[i] - 'A';

  vector<vector<vector<ll> > > dp(n+1, vector<vector<ll> >(1 << 10, vector<ll>(10))); // count, history, now
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < (1 << 10); j++) {
      for (int k = 0; k < 10; k++) {
        dp[i+1][j][k] = dp[i][j][k];
      }
    }
    dp[i+1][1 << a[i]][a[i]] = (dp[i+1][1 << a[i]][a[i]] + 1) % mod;
    for (int j = 0; j < (1 << 10); j++) {
      for (int k = 0; k < 10; k++) {
        if (k == a[i]) dp[i+1][j][k] = (dp[i+1][j][k] + dp[i][j][k]) % mod;
        else if ((j | (1 << a[i])) != j) dp[i+1][j|(1 << a[i])][a[i]] = (dp[i+1][j|(1 << a[i])][a[i]] + dp[i][j][k]) % mod;
      }
    }
  }

  ll ans = 0;
  for (int j = 1; j < (1 << 10); j++) {
    for (int k = 0; k < 10; k++) ans = (ans + dp[n][j][k]) % mod;
  }

  // for (int j = 1; j < (1 << 10); j++) {
  //   for (int k = 0; k < 10; k++) {
  //     if (dp[n][j][k] != 0) cerr << bitset<10>(j) << " " << k << " " << dp[n][j][k] << endl;
  //   }
  // }

  cout << ans << endl;

  return 0;
}
