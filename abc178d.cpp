#include <iostream>
#include <iomanip>
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

  int s;
  cin >> s;

  vector<vector<ll> > dp(s+2, vector<ll>(s+2));
  dp[0][0] = 1;
  for (int i = 0; i < s; i++) {
    ll total = 0;
    for (int j = 0; j + 3 <= s; j++) {
      total = (total + dp[i][j]) % mod;
      dp[i+1][j+3] = total;
    }
  }

  ll ans = 0;
  for (int i = 0; i < s; i++) ans = (ans + dp[i][s]) % mod;

  cout << ans << endl;

  return 0;
}
