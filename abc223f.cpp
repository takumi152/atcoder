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

struct lazy_segtree {
  int n;
  vector<int> data;
  vector<int> lazy;
  vector<bool> lazyFlag;

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
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  string s;
  cin >> s;
  vector<vector<int> > query(q, vector<int>(3));
  for (auto &x: query) {
    cin >> x[0] >> x[1] >> x[2];
    x[1]--;
    x[2]--;
  }

  vector<int> depth(n+1);
  for (int i = 0; i < n; i++) {
    if (s[i] == '(') depth[i+1] = depth[i] + 1;
    else depth[i+1] = depth[i] - 1;
  }

  lazy_segtree lst(depth);

  vector<string> ans;
  for (int i = 0; i < q; i++) {
    if (query[i][0] == 1) {
      if (s[query[i][1]] == s[query[i][2]]) continue;
      else if (s[query[i][1]] == '(') lst.update(query[i][1] + 1, query[i][2] + 1, -2);
      else lst.update(query[i][1] + 1, query[i][2] + 1, 2);
      swap(s[query[i][1]], s[query[i][2]]);
    }
    else {
      if (lst.query(query[i][1], query[i][1] + 1) != lst.query(query[i][2] + 1, query[i][2] + 2)) {
        ans.push_back("No");
        continue;
      }
      if (lst.query(query[i][1], query[i][2] + 2) < lst.query(query[i][1], query[i][1] + 1)) {
        ans.push_back("No");
        continue;
      }
      ans.push_back("Yes");
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
