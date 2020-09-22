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

  int n, m;
  cin >> n >> m;
  vector<Pii> ab(m);
  for (auto &x: ab) cin >> x.first >> x.second;

  unionfind uf(n);
  for (int i = 0; i < m; i++) {
    uf.unite(ab[i].first-1, ab[i].second-1);
  }

  unordered_map<int, int> rootCount;
  for (int i = 0; i < n; i++) {
    rootCount[uf.root(i)]++;
  }

  int maximum = 0;
  for (auto &x: rootCount) {
    if (x.second > maximum) maximum = x.second;
  }

  cout << maximum << endl;

  return 0;
}
