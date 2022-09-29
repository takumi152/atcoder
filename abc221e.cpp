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

struct mod_affine_lazy_segtree {
  int n;
  vector<ll> data;
  vector<ll> lazy_b;
  vector<ll> lazy_c;
  vector<bool> lazyFlag;

  mod_affine_lazy_segtree() {
    init(1);
  }

  mod_affine_lazy_segtree(const int s) {
    init(s);
  }

  mod_affine_lazy_segtree(const vector<ll> &v) {
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
    lazy_b = vector<ll>(2*n-1, 1);
    lazy_c = vector<ll>(2*n-1, 0);
    lazyFlag = vector<bool>(2*n-1);
  }

  void evaluate(int p, int a, int b) {
    if (lazyFlag[p]) {
      // (b * a[i] + c) % mod
      data[p] = (lazy_b[p] * data[p] + lazy_c[p] * (b - a)) % mod;
      if (b - a > 1) {
        lazy_b[p*2+1] = (lazy_b[p*2+1] * lazy_b[p]) % mod;
        lazy_c[p*2+1] = (lazy_c[p*2+1] * lazy_b[p] + lazy_c[p]) % mod;
        lazy_b[p*2+2] = (lazy_b[p*2+2] * lazy_b[p]) % mod;
        lazy_c[p*2+2] = (lazy_c[p*2+2] * lazy_b[p] + lazy_c[p]) % mod;
        lazyFlag[p*2+1] = true;
        lazyFlag[p*2+2] = true;
      }
      lazy_b[p] = 1;
      lazy_c[p] = 0;
      lazyFlag[p] = false;
    }
  }

  void update(int l, int r, ll vb, ll vc, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init

    // evaluate lazy value
    evaluate(p, a, b);

    if (r <= a || b <= l) return; // out of range
    if (l <= a && b <= r) { // fully covered
      lazy_b[p] = (lazy_b[p] * vb) % mod;
      lazy_c[p] = (lazy_c[p] * vb + vc) % mod;
      lazyFlag[p] = true;
      evaluate(p, a, b);
    }
    else {
      update(l, r, vb, vc, p*2+1, a, (a + b) / 2); // left
      update(l, r, vb, vc, p*2+2, (a + b) / 2, b); // right
      data[p] = (data[p*2+1] + data[p*2+2]) % mod; // update
    }
    return;
  }

  int query(int l, int r, int p = 0, int a = 0, int b = -1) {
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

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<int> a_index(n);
  for (int i = 0; i < n; i++) a_index[i] = i;
  sort(a_index.begin(), a_index.end(), [&](auto &x, auto &y){return Pii(a[x], x) < Pii(a[y], y);});

  vector<int> a_pointer(n);
  for (int i = 0; i < n; i++) a_pointer[a_index[i]] = i;

  ll ans = 0;
  mod_affine_lazy_segtree lst(n);
  for (int i = 0; i < n; i++) {
    ans = (ans + lst.query(a_pointer[i], a_pointer[i] + 1)) % mod;
    lst.update(0, n, 2, 0);
    lst.update(a_pointer[i], n, 1, 1);
  }

  cout << ans << endl;

  return 0;
}
