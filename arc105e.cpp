// WIP

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

  int t;
  cin >> t;
  vector<int> n(t), m(t);
  vector<vector<Pii> > ab(t);
  for (int i = 0; i < t; i++) {
    cin >> n[i] >> m[i];
    ab[i] = vector<Pii>(m[i]);
    for (auto &x: ab[i]) {
      cin >> x.first >> x.second;
      x.first--;
      x.second--;
    }
  }

  vector<string> ans(t);
  for (int i = 0; i < t; i++) {
    unionfind uf(n[i]);
    vector<vector<int> > edge(n[i]);
    for (int j = 0; j < m[i]; j++) {
      uf.unite(ab[i][j].first, ab[i][j].second);
      edge[ab[i][j].first].push_back(ab[i][j].second);
      edge[ab[i][j].second].push_back(ab[i][j].first);
    }
    unordered_map<int, ll> rootCount;
    for (int j = 0; j < n[i]; j++) rootCount[uf.root(j)]++;
    ll redVertex = rootCount[uf.root(0)];
    ll blueVertex = rootCount[uf.root(n[i]-1)];
    ll redRemaining = redVertex * (redVertex + 1) / 2;
    ll blueRemaining = blueVertex * (blueVertex + 1) / 2;
    vector<pair<ll, ll> > neutralGroup(n[i]);
    for (auto &x: rootCount) neutralGroup[x.first].first = x.second;
    for (int j = 0; j < m[i]; j++) {
      if (uf.root(ab[i][j].first) == 0) redRemaining--;
      else if (uf.root(ab[i][j].first) == n-1) blueRemaining--;
      else neutralGroup[uf.root(ab[i][j].first)].second++;
    }
    ll edgeCount = redRemaining + blueRemaining
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
