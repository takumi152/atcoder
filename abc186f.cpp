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

struct fenwick_tree {
  int n;
  vector<ll> data;

  fenwick_tree() {
    init(0);
  }

  fenwick_tree(int s) {
    init(s);
  }

  fenwick_tree(vector<ll> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) add(i, v[i]);
  }

  void init(int s) {
    n = s;
    data = vector<ll>(s);
  }

  void add(int p, ll v) {
    p++;
    while (p <= n) {
      data[p-1] += v;
      p += p & -p;
    }
  }

  ll sum(int l, int r) {
    return sum(r) - sum(l);
  }

  ll sum(int r) {
    ll s = 0;
    while (r > 0) {
      s += data[r-1];
      r -= r & -r;
    }
    return s;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w, m;
  cin >> h >> w >> m;
  vector<Pii> xy(m);
  for (auto &x: xy) cin >> x.first >> x.second;

  vector<vector<int> > row(h), col(w);
  for (int i = 0; i < m; i++) {
    row[xy[i].first-1].push_back(xy[i].second-1);
    col[xy[i].second-1].push_back(xy[i].first-1);
  }
  for (int i = 0; i < h; i++) sort(row[i].begin(), row[i].end());
  for (int j = 0; j < w; j++) sort(col[j].begin(), col[j].end());

  ll ans = 0;

  vector<int> col_reachable(w);
  for (int j = 0; j < w; j++) {
    if (!row[0].empty()) {
      if (j == row[0][0]) break;
    }
    if (!col[j].empty()) col_reachable[j] = col[j][0];
    else col_reachable[j] = h;
    ans += col_reachable[j];
  }
  vector<Pii> que;
  for (int j = 0; j < w; j++) que.emplace_back(col_reachable[j], j);
  sort(que.begin(), que.end());
  int p = 0;
  fenwick_tree ft(w);
  for (int i = 0; i < h; i++) {
    if (!col[0].empty()) {
      if (i == col[0][0]) break;
    }
    while (p < w) {
      if (que[p].first == i) {
        ft.add(que[p].second, 1);
          p++;
      }
      else break;
    }
    if (!row[i].empty()) ans += ft.sum(row[i][0]);
    else ans += ft.sum(w);
  }

  cout << ans << endl;

  return 0;
}
