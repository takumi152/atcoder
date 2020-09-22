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

  int n, q;
  cin >> n >> q;
  vector<int> t(q), u(q), v(q);
  for (int i = 0; i < q; i++) cin >> t[i] >> u[i] >> v[i];

  unionfind uf(n);

  vector<int> ans;
  for (int i = 0; i < q; i++) {
    if (t[i] == 0) {
      uf.unite(u[i], v[i]);
    }
    else if (t[i] == 1) {
      if (uf.same(u[i], v[i])) ans.push_back(1);
      else ans.push_back(0);
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
