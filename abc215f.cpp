#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<Pii> xy(n);
  for (auto &z: xy) cin >> z.first >> z.second;

  sort(xy.begin(), xy.end());

  vector<ll> x(n);
  for (int i = 0; i < n; i++) {
    x[i] = xy[i].first;
  }
  vector<ll> y_min_left(n + 1, 1000000000), y_max_left(n + 1);
  for (int i = 0; i < n; i++) {
    y_min_left[i+1] = min(y_min_left[i], (ll) xy[i].second);
    y_max_left[i+1] = max(y_max_left[i], (ll) xy[i].second);
  }
  vector<ll> y_min_right(n + 1, 1000000000), y_max_right(n + 1);
  for (int i = n-1; i >= 0; i--) {
    y_min_right[i] = min(y_min_right[i+1], (ll) xy[i].second);
    y_max_right[i] = max(y_max_right[i+1], (ll) xy[i].second);
  }

  auto check_existence = [&](Pii target, ll k) {
    auto it_x_pos = lower_bound(x.begin(), x.end(), target.first + k);
    if (it_x_pos != x.end()) {
      int low = distance(x.begin(), it_x_pos);
      if (y_min_right[low] <= target.second - k) return true;
      if (y_max_right[low] >= target.second + k) return true;
    }
    auto it_x_neg = upper_bound(x.begin(), x.end(), target.first - k);
    if (it_x_neg != x.begin()) {
      int high = distance(x.begin(), it_x_neg);
      if (y_min_left[high] <= target.second - k) return true;
      if (y_max_left[high] >= target.second + k) return true;
    }
    return false;
  };

  ll ans = 0;
  for (int i = 0; i < n; i++) {
    ll ok = 0;
    ll ng = 1000000001;
    while (abs(ok - ng) > 1) {
      ll next = (ok + ng) / 2;
      if (check_existence(xy[i], next)) ok = next;
      else ng = next;
    }
    if (ok > ans) ans = ok;
  }

  cout << ans << endl;

  return 0;
}
