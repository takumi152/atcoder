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

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

ll modinv(ll x, ll m = mod) {
  ll b = m;
  ll u = 1;
  ll v = 0;
  ll tmp;
  while (b) {
    ll t = x / b;
    x -= t * b;
    tmp = x;
    x = b;
    b = tmp;
    u -= t * v;
    tmp = u;
    u = v;
    v = tmp;
  }
  u %= m;
  if (u < 0) u += m;
  return u;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n;
  cin >> n;

  unordered_map<ll, ll> dp;
  unordered_set<ll> visited;
  dp[1] = 1;
  priority_queue<ll, vector<ll>, greater<ll>> que;
  que.push(1);
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    if (visited.find(now) != visited.end()) continue;
    visited.insert(now);
    for (ll d = 2; d <= 6; d++) {
      dp[now * d] = (dp[now * d] + dp[now]* modinv(5)) % mod;
      if (now * d < n) que.push(now * d);
    }
    dp.erase(now);
  }

  ll exact = 0;
  ll total = 0;
  for (auto &[val, cnt]: dp) {
    if (val == n) exact = (exact + cnt) % mod;
    total = (total + cnt) % mod;
  }

  cout << (exact * modinv(total)) % mod << endl;

  return 0;
}
