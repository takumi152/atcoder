#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

struct unionfind {
  vector<int> group;
  vector<int> rank;

  unionfind() {
    unionfind(0);
  }

  unionfind(int n) {
    group = vector<int>(n);
    rank = vector<int>(n);
    for (int i = 0; i < n; i++) group[i] = i;
  }

  int root(int x) {
    if (group[x] == x) return x;
    return group[x] = root(group[x]);
  }

  void unite(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    if (rank[rx] > rank[ry]) group[ry] = rx;
    else if (rank[ry] > rank[rx]) group[rx] = ry;
    else if (rx != ry) {
      group[ry] = rx;
      rank[rx]++;
    }
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

  int n;
  cin >> n;
  vector<Pii> xy(n);
  for (auto &x: xy) cin >> x.first >> x.second;

  vector<int> y(n);
  for (auto &x: xy) y[x.first-1] = x.second;
  unionfind uf(n);

  priority_queue<Pii, vector<Pii>, greater<Pii> > pque;
  pque.emplace(y[0], 0);
  for (int i = 1; i < n; i++) {
    auto top = pque.top();
    if (top.first < y[i]) {
      while (!pque.empty()) {
        auto next = pque.top();
        if (next.first > y[i]) break;
        pque.pop();
        uf.unite(i, next.second);
      }
      pque.push(top);
    }
    else {
      pque.emplace(y[i], i);
    }
  }

  unordered_map<int, int> root_count;
  for (int i = 0; i < n; i++) root_count[uf.root(i)]++;

  vector<int> ans(n);
  for (int i = 0; i < n; i++) ans[i] = root_count[uf.root(xy[i].first-1)];

  for (auto &x: ans) cout << x << endl;

  return 0;
}
