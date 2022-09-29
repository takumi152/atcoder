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
  vector<unordered_set<int> > edge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].insert(b-1);
    edge[b-1].insert(a-1);
  }

  auto tree_maximum_matching = [&](vector<unordered_set<int> > e) {
    vector<bool> used_vertex(e.size());
    queue<int> que;
    for (int i = 0; i < n; i++) {
      if (e[i].size() == 1) que.push(i);
    }
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      int next = -1;
      for (auto &x: e[now]) {
        if (!used_vertex[x]) {
          next = x;
          break;
        }
      }
      if (next == -1) continue;
      used_vertex[now] = true;
      used_vertex[next] = true;
      auto e_now_to_erase = e[now];
      for (auto &x: e_now_to_erase) e[x].erase(now);
      e[now].clear();
      for (auto &x: e[next]) que.push(x);
      auto e_next_to_erase = e[next];
      for (auto &x: e_next_to_erase) e[x].erase(next);
      e[next].clear();
    }

    return used_vertex;
  };

  auto used_vertex = tree_maximum_matching(edge);
  vector<bool> eraseable_vertex(n);
  for (int i = 0; i < n; i++) {
    if (used_vertex[i]) {
      bool unused_adj = false;
      for (auto &x: edge[i]) {
        if (!used_vertex[x]) {
          unused_adj = true;
          break;
        }
      }
      if (unused_adj) {
        for (auto &x: edge[i]) {
          if (edge[x].size() == 1) {
            eraseable_vertex[x] = true;
          }
        }
      }
    }
  }

  int ans = 0;
  for (int i = 0; i < n; i++) {
    if (eraseable_vertex[i]) ans++;
  }

  cout << ans << endl;

  return 0;
}
