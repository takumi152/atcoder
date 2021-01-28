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

  int n, m;
  cin >> n >> m;
  vector<Pii> ab(m);
  for (auto &x: ab) {
    cin >> x.first >> x.second;
    x.first--;
    x.second--;
  }
  vector<int> c(n);
  for (auto &x: c) cin >> x;

  vector<int> ans(m);
  for (int i = 0; i < m; i++) {
    if (c[ab[i].first] > c[ab[i].second]) ans[i] = 1;
    else if (c[ab[i].first] < c[ab[i].second]) ans[i] = -1;
  }

  vector<vector<Pii> > edge(n);
  for (int i = 0; i < m; i++) {
    if (ans[i] == 0) {
      edge[ab[i].first].emplace_back(ab[i].second, i);
      edge[ab[i].second].emplace_back(ab[i].first, i);
    }
  }

  vector<bool> visited(n);
  auto dfs = [&](auto self, int now) {
    if (visited[now]) return;
    visited[now] = true;
    for (auto &[next, id]: edge[now]) {
      if (ans[id] != 0) continue;
      if (ab[id].first == now) ans[id] = 1;
      else ans[id] = -1;
      if (!visited[next]) self(self, next);
    }
  };

  for (int i = 0; i < n; i++) dfs(dfs, i);

  for (auto &x: ans) {
    if (x == 1) cout << "->" << endl;
    else if (x == -1) cout << "<-" << endl;
  }

  return 0;
}
