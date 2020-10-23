#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

vector<vector<ll> > mval(10, vector<ll>(500001));

void init_mval() {
  for (int i = 1; i <= 9; i++) {
    int now = i;
    mval[i][0] = now;
    for (int j = 1; j <= 500000; j++) {
      now = (now * 10) % mod;
      mval[i][j] = (mval[i][j-1] + now) % mod;
    }
  }
}

ll ipow10(ll x, ll m = mod) {
  ll v = 1;
  ll f = 10;
  while (x > 0) {
    if (x & 1) v = (v * f) % m;
    x >>= 1;
    f = (f * f) % m;
  }
  return v;
}

struct lazy_segtree {
  int n;
  vector<ll> data;
  vector<ll> lazy;
  vector<bool> lazyFlag;

  lazy_segtree() {
    init(1);
  }

  lazy_segtree(const int s) {
    init(s);
    update(0, s, 1);
  }

  lazy_segtree(const vector<ll> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = (data[i*2+1] + data[i*2+2]) % mod;
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<ll>(2*n-1, 0);
    lazy = vector<ll>(2*n-1, 0);
    lazyFlag = vector<bool>(2*n-1);
  }

  void evaluate(int p, int a, int b) {
    if (lazyFlag[p]) {
      int offset = 0;
      int factor = 1;
      for (int k = p; k > 0; k >>= 1) {
        if (k & 1) offset += factor;
        factor <<= 1;
        k >>= 1;
      }
      data[p] = (mval[lazy[p]][b-a-1] * ipow10(offset)) % mod;
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
      data[p] = (data[p*2+1] + data[p*2+2]) % mod; // update
    }
    return;
  }

  ll query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) return data[p];// fully covered

    ll vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    ll vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return (vl + vr) % mod;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  vector<vector<int> > query(q, vector<int>(3));
  for (auto &x: query) {
    cin >> x[0] >> x[1] >> x[2];
    x[0] = n - x[0] + 1;
    x[1] = n - x[1];
  }

  init_mval();

  lazy_segtree lst(n);
  vector<ll> ans(q);
  for (int i = 0; i < q; i++) {
    lst.update(query[i][1], query[i][0], query[i][2]);
    ans[i] = lst.query(0, n);
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
