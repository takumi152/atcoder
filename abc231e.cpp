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

  ll n, x;
  cin >> n >> x;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  vector<ll> max_factor(n);
  for (int i = 0; i < n-1; i++) max_factor[i] = a[i+1] / a[i];
  max_factor[n-1] = (ll) 2e18;

  vector<ll> req_factor(n);
  {
    ll x_tmp = x;
    for (int i = n-1; i >= 0; i--) {
      req_factor[i] = x_tmp / a[i];
      x_tmp %= a[i];
    }
  }

  vector<ll> suf_pay(n, (ll) 4e18);
  vector<ll> suf_nopay(n, (ll) 4e18);
  vector<ll> insuf_pay(n, (ll) 4e18);
  vector<ll> insuf_nopay(n, (ll) 4e18);
  vector<bool> insuffable(n, true);
  vector<bool> suffable(n, true);
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      suf_pay[i] = req_factor[i];
      suf_nopay[i] = max_factor[i] - req_factor[i];
      if (req_factor[i] == 0) insuffable[i] = false;
    }
    else if (i != n-1) {
      suf_pay[i] = req_factor[i];
      suf_nopay[i] = max_factor[i] - req_factor[i];
      if (req_factor[i] == 0) insuffable[i] = false;
      insuf_pay[i] = req_factor[i] + 1;
      insuf_nopay[i] = max_factor[i] - req_factor[i] - 1;
      if (req_factor[i] == max_factor[i] + 1) suffable[i] = false;
    }
    else {
      suf_pay[i] = req_factor[i];
      insuf_pay[i] = req_factor[i] + 1;
    }
  }

  vector<vector<ll> > dp(n+1, vector<ll>(2, (ll) 4e18));
  dp[0][0] = 0;
  for (int i = 0; i < n; i++) {
    dp[i+1][0] = min(dp[i+1][0], dp[i][0] + suf_pay[i]);
    if (insuffable[i]) dp[i+1][1] = min(dp[i+1][1], dp[i][0] + suf_nopay[i]);
    if (suffable[i]) dp[i+1][0] = min(dp[i+1][0], dp[i][1] + insuf_pay[i]);
    dp[i+1][1] = min(dp[i+1][1], dp[i][1] + insuf_nopay[i]);
  }

  cout << dp[n][0] << endl;

  return 0;
}
