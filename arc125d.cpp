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

const ll mod = 998244353;

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

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) {
    cin >> x;
    x--;
  }

  vector<vector<int> > appearance(n);
  for (int i = 0; i < n; i++) appearance[a[i]].push_back(i);

  fenwick_tree ft(n);
  for (int i = 0; i < n; i++) {
    int low;
    auto it1 = lower_bound(appearance[a[i]].begin(), appearance[a[i]].end(), i);
    if (it1 == appearance[a[i]].begin()) low = 0;
    else {
      auto it2 = it1;
      it2--;
      low = *it2;
    }
    ft.add(i, ft.sum(low, i) % mod);
    if (it1 != appearance[a[i]].begin()) ft.add(low, -ft.sum(low, low+1));
    else ft.add(i, 1);
    // cerr << i << endl;
    // for (int j = 0; j < n; j++) cerr << ft.sum(j, j+1) << " ";
    // cerr << endl;
  }

  ll ans = ft.sum(n) % mod;

  cout << ans << endl;

  return 0;
}
