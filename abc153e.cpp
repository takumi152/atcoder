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
const ll inf = (ll) 9e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, n;
  cin >> h >> n;
  vector<Pii> ab(n);
  for (auto &x: ab) cin >> x.first >> x.second;

  vector<ll> dp(h+1, inf);
  dp[h] = 0;
  for (int i = h; i > 0; i--) {
    for (int j = 0; j < n; j++) {
      dp[max(0, i-ab[j].first)] = min(dp[max(0, i-ab[j].first)], dp[i] + ab[j].second);
    }
  }

  cout << dp[0] << endl;

  return 0;
}
