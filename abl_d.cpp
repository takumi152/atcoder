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

const ll mod = 1000000007;


struct max_segtree {
  int n;
  vector<int> data;

  max_segtree() {
    init(1);
  }

  max_segtree(const int s) {
    init(s);
  }

  max_segtree(const int s, const int u) {
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = u;
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = max(data[i*2+1], data[i*2+2]);
    }
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

  int n, k;
  cin >> n >> k;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  max_segtree st(300001, 0);
  st.update(a[0], 1);
  for (int i = 1; i < n; i++) {
    int maximum = st.query(max(0, a[i]-k), min(300001, a[i]+k+1));
    st.update(a[i], maximum+1);
  }

  int ans = st.query(0, 300001);

  cout << ans << endl;

  return 0;
}
