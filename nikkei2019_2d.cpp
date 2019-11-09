#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<ll> l(m), r(m), c(m);
  for (int i = 0; i < m; i++) cin >> l[i] >> r[i] >> c[i];

  vector<vector<ll> > lrc(m, vector<ll>(3));
  for (int i = 0; i < m; i++) {
    lrc[i][0] = l[i] - 1;
    lrc[i][1] = -r[i];
    lrc[i][2] = c[i];
  }

  sort(lrc.begin(), lrc.end());

  for (int i = 0; i < m; i++) lrc[i][1] = -lrc[i][1];

  vector<ll> dp(n+2, inf);
  dp[0] = 0;
  for (int k = 0; k < m; k++) {
    ll b = lrc[k][0];
    ll e = lrc[k][1];
    ll cost = dp[b] + lrc[k][2];
    auto it = upper_bound(dp.begin() + b, dp.begin() + e, cost);
    ll d = distance(dp.begin(), it);
    for (int i = d; i < e; i++) {
      dp[i] = cost;
    }
  }

  auto ans = dp[n-1];
  if (ans >= inf) {
    cout << -1 << endl; // impossible
  }
  else {
    cout << ans << endl;
  }

  return 0;
}
