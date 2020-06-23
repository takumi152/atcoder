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
  vector<int> data;
  vector<int> lazy;
  vector<bool> lazyFlag;

  lazy_segtree() {
    init(1);
  }

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

  void update(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
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

  int mindex(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return -1; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1)                                                     return p - (n - 1);
      else if (data[p*2+1] + lazy[p*2+1] <= data[p*2+2] + lazy[p*2+2])  return mindex(l, r, p*2+1, a, (a + b) / 2);
      else                                                              return mindex(l, r, p*2+2, (a + b) / 2, b);
    }

    int vl = mindex(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = mindex(l, r, p*2+2, (a + b) / 2, b); // right
    if (vl == -1 && vr == -1) return -1;
    else if (vl == -1) return vr;
    else if (vr == -1) return vl;
    else if (data[vl+n-1] < data[vr+n-1]) return vl;
    else return vr;
  }

  int first_less_than(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return r; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1) {
        if (data[p] < v)         return p - (n - 1);
        else                     return r; // not found
      }
      else if (data[p*2+1] + lazy[p*2+1] < v)  return first_less_than(l, r, v, p*2+1, a, (a + b) / 2); // left
      else if (data[p*2+2] + lazy[p*2+2] < v)  return first_less_than(l, r, v, p*2+2, (a + b) / 2, b); // right
      else                                     return r; // not found
    }

    int vl = first_less_than(l, r, v, p*2+1, a, (a + b) / 2); // left
    int vr = first_less_than(l, r, v, p*2+2, (a + b) / 2, b); // right
    if (vl == r && vr == r) return r;
    else if (vl == r) return vr;
    else if (vr == r) return vl;
    else return vl;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  vector<int> t(q), x(q), y(q);
  for (int i = 0; i < q; i++) {
    cin >> t[i] >> x[i] >> y[i];
    x[i]--;
    y[i]--;
  }

  vector<int> a(n);
  for (int i = 0; i < n; i++) a[i] = i;

  lazy_segtree lst(n);

  for (int i = 0; i < q; i++) {
    if (t[i] == 1) {
      swap(a[x[i]], a[x[i]+1]);
      lst.update(x[i], x[i]+2, -1);
    }
    else {
      while (true) {
        int mindex = lst.first_less_than(x[i], y[i]+1, 0);
        if (mindex == y[i]+1) break;
        bool swapped = false;
        if (mindex > x[i] && !swapped) {
          if (a[mindex-1] > a[mindex]) {
            swap(a[mindex-1], a[mindex]);
            lst.update(mindex-1, mindex+1, -1);
            swapped = true;
          }
        }
        if (mindex < y[i] && !swapped) {
          if (a[mindex] > a[mindex+1]) {
            swap(a[mindex], a[mindex+1]);
            lst.update(mindex, mindex+2, -1);
            swapped = true;
          }
        }
        if (!swapped) lst.update(mindex, mindex+1, 0);
      }
    }
  }

  for (auto &z: a) cout << z+1 << " ";
  cout << endl;

  return 0;
}
