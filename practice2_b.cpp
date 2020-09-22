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

struct fenwick_tree {
  int n;
  vector<ll> data;

  fenwick_tree() {
    init(0);
  }

  fenwick_tree(int s) {
    init(s);
  }

  fenwick_tree(vector<ll> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) add(i, v[i]);
  }

  void init(int s) {
    n = s;
    data = vector<ll>(s);
  }

  void add(int p, ll v) {
    p++;
    while (p <= n) {
      data[p-1] += v;
      p += p & -p;
    }
  }

  ll sum(int l, int r) {
    return sum(r) - sum(l);
  }

  ll sum(int r) {
    ll s = 0;
    while (r > 0) {
      s += data[r-1];
      r -= r & -r;
    }
    return s;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;
  vector<vector<int> > query(q, vector<int>(3));
  for (auto &x: query) cin >> x[0] >> x[1] >> x[2];

  fenwick_tree ft(a);

  vector<ll> ans;
  for (int i = 0; i < q; i++) {
    if      (query[i][0] == 0) ft.add(query[i][1], query[i][2]);
    else if (query[i][0] == 1) ans.push_back(ft.sum(query[i][1], query[i][2]));
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
