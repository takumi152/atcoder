#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_set>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m, k;
  cin >> n >> m >> k;
  vector<vector<int>> edge(n);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    edge[u-1].push_back(v-1);
    edge[v-1].push_back(u-1);
  }
  vector<int> a(n), b(k);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  vector<unordered_set<int>> id_set(n+1);
  for (int i = 0; i < n; i++) id_set[a[i]].insert(i);

  vector<bool> visited(n);
  unordered_set<int> next_vertex_set_to_begin;
  next_vertex_set_to_begin.insert(0);
  for (int i = 0; i < k; i++) {
    queue<int> que;
    for (auto &x: next_vertex_set_to_begin) que.push(x);
    next_vertex_set_to_begin.clear();
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (visited[now]) continue;
      if (i == 0 && now == 0 && id_set[b[i]].find(now) != id_set[b[i]].end()) {
        next_vertex_set_to_begin.insert(now);
        continue;
      }
      visited[now] = true;
      for (auto &next: edge[now]) {
        if (!visited[next]) {
          if (id_set[b[i]].find(next) != id_set[b[i]].end()) {
            next_vertex_set_to_begin.insert(next);
          }
          else {
            que.push(next);
          }
        }
      }
    }
    if (visited[n-1]) {
      cout << "No" << endl;
      return 0;
    }
  }

  cout << "Yes" << endl;

  return 0;
}
