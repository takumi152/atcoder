// WA

#include <iostream>
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

struct min_lazy_segtree {
  int n;
  vector<int> data;
  vector<int> lazy;
  vector<bool> lazyFlag;

  min_lazy_segtree() {
    init(1);
  }

  min_lazy_segtree(const int s) {
    init(s);
  }

  min_lazy_segtree(const vector<int> &v) {
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
    data = vector<int>(2*n-1, 0x7fffffff);
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

  int maxdex(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return -1; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1)                                                    return p - (n - 1);
      else if (data[p*2+1] + lazy[p*2+1] > data[p*2+2] + lazy[p*2+2])  return maxdex(l, r, p*2+1, a, (a + b) / 2);
      else                                                             return maxdex(l, r, p*2+2, (a + b) / 2, b);
    }

    int vl = maxdex(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = maxdex(l, r, p*2+2, (a + b) / 2, b); // right
    if (vl == -1 && vr == -1) return -1;
    else if (vl == -1) return vr;
    else if (vr == -1) return vl;
    else if (data[vl+n-1] > data[vr+n-1]) return vl;
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

  int last_greater_than_or_equal(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return r; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1) {
        if (data[p] >= v)         return p - (n - 1);
        else                      return r; // not found
      }
      else if (data[p*2+2] + lazy[p*2+2] >= v)  return last_greater_than_or_equal(l, r, v, p*2+2, (a + b) / 2, b); // right
      else if (data[p*2+1] + lazy[p*2+1] >= v)  return last_greater_than_or_equal(l, r, v, p*2+1, a, (a + b) / 2); // left
      else                                      return r; // not found
    }

    int vl = last_greater_than_or_equal(l, r, v, p*2+1, a, (a + b) / 2); // left
    int vr = last_greater_than_or_equal(l, r, v, p*2+2, (a + b) / 2, b); // right
    if (vl == r && vr == r) return r;
    else if (vl == r) return vr;
    else if (vr == r) return vl;
    else return vr;
  }
};

struct max_lazy_segtree {
  int n;
  vector<int> data;
  vector<int> lazy;
  vector<bool> lazyFlag;

  max_lazy_segtree() {
    init(1);
  }

  max_lazy_segtree(const int s) {
    init(s);
  }

  max_lazy_segtree(const vector<int> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = max(data[i*2+1], data[i*2+2]);
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<int>(2*n-1, 0x80000000);
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
      data[p] = max(data[p*2+1], data[p*2+2]); // update
    }
    return;
  }

  int query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0x80000000; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return max(vl, vr);
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

  int maxdex(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return -1; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1)                                                    return p - (n - 1);
      else if (data[p*2+1] + lazy[p*2+1] > data[p*2+2] + lazy[p*2+2])  return maxdex(l, r, p*2+1, a, (a + b) / 2);
      else                                                             return maxdex(l, r, p*2+2, (a + b) / 2, b);
    }

    int vl = maxdex(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = maxdex(l, r, p*2+2, (a + b) / 2, b); // right
    if (vl == -1 && vr == -1) return -1;
    else if (vl == -1) return vr;
    else if (vr == -1) return vl;
    else if (data[vl+n-1] > data[vr+n-1]) return vl;
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

  int last_greater_than_or_equal(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return r; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1) {
        if (data[p] >= v)         return p - (n - 1);
        else                      return r; // not found
      }
      else if (data[p*2+2] + lazy[p*2+2] >= v)  return last_greater_than_or_equal(l, r, v, p*2+2, (a + b) / 2, b); // right
      else if (data[p*2+1] + lazy[p*2+1] >= v)  return last_greater_than_or_equal(l, r, v, p*2+1, a, (a + b) / 2); // left
      else                                      return r; // not found
    }

    int vl = last_greater_than_or_equal(l, r, v, p*2+1, a, (a + b) / 2); // left
    int vr = last_greater_than_or_equal(l, r, v, p*2+2, (a + b) / 2, b); // right
    if (vl == r && vr == r) return r;
    else if (vl == r) return vr;
    else if (vr == r) return vl;
    else return vr;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  int left_count = 0;
  int right_count = 0;
  for (auto &x: s) {
    for (auto &y: x) {
      if (y == '(') left_count++;
      else right_count++;
    }
  }

  if (left_count != right_count) {
    cout << "No" << endl;
    return 0;
  }

  vector<Pii> cc(n);
  for (int i = 0; i < n; i++) {
    int bmin = 0;
    int blast = 0;
    for (auto &x: s[i]) {
      if (x == '(') blast++;
      else blast--;
      if (blast < bmin) bmin = blast;
    }
    cc[i] = Pii(bmin, blast);
  }

  sort(cc.rbegin(), cc.rend());
  vector<int> bm(n), bl(n);
  for (int i = 0; i < n; i++) {
    bm[i] = cc[i].first;
    bl[i] = cc[i].second;
  }

  max_lazy_segtree stbm(bm);
  max_lazy_segtree stbl(bl);
  int u = 0;
  for (int i = 0; i < n; i++) {
    int last_ge = stbm.last_greater_than_or_equal(0, n, -u);
    int maxdex = stbl.maxdex(0, min(n, last_ge+1));
    int val = stbl.query(maxdex, maxdex+1);
    u += val;
    cerr << i << " " << last_ge << " " << maxdex << " " << val << " " << u << endl;
    for (auto &x: stbm.data) cerr << x << " ";
    cerr << endl;
    for (auto &x: stbl.data) cerr << x << " ";
    cerr << endl;
    if (u < 0) {
      cout << "No" << endl;
      return 0;
    }
    stbl.update(maxdex, maxdex+1, -1000000007);
  }

  cout << "Yes" << endl;

  return 0;
}
