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

struct lazy_segtree {
  int n;
  vector<ll> data;
  vector<ll> lazy;

  lazy_segtree(const int s) {
    init(s);
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
  }

  void evaluate(int p) {
    if (lazy[p] != 0) {
      data[p] += lazy[p];
      if (p <= n-2) {
        lazy[p*2+1] += lazy[p] / 2;
        lazy[p*2+2] += lazy[p] / 2;
      }
      lazy[p] = 0;
    }
  }

  void update(int l, int r, ll v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init

    // evaluate lazy value
    evaluate(p);

    if (r <= a || b <= l) return; // out of range
    if (l <= a && b <= r) { // fully covered
      lazy[p] += v * (b - a);
      evaluate(p);
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
    if (r <= a || b <= l) return 0LL; // out of range

    // evaluate lazy value
    evaluate(p);

    if (l <= a && b <= r) return data[p];// fully covered

    ll vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    ll vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return vl + vr;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<vector<int> > edge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }

  lazy_segtree lst(n);
  ll ans = 0;
  for (int i = 0; i < n; i++) {
    lst.update(0, i+1, 1);
    for (auto &x: edge[i]) {
      if (x < i) {
        lst.update(0, x+1, -1);
      }
    }
    ans += lst.query(0, n);
  }

  cout << ans << endl;

  return 0;
}
