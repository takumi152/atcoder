#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;
  int q;
  cin >> q;
  vector<Pii> lr(q);
  for (auto &[l, r]: lr) {
    cin >> l >> r;
    l--;
    r--;
  }

  vector<vector<ll>> a_mod_sum(k, vector<ll>({0}));
  for (int i = 0; i < n; i++) {
    a_mod_sum[i % k].push_back(a_mod_sum[i % k].back() + a[i]);
  }

  vector<string> ans(q);
  for (int i = 0; i < q; i++) {
    auto [l, r] = lr[i];
    ll target = (ll) 9e18;
    bool good = true;
    for (int j = 0; j < k; j++) {
      int begin = (l - j + k - 1) / k;
      int end = (r - j) / k + 1;
      int sum = a_mod_sum[j][end] - a_mod_sum[j][begin];
      if (target == (ll) 9e18) target = sum;
      else if (target != sum) good = false;
    }
    if (good) ans[i] = "Yes";
    else ans[i] = "No";
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
