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

const ll mod = 998244353;

struct lazy_segtree {
  int n;
  vector<ll> data;
  vector<ll> lazy;
  vector<bool> lazyFlag;

  lazy_segtree(const int s) {
    init(s);
  }

  lazy_segtree(const int s, const ll u) {
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = u;
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = data[i*2+1] + data[i*2+2];
    }
  }

  lazy_segtree(const vector<ll> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = data[i*2+1] + data[i*2+2];
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<ll>(2*n-1);
    lazy = vector<ll>(2*n-1);
    lazyFlag = vector<bool>(2*n-1);
  }

  void evaluate(int p, int a, int b) {
    if (lazyFlag[p]) {
      data[p] = lazy[p];
      if (b - a > 1) {
        lazy[p*2+1] = lazy[p];
        lazy[p*2+2] = lazy[p];
        lazyFlag[p*2+1] = true;
        lazyFlag[p*2+2] = true;
      }
      lazy[p] = 0;
      lazyFlag[p] = false;
    }
  }

  void update(int l, int r, ll v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init

    // evaluate lazy value
    evaluate(p, a, b);

    if (r <= a || b <= l) return; // out of range
    if (l <= a && b <= r) { // fully covered
      lazy[p] = v;
      lazyFlag[p] = true;
      evaluate(p, a, b);
    }
    else {
      update(l, r, v, p*2+1, a, (a + b) / 2); // left
      update(l, r, v, p*2+2, (a + b) / 2, b); // right
      data[p] = data[p*2+1] + data[p*2+2]; // update
    }
    return;
  }

  ll query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return vl + vr;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, q;
  cin >> n >> q;
  n -= 2;
  vector<Pii> query(q);
  for (auto &x: query) {
    cin >> x.first >> x.second;
    x.second -= 2;
  }

  lazy_segtree row(n, n), col(n, n);
  ll ans = n * n;
  int row_most = n;
  int col_most = n;
  for (auto &[t, x]: query) {
    if (t == 1) {
      ans -= row.query(x, x+1);
      if (x < row_most) {
        row_most = x;
        col.update(0, col_most, row_most);
      }
    }
    else {
      ans -= col.query(x, x+1);
      if (x < col_most) {
        col_most = x;
        row.update(0, row_most, col_most);
      }
    }
  }

  cout << ans << endl;

  return 0;
}
