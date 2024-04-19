#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m, d;
  cin >> n >> m >> d;
  vector<ll> a(n), b(m);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  sort(a.begin(), a.end());
  sort(b.begin(), b.end());

  ll ans = -1;
  for (int i = 0; i < n; i++) {
    auto it = upper_bound(b.begin(), b.end(), a[i] + d);
    if (it != b.begin()) {
      ll x = *(it - 1);
      if (abs(a[i] - x) <= d && a[i] + x > ans) {
        ans = a[i] + x;
      }
    }
  }

  for (int i = 0; i < m; i++) {
    auto it = upper_bound(a.begin(), a.end(), b[i] + d);
    if (it != a.begin()) {
      ll x = *(it - 1);
      if (abs(b[i] - x) <= d && b[i] + x > ans) {
        ans = b[i] + x;
      }
    }
  }

  cout << ans << endl;

  return 0;
}
