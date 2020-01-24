#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;
const ll inf = (ll) 9e18;

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
  vector<vector<int> > xyc(n+m, vector<int>(3));
  for (int i = 0; i < n+m; i++) {
    cin >> xyc[i][0] >> xyc[i][1] >> xyc[i][2];
  }

  vector<vector<double> > edge(n+m, vector<double>(n+m));
  for (int i = 0; i < n+m; i++) {
    for (int j = i+1; j < n+m; j++) {
      edge[i][j] = sqrt((xyc[i][0] - xyc[j][0]) * (xyc[i][0] - xyc[j][0]) + (xyc[i][1] - xyc[j][1]) * (xyc[i][1] - xyc[j][1]));
      if (xyc[i][2] != xyc[j][2]) edge[i][j] *= 10.0;
      edge[j][i] = edge[i][j];
    }
  }

  double best = 1e308;
  for (int k = 0; k < (1 << m); k++) {
    vector<bool> need(n+m, true);
    int used = 0;
    int target = n+m-1;
    for (int i = 0; i < m; i++) {
      if (!((k >> i) & 1)) {
        need[n+i] = false;
        target--;
      }
    }

    unionfind uf(n+m);
    priority_queue<pair<double, pair<int, int> >, vector<pair<double, pair<int, int> > >, greater<pair<double, pair<int, int> > > > que;
    for (int i = 0; i < n+m; i++) {
      for (int j = i+1; j < n+m; j++) {
        if (!need[i] || !need[j]) continue;
        que.emplace(edge[i][j], make_pair(i, j));
      }
    }
    double cost = 0.0;
    while (!que.empty() || used < target) {
      auto now = que.top();
      que.pop();
      double c = now.first;
      int x = now.second.first;
      int y = now.second.second;
      if (uf.same(x, y)) continue;
      cost += c;
      used++;
      uf.unite(x, y);
    }
    if (cost < best) best = cost;
  }

  cout << setprecision(16) << best << endl;

  return 0;
}
