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

struct euclid_lazy_segtree {
  int n;
  vector<vector<double> > data;
  vector<vector<vector<double> > > lazy;
  vector<bool> lazyFlag;

  euclid_lazy_segtree() {
    init(1);
  }

  euclid_lazy_segtree(const int s) {
    init(s);
  }

  euclid_lazy_segtree(const vector<pair<double, double> > &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1][0] = v[i].first;
      data[i+n-1][1] = v[i].second;
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<vector<double> >(2*n-1, vector<double>({0.0, 0.0, 1.0}));
    lazy = vector<vector<vector<double> > >(2*n-1, vector<vector<double> >(3, vector<double>(3)));
    for (int i = 0; i < 2*n-1; i++) {
      for (int j = 0; j < 3; j++) lazy[i][j][j] = 1.0;
    }
    lazyFlag = vector<bool>(2*n-1);
  }

  void apply_op(vector<vector<double> > &op, vector<vector<double> > &to) {
    evaluate_lazy(op, to);
  }

  void evaluate_lazy(vector<vector<double> > &op, vector<vector<double> > &to) {
    static vector<vector<double> > next(3, vector<double>(3));
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        next[i][j] = 0.0;
        for (int k = 0; k < 3; k++) {
          next[i][j] += op[i][k] * to[k][j];
        }
      }
    }
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        to[i][j] = next[i][j];
      }
    }
  }

  void evaluate_data(vector<vector<double> > &op, vector<double> &to) {
    static vector<double> next(3);
    for (int i = 0; i < 3; i++) {
      next[i] = 0.0;
      for (int k = 0; k < 3; k++) {
        next[i] += op[i][k] * to[k];
      }
    }
    for (int i = 0; i < 3; i++) {
      to[i] = next[i];
    }
  }

  void propagate(int p, int a, int b) {
    if (lazyFlag[p]) {
      if (b - a > 1) {
        evaluate_lazy(lazy[p], lazy[p*2+1]);
        evaluate_lazy(lazy[p], lazy[p*2+2]);
        lazyFlag[p*2+1] = true;
        lazyFlag[p*2+2] = true;
      }
      else {
        evaluate_data(lazy[p], data[p]);
      }
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          lazy[p][i][j] = (i == j ? 1.0 : 0.0);
        }
      }
      lazyFlag[p] = false;
    }
  }

  void update(int l, int r, vector<vector<double> > op, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init

    // propagate lazy value
    propagate(p, a, b);

    if (r <= a || b <= l) return; // out of range
    if (l <= a && b <= r) { // fully covered
      evaluate_lazy(op, lazy[p]);
      lazyFlag[p] = true;
      propagate(p, a, b);
    }
    else {
      update(l, r, op, p*2+1, a, (a + b) / 2); // left
      update(l, r, op, p*2+2, (a + b) / 2, b); // right
    }
    return;
  }

  void translate(int l, int r, double x, double y) {
    vector<vector<double> > op({{1.0, 0.0,   x},
                                {0.0, 1.0,   y},
                                {0.0, 0.0, 1.0}});

    update(l, r, op);
  }

  void flip_x_axis(int l, int r) {
    static vector<vector<double> > op({{-1.0, 0.0, 0.0},
                                       { 0.0, 1.0, 0.0},
                                       { 0.0, 0.0, 1.0}});

    update(l, r, op);
  }

  void flip_y_axis(int l, int r) {
    static vector<vector<double> > op({{1.0,  0.0, 0.0},
                                       {0.0, -1.0, 0.0},
                                       {0.0,  0.0, 1.0}});

    update(l, r, op);
  }

  void rotate_with_pivot(int l, int r, double x, double y, double theta) {
    vector<vector<double> > op1({{1.0, 0.0,  -x},
                                 {0.0, 1.0,  -y},
                                 {0.0, 0.0, 1.0}});
    vector<vector<double> > op2({{cos(theta), -sin(theta), 0.0},
                                 {sin(theta),  cos(theta), 0.0},
                                 {       0.0,         0.0, 1.0}});
    vector<vector<double> > op3({{1.0, 0.0,   x},
                                 {0.0, 1.0,   y},
                                 {0.0, 0.0, 1.0}});
    vector<vector<double> > op({{1.0, 0.0, 0.0},
                                {0.0, 1.0, 0.0},
                                {0.0, 0.0, 1.0}});

    apply_op(op1, op);
    apply_op(op2, op);
    apply_op(op3, op);
    update(l, r, op);
  }

  void rotate_90_deg_cw(int l, int r) {
    static vector<vector<double> > op1({{1.0, 0.0, 0.0},
                                        {0.0, 1.0, 0.0},
                                        {0.0, 0.0, 1.0}});
    static vector<vector<double> > op2({{ 0.0, 1.0, 0.0},
                                        {-1.0, 0.0, 0.0},
                                        { 0.0, 0.0, 1.0}});
    static vector<vector<double> > op3({{1.0, 0.0, 0.0},
                                        {0.0, 1.0, 0.0},
                                        {0.0, 0.0, 1.0}});
    vector<vector<double> > op({{1.0, 0.0, 0.0},
                                {0.0, 1.0, 0.0},
                                {0.0, 0.0, 1.0}});

    apply_op(op1, op);
    apply_op(op2, op);
    apply_op(op3, op);
    update(l, r, op);
  }

  void rotate_90_deg_ccw(int l, int r) {
    static vector<vector<double> > op1({{1.0, 0.0, 0.0},
                                        {0.0, 1.0, 0.0},
                                        {0.0, 0.0, 1.0}});
    static vector<vector<double> > op2({{0.0, -1.0, 0.0},
                                        {1.0,  0.0, 0.0},
                                        {0.0,  0.0, 1.0}});
    static vector<vector<double> > op3({{1.0, 0.0, 0.0},
                                        {0.0, 1.0, 0.0},
                                        {0.0, 0.0, 1.0}});
    vector<vector<double> > op({{1.0, 0.0, 0.0},
                                {0.0, 1.0, 0.0},
                                {0.0, 0.0, 1.0}});

    apply_op(op1, op);
    apply_op(op2, op);
    apply_op(op3, op);
    update(l, r, op);
  }

  pair<double, double> query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return pair<double, double>(0.0, 0.0); // out of range

    // propagate lazy value
    propagate(p, a, b);

    if (l <= a && b <= r) return pair<double, double>(data[p][0] / data[p][2], data[p][1] / data[p][2]); // fully covered

    pair<double, double> vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    pair<double, double> vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return pair<double, double>(vl.first + vr.first, vl.second + vr.second);
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<pair<double, double> > xy(n);
  for (auto &x: xy) cin >> x.first >> x.second;
  int m;
  cin >> m;
  vector<Pii> op(m);
  for (auto &x: op) {
    cin >> x.first;
    if (x.first >= 3) cin >> x.second;
  }
  int q;
  cin >> q;
  vector<Pii> ab(q);
  for (auto &x: ab) cin >> x.first >> x.second;

  vector<int> query_order(q);
  for (int i = 0; i < q; i++) query_order[i] = i;
  sort(query_order.begin(), query_order.end(), [&](auto &a, auto &b){return ab[a] < ab[b];});

  int s = 1;
  while (s < n) s <<= 1;

  euclid_lazy_segtree elst(xy);

  vector<pair<double, double> > ans(q);
  int p = 0;
  for (int i = 0; i <= m; i++) {
    while (p < q) {
      if (ab[query_order[p]].first == i) {
        ans[query_order[p]] = elst.query(ab[query_order[p]].second-1, ab[query_order[p]].second);
        p++;
      }
      else break;
    }
    if (i == m) break;

    if (op[i].first == 1) elst.rotate_90_deg_cw(0, s);
    else if (op[i].first == 2) elst.rotate_90_deg_ccw(0, s);
    else if (op[i].first == 3) {
      elst.flip_x_axis(0, s);
      elst.translate(0, s, op[i].second * 2, 0);
    }
    else if (op[i].first == 4) {
      elst.flip_y_axis(0, s);
      elst.translate(0, s, 0, op[i].second * 2);
    }
  }

  for (auto &x: ans) cout << (ll) (x.first) << " " << (ll) (x.second) << endl;

  return 0;
}
