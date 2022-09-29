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
  vector<Pii> ab(n);
  for (auto &x: ab) cin >> x.first;
  for (auto &x: ab) cin >> x.second;

  // a_i >= a_j
  // b_i <= b_j

  sort(ab.begin(), ab.end(), [&](auto &a, auto &b){return Pii(a.second, a.first) < Pii(b.second, b.first);});

  unordered_map<int, int> b_to_index;
  for (int i = 0; i < n; i++) {
    if (b_to_index.find(ab[i].second) == b_to_index.end()) b_to_index[ab[i].second] = i;
  }

  sort(ab.rbegin(), ab.rend());

  fenwick_tree ft(n);
  for (int i = 0; i < n; i++) ft.add(i, 1);
  ll ans = 0;
  int p = 0;
  int last_a = 0;
  for (int i = 0; i < n; i++) {
    if (ab[i].first != last_a) {
      while (p < i) {
        ft.add(b_to_index[ab[p].second], -1);
        p++;
      }
    }
    ans += ft.sum(b_to_index[ab[i].second], n);
    last_a = ab[i].first;
  }

  cout << ans << endl;

  return 0;
}
