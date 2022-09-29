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

  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  vector<vector<ll> > dp1(n+1, vector<ll>(2));
  vector<vector<ll> > dp2(n+1, vector<ll>(2));
  dp1[0][0] = 1;
  dp1[1][0] = 1;
  dp2[1][0] = a[0];
  for (int i = 1; i < n; i++) {
    dp1[i+1][0] = (dp1[i][0] + dp1[i][1]) % mod;
    dp1[i+1][1] = dp1[i][0];

    dp2[i+1][0] = ((dp2[i][0] + dp2[i][1]) + dp1[i+1][0] * a[i]) % mod;
    dp2[i+1][1] = (dp2[i][0] - dp1[i+1][1] * a[i]) % mod;
    if (dp2[i+1][1] < 0) dp2[i+1][1] += mod;
  }

  ll ans = (dp2[n][0] + dp2[n][1]) % mod;
  if (ans < 0) ans += mod;

  cout << ans << endl;

  return 0;
}
