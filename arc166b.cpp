#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <numeric>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  ll a, b, c;
  cin >> n >> a >> b >> c;
  vector<ll> ar(n);
  for (auto &x: ar) cin >> x;

  vector<pair<ll, int>> a_req(n);
  vector<pair<ll, int>> b_req(n);
  vector<pair<ll, int>> c_req(n);
  vector<pair<ll, int>> ab_req(n);
  vector<pair<ll, int>> ac_req(n);
  vector<pair<ll, int>> bc_req(n);
  vector<pair<ll, int>> abc_req(n);
  for (int i = 0; i < n; i++) {
    a_req[i] = make_pair((a - ar[i] % a) % a, i);
    b_req[i] = make_pair((b - ar[i] % b) % b, i);
    c_req[i] = make_pair((c - ar[i] % c) % c, i);
    ab_req[i] = make_pair((lcm(a, b) - ar[i] % lcm(a, b)) % lcm(a, b), i);
    ac_req[i] = make_pair((lcm(a, c) - ar[i] % lcm(a, c)) % lcm(a, c), i);
    bc_req[i] = make_pair((lcm(b, c) - ar[i] % lcm(b, c)) % lcm(b, c), i);
    abc_req[i] = make_pair((lcm(lcm(a, b), c) - ar[i] % lcm(lcm(a, b), c)) % lcm(lcm(a, b), c), i);
  }
  sort(a_req.begin(), a_req.end());
  sort(b_req.begin(), b_req.end());
  sort(c_req.begin(), c_req.end());
  sort(ab_req.begin(), ab_req.end());
  sort(ac_req.begin(), ac_req.end());
  sort(bc_req.begin(), bc_req.end());
  sort(abc_req.begin(), abc_req.end());

  ll ans = a * b * c;
  // a + b + c
  for (int i = 0; i < min(n, 100); i++) {
    for (int j = 0; j < min(n, 100); j++) {
      for (int k = 0; k < min(n, 100); k++) {
        if (a_req[i].second == b_req[j].second || a_req[i].second == c_req[k].second || b_req[j].second == c_req[k].second) {
          continue;
        }
        ans = min(ans, a_req[i].first + b_req[j].first + c_req[k].first);
      }
    }
  }
  // ab + c
  for (int i = 0; i < min(n, 100); i++) {
    for (int j = 0; j < min(n, 100); j++) {
      if (ab_req[i].second == c_req[j].second) {
        continue;
      }
      ans = min(ans, ab_req[i].first + c_req[j].first);
    }
  }
  // ac + b
  for (int i = 0; i < min(n, 100); i++) {
    for (int j = 0; j < min(n, 100); j++) {
      if (ac_req[i].second == b_req[j].second) {
        continue;
      }
      ans = min(ans, ac_req[i].first + b_req[j].first);
    }
  }
  // bc + a
  for (int i = 0; i < min(n, 100); i++) {
    for (int j = 0; j < min(n, 100); j++) {
      if (bc_req[i].second == a_req[j].second) {
        continue;
      }
      ans = min(ans, bc_req[i].first + a_req[j].first);
    }
  }
  // abc
  for (int i = 0; i < min(n, 100); i++) {
    ans = min(ans, abc_req[i].first);
  }

  cout << ans << endl;

  return 0;
}
