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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<Pii> > edge(n);
  for (int i = 0; i < m; i++) {
    int u, v, c;
    cin >> u >> v >> c;
    edge[u-1].emplace_back(v-1, c);
    edge[v-1].emplace_back(u-1, c);
  }

  vector<int> ans(n);
  vector<bool> visited(n);
  queue<Pii> que;
  que.emplace(0, 1);
  while (!que.empty()) {
    auto [vertex, label] = que.front();
    que.pop();
    if (visited[vertex]) continue;
    visited[vertex] = true;
    ans[vertex] = label;
    for (auto &[next_vertex, next_label]: edge[vertex]) {
      if (!visited[next_vertex]) {
        if (next_label == label) {
          if (next_label == 1) que.emplace(next_vertex, 2);
          else que.emplace(next_vertex, 1);
        }
        else {
          que.emplace(next_vertex, next_label);
        }
      }
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
