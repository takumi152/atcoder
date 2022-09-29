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

template <class T>
struct max_segtree {
  int n;
  vector<T> data;

  max_segtree(const int s) {
    init(s);
  }

  max_segtree(const int s, const T u) {
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = u;
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = max(data[i*2+1], data[i*2+2]);
    }
  }

  max_segtree(const vector<T> &v) {
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
    data = vector<T>(2*n-1);
  }

  void update(int x, T v) {
    int p = x + n - 1;

    // update bottom
    data[p] = v;

    // update rest
    while (p > 0) {
      p = (p - 1) >> 1;
      data[p] = max(data[p*2+1], data[p*2+2]);
    }
  }

  T query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0x80000000; // out of range

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return max(vl, vr);
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<Pii> ab(m);
  for (auto &x: ab) {
    cin >> x.first >> x.second;
    x.second = -x.second;
  }

  sort(ab.begin(), ab.end());

  for (auto &x: ab) x.second = -x.second;

  max_segtree<int> st(n+1);
  for (int i = 0; i < m; i++) {
    st.update(ab[i].second, max(st.query(ab[i].second, ab[i].second + 1), st.query(0, ab[i].second) + 1));
  }

  cout << st.query(0, n+1) << endl;

  return 0;
}
