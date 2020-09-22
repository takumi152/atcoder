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
      data[i] = (data[i*2+1] + data[i*2+2]) % mod;
    }
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
    data = vector<ll>(2*n-1);
    lazy = vector<ll>(2*n-1);
    lazyFlag = vector<bool>(2*n-1);
  }

  void evaluate(int p, int a, int b) {
    if (lazyFlag[p]) {
      data[p] = (data[p] + lazy[p]) % mod;
      if (b - a > 1) {
        lazy[p*2+1] = (lazy[p*2+1] + lazy[p]) % mod;
        lazy[p*2+2] = (lazy[p*2+2] + lazy[p]) % mod;
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
      lazy[p] = (lazy[p] + v) % mod;
      lazyFlag[p] = true;
      evaluate(p, a, b);
    }
    else {
      update(l, r, v, p*2+1, a, (a + b) / 2); // left
      update(l, r, v, p*2+2, (a + b) / 2, b); // right
      data[p] = (data[p*2+1] + data[p*2+2]) % mod;; // update
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
    return (vl + vr) % mod;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  vector<Pii> lr(k);
  for (auto &x: lr) cin >> x.first >> x.second;

  lazy_segtree lst(n);
  lst.update(0, 1, 1);
  for (int i = 0; i < n; i++) {
    ll base = lst.query(i, i+1);
    for (int j = 0; j < k; j++) {
      lst.update(min(n, i+lr[j].first), min(n, i+lr[j].second+1), base);
    }
  }

  ll ans = lst.query(n-1, n);

  cout << ans << endl;

  return 0;
}
