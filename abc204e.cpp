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

ll isqrt(ll x) {
  ll ok = 0;
  ll ng = 1000000007;
  while (abs(ok - ng) > 1) {
    ll center = (ok + ng) / 2;
    if (center * center <= x) ok = center;
    else ng = center;
  }

  return ok;
}

ll find_fastest_departure(ll d) {
  return isqrt(d);
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m;
  cin >> n >> m;
  vector<vector<vector<ll> > > edge(n);
  for (ll i = 0; i < m; i++) {
    ll a, b, c, d;
    cin >> a >> b >> c >> d;
    edge[a-1].push_back(vector<ll>({b-1, c, d}));
    edge[b-1].push_back(vector<ll>({a-1, c, d}));
  }

  ll ans = -1;

  vector<bool> visited(n);
  priority_queue<pair<ll, ll>, vector<pair<ll, ll> >, greater<pair<ll, ll> > > que;
  que.emplace(0, 0);
  while (!que.empty()) {
    auto [t, now] = que.top();
    que.pop();
    if (visited[now]) continue;
    visited[now] = true;
    if (now == n-1) {
      ans = t;
      break;
    }
    for (auto &next: edge[now]) {
      if (!visited[next[0]]) {
        ll dep = max(t, find_fastest_departure(next[2]));
        que.emplace(dep + next[1] + next[2] / (dep + 1), next[0]);
      }
    }
  }

  cout << ans << endl;

  return 0;
}
