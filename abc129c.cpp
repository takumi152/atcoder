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

  int n, m;
  cin >> n >> m;
  vector<int> a(m);
  for (auto &x: a) cin >> x;

  vector<bool> dangerous(n+1);
  for (auto &x: a) dangerous[x] = true;

  vector<ll> dp(n+1);
  dp[0] = 1;
  if (!dangerous[1]) dp[1] = 1;
  for (int i = 2; i <= n; i++) {
    if (!dangerous[i]) dp[i] = (dp[i-2] + dp[i-1]) % mod;
  }

  cout << dp[n] << endl;

  return 0;
}
