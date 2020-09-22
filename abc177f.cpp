#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

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
      data[i] = min(data[i*2+1], data[i*2+2]);
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
      data[p] = min(data[p*2+1], data[p*2+2]);
    }
  }

  int query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0x7fffffff; // out of range

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return min(vl, vr);
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<Pii> ab(h);
  for (auto &x: ab) {
    cin >> x.first >> x.second;
    x.first--;
    x.second--;
  }

  segtree st(w+1, 0);
  set<int> relevant;
  for (int i = 0; i < w; i++) relevant.insert(i);

  vector<int> ans(h);
  for (int i = 0; i < h; i++) {
    auto it = relevant.lower_bound(ab[i].first);
    if (it != relevant.end()) {
      int next_blocked = *it;
      int minimum_move = 1000000007;
      while (next_blocked <= ab[i].second) {
        minimum_move = min(minimum_move, ab[i].second - next_blocked + st.query(next_blocked, next_blocked+1) + 1);
        st.update(next_blocked, 1000000007);
        it = relevant.erase(it);
        if (it != relevant.end()) next_blocked = *it;
        else break;
      }
      st.update(ab[i].second+1, min(st.query(ab[i].second+1, ab[i].second+2), minimum_move));
      if (ab[i].second+1 < w && st.query(ab[i].second+1, ab[i].second+2) < 1000000007) relevant.insert(ab[i].second+1);
    }
    ans[i] = st.query(0, w) + i + 1;
    if (ans[i] >= 1000000007) ans[i] = -1;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
