#include <iostream>
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

const ll mod = 1000000007;

struct unionfind {
  vector<int> group;

  unionfind(int n) {
    group = vector<int>(n);
    for (int i = 0; i < n; i++) group[i] = i;
  }

  int root(int x) {
    if (group[x] == x) return x;
    return group[x] = root(group[x]);
  }

  void unite(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    if (rx == ry) return;
    group[rx] = ry;
  }

  bool same(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    return rx == ry;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m, k;
  cin >> n >> m >> k;
  vector<vector<int> > follow(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    follow[a-1].push_back(b-1);
    follow[b-1].push_back(a-1);
  }
  vector<vector<int> > block(n);
  for (int i = 0; i < k; i++) {
    int a, b;
    cin >> a >> b;
    block[a-1].push_back(b-1);
    block[b-1].push_back(a-1);
  }

  unionfind uf(n);
  vector<bool> visited(n);
  for (int i = 0; i < n; i++) {
    if (!visited[i]) {
      queue<int> que;
      que.push(i);
      while (!que.empty()) {
        auto now = que.front();
        que.pop();
        if (visited[now]) continue;
        visited[now] = true;
        uf.unite(i, now);
        for (auto &x: follow[now]) {
          if (!visited[x]) que.push(x);
        }
      }
    }
  }

  unordered_map<int, int> groupCount;
  for (int i = 0; i < n; i++) groupCount[uf.root(i)]++;

  vector<int> ans(n);
  for (int i = 0; i < n; i++) {
    ans[i] = groupCount[uf.root(i)] - 1;
    unordered_set<int> excluded;
    for (auto &x: follow[i]) excluded.insert(x);
    for (auto &x: block[i]) {
      if (uf.root(x) == uf.root(i)) excluded.insert(x);
    }
    ans[i] -= excluded.size();
  }

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
