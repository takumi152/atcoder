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

  int n, k;
  cin >> n >> k;
  vector<vector<int> > v(n, vector<int>(k));
  for (auto &x: v) {
    for (auto &y: x) cin >> y;
  }

  vector<fenwick_tree> dp(n, fenwick_tree(k));
  for (int i = 0; i < k; i++) dp[0].add(i, 1);
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < k; j++) {
      dp[i].add(j, dp[i-1].sum(0, distance(v[i-1].begin(), upper_bound(v[i-1].begin(), v[i-1].end(), v[i][j]))) % mod);
    }
  }

  ll ans = dp[n-1].sum(0, k) % mod;

  cout << ans << endl;

  return 0;
}
