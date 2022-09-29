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
  vector<Pii> ab(n);
  for (int i = 0; i < n; i++) cin >> ab[i].first;
  for (int i = 0; i < n; i++) cin >> ab[i].second;

  sort(ab.begin(), ab.end());

  vector<ll> dp(5001);
  dp[0] = 1;
  ll ans = 0;
  int p = 0;
  for (int k = 1; k <= 5000; k++) {
    while (p < n) {
      if (ab[p].first == k) {
        vector<ll> dp_next(5001);
        for (int i = 0; i <= 5000; i++) {
          if (i + ab[p].second <= 5000) dp_next[i + ab[p].second] = (dp[i] + dp_next[i + ab[p].second]) % mod;
          else break;
        }
        for (int i = 1; i <= k; i++) ans = (ans + dp_next[i]) % mod;
        for (int i = 0; i <= 5000; i++) dp[i] = (dp[i] + dp_next[i]) % mod;
        p++;
      }
      else break;
    }
  }

  cout << ans << endl;

  return 0;
}
