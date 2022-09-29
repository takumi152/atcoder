#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<ll> h(n);
  for (auto &x: h) cin >> x;
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }

  vector<ll> reverse_cost(n);
  vector<bool> visited(n);
  priority_queue<pair<ll, int>, vector<pair<ll, int> >, greater<pair<ll, int> > > que;
  que.emplace(0, 0);
  while (!que.empty()) {
    auto [cost, now] = que.top();
    que.pop();
    if (visited[now]) continue;
    visited[now] = true;
    reverse_cost[now] = cost;
    for (auto &next: edge[now]) {
      if (visited[next]) continue;
      ll extra_cost = max(0LL, h[next] - h[now]);
      que.emplace(cost + extra_cost, next);
    }
  }

  ll ans = 0;
  for (int i = 0; i < n; i++) {
    ll score = h[0] - h[i] - reverse_cost[i];
    if (score > ans) ans = score;
  }

  cout << ans << endl;

  return 0;
}
