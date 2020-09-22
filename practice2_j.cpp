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

struct max_segtree {
  int n;
  vector<int> data;

  max_segtree() {
    init(1);
  }

  max_segtree(const int s) {
    init(s);
  }

  max_segtree(const vector<int> &v) {
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
  }

  void update(int x, int v) {
    int p = x + n - 1;

    // update bottom
    data[p] = v;

    // update rest
    while (p > 0) {
      p = (p - 1) >> 1;
      data[p] = max(data[p*2+1], data[p*2+2]);
    }
  }

  int query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0x80000000; // out of range

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return max(vl, vr);
  }

  int maxdex(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return r; // out of range

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1)                         return p - (n - 1);
      else if (data[p*2+1] >= data[p*2+2])  return maxdex(l, r, p*2+1, a, (a + b) / 2);
      else                                  return maxdex(l, r, p*2+2, (a + b) / 2, b);
    }

    int vl = maxdex(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = maxdex(l, r, p*2+2, (a + b) / 2, b); // right
    if (vl == r && vr == r) return r;
    else if (vl == r) return vr;
    else if (vr == r) return vl;
    else if (data[vl+n-1] >= data[vr+n-1]) return vl;
    else return vr;
  }

  int first_greater_or_equal_than(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return r; // out of range

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1) {
        if (data[p] >= v)         return p - (n - 1);
        else                      return r; // not found
      }
      else if (data[p*2+1] >= v)  return first_greater_or_equal_than(l, r, v, p*2+1, a, (a + b) / 2); // left
      else if (data[p*2+2] >= v)  return first_greater_or_equal_than(l, r, v, p*2+2, (a + b) / 2, b); // right
      else                        return r; // not found
    }

    int vl = first_greater_or_equal_than(l, r, v, p*2+1, a, (a + b) / 2); // left
    int vr = first_greater_or_equal_than(l, r, v, p*2+2, (a + b) / 2, b); // right
    if (vl == r && vr == r) return r;
    else if (vl == r) return vr;
    else if (vr == r) return vl;
    else return vl;
  }

  int last_greater_or_equal_than(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return r; // out of range

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1) {
        if (data[p] >= v)         return p - (n - 1);
        else                      return r; // not found
      }
      else if (data[p*2+2] >= v)  return last_greater_or_equal_than(l, r, v, p*2+2, (a + b) / 2, b); // right
      else if (data[p*2+1] >= v)  return last_greater_or_equal_than(l, r, v, p*2+1, a, (a + b) / 2); // left
      else                        return r; // not found
    }

    int vl = last_greater_or_equal_than(l, r, v, p*2+1, a, (a + b) / 2); // left
    int vr = last_greater_or_equal_than(l, r, v, p*2+2, (a + b) / 2, b); // right
    if (vl == r && vr == r) return r;
    else if (vl == r) return vr;
    else if (vr == r) return vl;
    else return vr;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  vector<int> a(n);
  for (auto &x: a) cin >> x;
  vector<vector<int> > query(q, vector<int>(3));
  for (auto &x: query) cin >> x[0] >> x[1] >> x[2];

  max_segtree mst(a);

  vector<int> ans;
  for (auto &x: query) {
    if (x[0] == 1) mst.update(x[1]-1, x[2]);
    else if (x[0] == 2) ans.push_back(mst.query(x[1]-1, x[2]));
    else if (x[0] == 3) ans.push_back(mst.first_greater_or_equal_than(x[1]-1, n, x[2]) + 1);
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
