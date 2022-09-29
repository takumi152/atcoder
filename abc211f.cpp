#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <cassert>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

struct lazy_segtree {
  int n;
  vector<int> data;
  vector<int> lazy;
  vector<bool> lazyFlag;

  lazy_segtree(const int s) {
    init(s);
  }

  lazy_segtree(const vector<int> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = min(data[i*2+1], data[i*2+2]);
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<int>(2*n-1);
    lazy = vector<int>(2*n-1);
    lazyFlag = vector<bool>(2*n-1);
  }

  void evaluate(int p, int a, int b) {
    if (lazyFlag[p]) {
      data[p] += lazy[p];
      if (b - a > 1) {
        lazy[p*2+1] += lazy[p];
        lazy[p*2+2] += lazy[p];
        lazyFlag[p*2+1] = true;
        lazyFlag[p*2+2] = true;
      }
      lazy[p] = 0;
      lazyFlag[p] = false;
    }
  }

  void update(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init

    // evaluate lazy value
    evaluate(p, a, b);

    if (r <= a || b <= l) return; // out of range
    if (l <= a && b <= r) { // fully covered
      lazy[p] += v;
      lazyFlag[p] = true;
      evaluate(p, a, b);
    }
    else {
      update(l, r, v, p*2+1, a, (a + b) / 2); // left
      update(l, r, v, p*2+2, (a + b) / 2, b); // right
      data[p] = min(data[p*2+1], data[p*2+2]); // update
    }
    return;
  }

  int query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0x7fffffff; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return min(vl, vr);
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> m(n);
  vector<vector<Pii> > xy(n);
  for (int i = 0; i < n; i++) {
    cin >> m[i];
    xy[i] = vector<Pii>(m[i]);
    for (auto &x: xy[i]) cin >> x.first >> x.second;
  }
  int q;
  cin >> q;
  vector<Pii> qxy(q);
  for (auto &x: qxy) cin >> x.first >> x.second;

  vector<vector<int> > event;

  // add / remove section
  lazy_segtree lst(100000);
  for (int i = 0; i < n; i++) {
    vector<vector<int> > edge;
    for (int j = 0; j < m[i]; j++) {
      if (xy[i][j].first == xy[i][(j+1)%m[i]].first) edge.push_back(vector<int>({xy[i][j].first, min(xy[i][j].second, xy[i][(j+1)%m[i]].second), max(xy[i][j].second, xy[i][(j+1)%m[i]].second)}));
    }
    sort(edge.begin(), edge.end());

    for (auto &e: edge) {
      if (lst.query(e[1], e[2]) == 0) {
        lst.update(e[1], e[2], 1);
        event.push_back(vector<int>({e[0], 1, e[1], e[2]}));
      }
      else {
        lst.update(e[1], e[2], -1);
        event.push_back(vector<int>({e[0], 0, e[1], e[2]}));
      }
    }

    assert(lst.query(0, 100000) == 0);
  }

  // query
  for (int i = 0; i < q; i++) {
    event.push_back(vector<int>({qxy[i].first, 2, qxy[i].second, qxy[i].second+1, i}));
  }

  sort(event.begin(), event.end());

  vector<int> ans(q);
  for (auto &e: event) {
    if (e[1] == 0) {
      lst.update(e[2], e[3], -1);
    }
    else if (e[1] == 1) {
      lst.update(e[2], e[3], 1);
    }
    else if (e[1] == 2) {
      ans[e[4]] = lst.query(e[2], e[3]);
    }
  }

  // for (auto &x: event) {
  //   for (auto &y: x) cerr << y << " ";
  //   cerr << endl;
  // }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
