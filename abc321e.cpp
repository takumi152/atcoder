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
typedef unsigned long long int ull;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<ll> n(t), x(t), k(t);
  for (int i = 0; i < t; i++) {
    cin >> n[i] >> x[i] >> k[i];
  }

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    ll b = x[i];
    ll r = k[i];
    bool last_set = false;
    while (b > 0 && r >= 0) {
      ll shl_limit = __builtin_clzll((ull) b) - 2;
      if (r <= shl_limit) {
        // cerr << "A " << i << " " << b << " " << r << " " << shl_limit << " " << ans[i] << endl;
        if (r == 0) ans[i] += 1LL;
        else if (b == x[i]) {
          ll b_sh_min = b << r;
          ll b_sh_max = (b << r) + (1LL << r) - 1LL;
          ans[i] += max(0LL, min(n[i], b_sh_max) - b_sh_min + 1LL);
        }
        else {
          ll b_low = (b << 1LL) + (last_set ? 0LL : 1LL);
          ll b_sh_min = b_low << (r - 1);
          ll b_sh_max = (b_low << (r - 1)) + (1LL << (r - 1)) - 1LL;
          ans[i] += max(0LL, min(n[i], b_sh_max) - b_sh_min + 1LL);
        }
        // cerr << "B " << i << " " << b << " " << r << " " << shl_limit << " " << ans[i] << endl;
      }
      last_set = b & 1;
      b >>= 1;
      r--;
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
