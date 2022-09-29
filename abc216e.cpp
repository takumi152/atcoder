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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, k;
  cin >> n >> k;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  sort(a.rbegin(), a.rend());
  a.push_back(0);

  ll ans = 0;
  ll rem = k;
  for (int i = 0; i < n; i++) {
    ll sat_minimum = max(a[i+1], a[i] - rem / (i + 1));
    ll rem_consumption = (a[i] - sat_minimum) * (i + 1);
    ans += (((a[i] * (a[i] + 1)) / 2) - ((sat_minimum * (sat_minimum + 1)) / 2)) * (i + 1);
    rem -= rem_consumption;
    // cerr << i << " " << ans << " " << sat_minimum << " " << rem << " " << rem_consumption << endl;
    if (sat_minimum != a[i+1]) {
      ans += sat_minimum * rem;
      break;
    }
  }

  cout << ans << endl;

  return 0;
}
