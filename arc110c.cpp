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

  ll n;
  cin >> n;
  vector<int> p(n);
  for (auto &x: p) cin >> x;

  ll inverse_num = 0;
  fenwick_tree ft(n+1);
  for (int i = 0; i < n; i++) {
    inverse_num += ft.sum(p[i], n+1);
    ft.add(p[i], 1);
  }
  if (inverse_num != n-1) {
    cout << -1 << endl;
    return 0;
  }

  vector<int> ans;
  vector<bool> found(n+1);
  int now = 1;
  int last = 0;
  for (int i = 0; i < n; i++) {
    found[p[i]] = true;
    if (found[now]) {
      for (int j = i-1; j >= last; j--) {
        swap(p[j], p[j+1]);
        ans.push_back(j+1);
      }
      last = i;
      while (now <= n) {
        if (found[now]) now++;
        else break;
      }
    }
  }

  for (int i = 0; i < n; i++) {
    if (p[i] != i+1) {
      cout << -1 << endl;
      return 0;
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
