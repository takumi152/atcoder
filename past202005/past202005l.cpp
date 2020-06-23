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

struct segtree {
  int n;
  vector<int> data;

  segtree(const int s) {
    init(s);
  }

  segtree(const int s, const int u) {
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = u;
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = max(data[i*2+1], data[i*2+2]);
    }
  }

  segtree(const vector<int> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = 0;
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<int>(2*n-1);
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
    if (r <= a || b <= l) return -1; // out of range

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1)                         return p - (n - 1);
      else if (data[p*2+1] >= data[p*2+2])  return maxdex(l, r, p*2+1, a, (a + b) / 2);
      else                                  return maxdex(l, r, p*2+2, (a + b) / 2, b);
    }

    int vl = maxdex(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = maxdex(l, r, p*2+2, (a + b) / 2, b); // right
    if (vl == -1 && vr == -1) return -1;
    else if (vl == -1) return vr;
    else if (vr == -1) return vl;
    else if (data[vl+n-1] >= data[vr+n-1]) return vl;
    else return vr;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> k(n);
  vector<vector<int> > t(n);
  for (int i = 0; i < n; i++) {
    cin >> k[i];
    t[i] = vector<int>(k[i]);
    for (auto &x: t[i]) cin >> x;
  }
  int m;
  cin >> m;
  vector<int> a(m);
  for (auto &x: a) cin >> x;

  segtree st1(n), st2(n);
  vector<int> now(n), next(n);
  for (int i = 0; i < n; i++) {
    st1.update(i, t[i][0]);
    now[i] = 0;
    if (k[i] >= 2) {
      st2.update(i, t[i][1]);
      next[i] = 1;
    }
    else next[i] = -1;
  }

  vector<int> ans(m);
  for (int i = 0; i < m; i++) {
    if (a[i] == 1) {
      ans[i] = st1.query(0, n);
      int idx = st1.maxdex(0, n);
      now[idx] = next[idx];
      if (next[idx]+1 < k[idx] && next[idx] != -1) next[idx]++;
      else next[idx] = -1;
      if (now[idx] >= 0) st1.update(idx, t[idx][now[idx]]);
      else st1.update(idx, 0);
      if (next[idx] >= 0) st2.update(idx, t[idx][next[idx]]);
      else st2.update(idx, 0);
    }
    else {
      int t1 = st1.query(0, n);
      int t2 = st2.query(0, n);
      if (t1 > t2) {
        ans[i] = t1;
        int idx = st1.maxdex(0, n);
        now[idx] = next[idx];
        if (next[idx]+1 < k[idx] && next[idx] != -1) next[idx]++;
        else next[idx] = -1;
        if (now[idx] >= 0) st1.update(idx, t[idx][now[idx]]);
        else st1.update(idx, 0);
        if (next[idx] >= 0) st2.update(idx, t[idx][next[idx]]);
        else st2.update(idx, 0);
      }
      else {
        ans[i] = t2;
        int idx = st2.maxdex(0, n);
        if (next[idx]+1 < k[idx] && next[idx] != -1) next[idx]++;
        else next[idx] = -1;
        if (next[idx] >= 0) st2.update(idx, t[idx][next[idx]]);
        else st2.update(idx, 0);
      }
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
