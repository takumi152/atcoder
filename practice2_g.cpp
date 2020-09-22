#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <cassert>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

vector<vector<int> > strongly_connected_component(vector<vector<int> > &edge) {
  int n = edge.size();
  vector<vector<int> > edge_rev(n);
  for (int i = 0; i < n; i++) {
    for (auto &x: edge[i]) edge_rev[x].push_back(i);
  }

  vector<int> dfs_order;
  dfs_order.reserve(n);

  vector<bool> visited1(n);
  auto dfs1 = [&](auto self, int v) {
    if (visited1[v]) return;
    visited1[v] = true;
    for (auto &x: edge[v]) {
      if (!visited1[x]) self(self, x);
    }
    dfs_order.push_back(v);
    return;
  };
  for (int i = 0; i < n; i++) dfs1(dfs1, i);

  reverse(dfs_order.begin(), dfs_order.end());

  int group_count = 0;
  vector<int> group(n, -1);
  auto dfs2 = [&](auto self, int v) {
    if (group[v] >= 0) return;
    group[v] = group_count;
    for (auto &x: edge_rev[v]) {
      if (group[x] == -1) self(self, x);
    }
    return;
  };
  for (auto &x: dfs_order) {
    if (group[x] == -1) {
      dfs2(dfs2, x);
      group_count++;
    }
  }

  vector<vector<int> > intergroup_edge(group_count);
  for (int i = 0; i < n; i++) {
    for (auto &x: edge[i]) {
      if (group[i] != group[x]) intergroup_edge[group[i]].push_back(group[x]);
    }
  }

  vector<int> topological_order;
  topological_order.reserve(group_count);

  vector<bool> visited2(group_count);
  auto dfs3 = [&](auto self, int v) {
    if (visited2[v]) return;
    visited2[v] = true;
    for (auto &x: intergroup_edge[v]) {
      if (!visited2[x]) self(self, x);
    }
    topological_order.push_back(v);
    return;
  };
  for (int i = 0; i < group_count; i++) dfs3(dfs3, i);

  reverse(topological_order.begin(), topological_order.end());

  vector<vector<int> > scc(group_count);
  for (int i = 0; i < n; i++) scc[group[i]].push_back(i);
  vector<vector<int> > topological_scc;
  for (auto &x: topological_order) topological_scc.push_back(scc[x]);

  return topological_scc;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a].push_back(b);
  }

  vector<vector<int> > scc = strongly_connected_component(edge);

  cout << scc.size() << endl;
  for (auto &x: scc) {
    cout << x.size() << " ";
    for (auto &y: x) cout << y << " ";
    cout << endl;
  }

  return 0;
}
