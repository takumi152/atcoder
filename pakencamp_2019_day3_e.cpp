#include <iostream>
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

  int h, w;
  cin >> h >> w;
  vector<string> s(h);
  for (auto &x: s) cin >> x;
  int q;
  cin >> q;
  vector<vector<int> > xyl(q, vector<int>(3));
  for (auto &x: xyl) {
    cin >> x[0] >> x[1] >> x[2];
    x[0]--;
    x[1]--;
  }

  unionfind uf(h * w);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (s[i][j] == '#') continue;
      if (i > 0) {
        if (s[i-1][j] == '.') uf.unite(i*w + j, (i-1)*w + j);
      }
      if (j > 0) {
        if (s[i][j-1] == '.') uf.unite(i*w + j, i*w + (j-1));
      }
      if (i < h-1) {
        if (s[i+1][j] == '.') uf.unite(i*w + j, (i+1)*w + j);
      }
      if (j < w-1) {
        if (s[i][j+1] == '.') uf.unite(i*w + j, i*w + (j+1));
      }
    }
  }

  unordered_map<int, int> count;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (s[i][j] == '#') continue;
      else count[uf.root(i*w + j)]++;
    }
  }

  vector<int> ans(q);
  for (int i = 0; i < q; i++) {
    if (xyl[i][2] == 1) {
      ans[i] = count[uf.root(xyl[i][0]*w + xyl[i][1])];
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
