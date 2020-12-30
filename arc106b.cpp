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

  int n, m;
  cin >> n >> m;
  vector<ll> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int c, d;
    cin >> c >> d;
    edge[c-1].push_back(d-1);
    edge[d-1].push_back(c-1);
  }

  unionfind uf(n);
  for (int i = 0; i < n; i++) {
    for (auto &x: edge[i]) uf.unite(i, x);
  }

  unordered_map<int, ll> a_total, b_total;
  for (int i = 0; i < n; i++) {
    a_total[uf.root(i)] += a[i];
    b_total[uf.root(i)] += b[i];
  }

  for (auto &x: a_total) {
    if (x.second != b_total[x.first]) {
      cout << "No" << endl;
      return 0;
    }
  }

  cout << "Yes" << endl;

  return 0;
}
