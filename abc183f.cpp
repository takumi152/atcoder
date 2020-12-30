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

  int n, q;
  cin >> n >> q;
  vector<int> c(n);
  for (auto &x: c) cin >> x;
  vector<vector<int> > query(q, vector<int>(3));
  for (auto &x: query) {
    for (auto &y: x) cin >> y;
  }

  unionfind uf(n);
  unordered_map<int, unordered_map<int, int> > group;
  for (int i = 0; i < n; i++) {
    group[i][c[i]] = 1;
  }
  vector<int> ans;
  for (int i = 0; i < q; i++) {
    // cerr << i << endl;
    // for (auto &x: group) {
    //   cerr << x.first << ": ";
    //   for (auto &y: x.second) {
    //     cerr << "(" << y.first << ", " << y.second << "), ";
    //   }
    //   cerr << endl;
    // }
    if (query[i][0] == 1) {
      if (uf.same(query[i][1]-1, query[i][2]-1)) continue;
      int a_group = uf.root(query[i][1]-1);
      int b_group = uf.root(query[i][2]-1);
      uf.unite(query[i][1]-1, query[i][2]-1);
      int group_after = uf.root(query[i][1]-1);
      if (a_group == group_after) {
        for (auto &x: group[b_group]) {
          group[a_group][x.first] += x.second;
        }
        group[b_group].clear();
      }
      else {
        for (auto &x: group[a_group]) {
          group[b_group][x.first] += x.second;
        }
        group[a_group].clear();
      }
    }
    else {
      ans.push_back(group[uf.root(query[i][1]-1)][query[i][2]]);
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
