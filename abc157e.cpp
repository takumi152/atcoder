#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;


template <class T>
struct segtree {
  int n;
  vector<T> data;

  segtree(const int s) {
    init(s);
  }

  segtree(const int s, const T u) {
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = u;
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = min(data[i*2+1], data[i*2+2]);
    }
  }

  segtree(const vector<T> &v) {
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
      data[p] = min(data[p*2+1], data[p*2+2]);
    }
  }

  T query(int l, int r, int p = 0, int a = 0, int b = -1) {
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

  int n;
  cin >> n;
  string s;
  cin >> s;
  int q;
  cin >> q;
  vector<vector<int> > query(q, vector<int>(3));
  for (auto &x: query) {
    cin >> x[0] >> x[1];
    if (x[0] == 1) {
      char c;
      cin >> c;
      x[2] = c;
    }
    else cin >> x[2];
  }

  vector<segtree<int> > st(26, segtree<int>(n));
  for (int i = 0; i < n; i++) {
    st[s[i]-'a'].update(i, -1);
  }

  vector<int> ans;
  for (int i = 0; i < q; i++) {
    if (query[i][0] == 1) {
      st[s[query[i][1]-1]-'a'].update(query[i][1]-1, 0);
      s[query[i][1]-1] = (char) query[i][2];
      st[s[query[i][1]-1]-'a'].update(query[i][1]-1, -1);
    }
    else {
      int count = 0;
      for (int j = 0; j < 26; j++) count += -(st[j].query(query[i][1]-1, query[i][2]));
      ans.push_back(count);
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
