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

  int n, m, k;
  cin >> n >> m >> k;
  vector<int> a(m), b(m), c(m);
  for (int i = 0; i < m; i++) {
    cin >> a[i] >> b[i] >> c[i];
    a[i]--;
    b[i]--;
  }
  vector<int> e(k);
  for (auto &x: e) {
    cin >> x;
    x--;
  }

  vector<ll> dp(n, (ll) 1e18);
  dp[0] = 0;
  for (int i = 0; i < k; i++) {
    if (dp[a[e[i]]] + c[e[i]] < dp[b[e[i]]]) dp[b[e[i]]] = dp[a[e[i]]] + c[e[i]];
  }
  if (dp[n-1] < (ll) 1e18) {
    cout << dp[n-1] << endl;
  }
  else {
    cout << -1 << endl;
  }

  return 0;
}
