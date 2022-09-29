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

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<vector<ll> > dp(n, vector<ll>(10));
  dp[0][a[0]] = 1;
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < 10; j++) {
      dp[i][(j + a[i]) % 10] = (dp[i][(j + a[i]) % 10] + dp[i-1][j]) % mod;
      dp[i][(j * a[i]) % 10] = (dp[i][(j * a[i]) % 10] + dp[i-1][j]) % mod;
    }
  }

  for (int i = 0; i < 10; i++) cout << dp[n-1][i] << endl;

  return 0;
}
