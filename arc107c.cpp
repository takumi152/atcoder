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

const ll mod = 998244353;

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

  int n, k;
  cin >> n >> k;
  vector<vector<int> > a(n, vector<int>(n));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }

  unionfind uf_vertical(n);
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      bool good = true;
      for (int l = 0; l < n; l++) {
        if (a[i][l] + a[j][l] > k) {
          good = false;
          break;
        }
      }
      if (good) uf_vertical.unite(i, j);
    }
  }

  unionfind uf_horizontal(n);
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      bool good = true;
      for (int l = 0; l < n; l++) {
        if (a[l][i] + a[l][j] > k) {
          good = false;
          break;
        }
      }
      if (good) uf_horizontal.unite(i, j);
    }
  }

  unordered_map<int, int> count_vertical;
  for (int i = 0; i < n; i++) count_vertical[uf_vertical.root(i)]++;
  unordered_map<int, int> count_horizontal;
  for (int i = 0; i < n; i++) count_horizontal[uf_horizontal.root(i)]++;

  vector<ll> factorial(101);
  factorial[0] = 1;
  for (ll i = 1; i < 101; i++) factorial[i] = (factorial[i-1] * i) % mod;

  ll ans = 1;
  for (auto &x: count_vertical) ans = (ans * factorial[x.second]) % mod;
  for (auto &x: count_horizontal) ans = (ans * factorial[x.second]) % mod;

  cout << ans << endl;

  return 0;
}
