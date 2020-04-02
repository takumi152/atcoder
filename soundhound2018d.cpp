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

  int n, m, s, t;
  cin >> n >> m >> s >> t;
  s--;
  t--;
  vector<vector<vector<int> > > edge(n);
  for (int i = 0; i < m; i++) {
    int u, v, a, b;
    cin >> u >> v >> a >> b;
    edge[u-1].push_back(vector<int>({v-1, a, b}));
    edge[v-1].push_back(vector<int>({u-1, a, b}));
  }

  vector<ll> stox(n), ttox(n);
  priority_queue<pair<ll, int>, vector<pair<ll, int> >, greater<pair<ll, int> > > que;
  vector<bool> visited(n);
  que.emplace(0, s);
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    if (visited[now.second]) continue;
    visited[now.second] = true;
    stox[now.second] = now.first;
    for (auto &x: edge[now.second]) {
      if (!visited[x[0]]) que.emplace(now.first + x[1], x[0]);
    }
  }

  visited = vector<bool>(n);
  que.emplace(0, t);
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    if (visited[now.second]) continue;
    visited[now.second] = true;
    ttox[now.second] = now.first;
    for (auto &x: edge[now.second]) {
      if (!visited[x[0]]) que.emplace(now.first + x[2], x[0]);
    }
  }

  vector<ll> ans(n);
  priority_queue<pair<ll, int> > rem;
  for (int i = 0; i < n; i++) rem.emplace((ll)1e15 - (stox[i] + ttox[i]), i);
  for (int i = 0; i < n; i++) {
    while (true) {
      auto now = rem.top();
      if (now.second < i) rem.pop();
      else break;
    }
    auto next = rem.top();
    ans[i] = next.first;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
