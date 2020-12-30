#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <map>

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

  int n, k, l;
  cin >> n >> k >> l;
  vector<vector<int> > edge_road(n);
  for (int i = 0; i < k; i++) {
    int p, q;
    cin >> p >> q;
    edge_road[p-1].push_back(q-1);
    edge_road[q-1].push_back(p-1);
  }
  vector<vector<int> > edge_train(n);
  for (int i = 0; i < l; i++) {
    int r, s;
    cin >> r >> s;
    edge_train[r-1].push_back(s-1);
    edge_train[s-1].push_back(r-1);
  }

  unionfind uf_road(n), uf_train(n);
  for (int i = 0; i < n; i++) {
    for (auto &x: edge_road[i]) uf_road.unite(i, x);
    for (auto &x: edge_train[i]) uf_train.unite(i, x);
  }

  map<Pii, int> rootCount;
  for (int i = 0; i < n; i++) rootCount[Pii(uf_road.root(i), uf_train.root(i))]++;

  vector<int> ans(n);
  for (int i = 0; i < n; i++) ans[i] = rootCount[Pii(uf_road.root(i), uf_train.root(i))];

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
