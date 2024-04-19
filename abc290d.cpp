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

  int t;
  cin >> t;
  vector<ll> n(t), d(t), k(t);
  for (int i = 0; i < t; i++) cin >> n[i] >> d[i] >> k[i];

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    ll cycle_count = gcd(n[i], d[i]);
    ll cycle_length = n[i] / cycle_count;
    ll target_cycle_num = (k[i] - 1) / cycle_length;
    ll target_cycle_pos = (k[i] - 1) % cycle_length;
    ans[i] = ((d[i] * target_cycle_pos) % n[i]) + target_cycle_num;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
