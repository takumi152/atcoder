#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
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

  int n, m;
  cin >> n >> m;
  vector<vector<int>> a(n, vector<int>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) cin >> a[i][j];
  }

  vector<Pii> merged;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      merged.emplace_back(a[i][j], i);
    }
  }
  sort(merged.begin(), merged.end());

  ll ans = 0;
  fenwick_tree appear_count(n);
  for (int i = 0; i < n * m; i++) {
    auto [_, idx] = merged[i];
    appear_count.add(idx, 1);
    ans += (n - (idx + 1)) * appear_count.sum(idx, idx + 1);
    ans += appear_count.sum(idx + 1, n);
  }

  cout << ans << endl;

  return 0;
}
