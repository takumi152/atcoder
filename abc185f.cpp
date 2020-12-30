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

const ll mod = 1000000007;

struct xor_segtree {
  int n;
  vector<int> data;

  xor_segtree() {
    init(1);
  }

  xor_segtree(const int s) {
    init(s);
  }

  xor_segtree(const vector<int> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = data[i*2+1] ^ data[i*2+2];
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
    data[p] ^= v;

    // update rest
    while (p > 0) {
      p = (p - 1) >> 1;
      data[p] = data[p*2+1] ^ data[p*2+2];
    }
  }

  int query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0; // out of range

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return vl ^ vr;
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
  for (auto &x: query) {
    for (auto &y: x) cin >> y;
  }

  xor_segtree st(a);

  vector<int> ans;
  for (auto &x: query) {
    if (x[0] == 1) st.update(x[1]-1, x[2]);
    else ans.push_back(st.query(x[1]-1, x[2]));
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
