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
  vector<int> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  vector<fenwick_tree> dp(n, fenwick_tree(3001));
  for (int j = a[0]; j <= b[0]; j++) dp[0].add(j, 1);
  for (int i = 1; i < n; i++) {
    for (int j = b[i]; j >= a[i]; j--) {
      dp[i].add(j, dp[i-1].sum(0, j+1) % mod);
    }
  }

  cout << dp[n-1].sum(0, 3001) % mod << endl;

  return 0;
}
