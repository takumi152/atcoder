#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll x, y;
  cin >> x >> y;

  if (x > y) {
    cout << x - y << endl;
    return 0;
  }

  unordered_map<ll, ll> extra_op;
  extra_op[y] = 0;
  ll low = y;
  ll high = y;
  while (low > 0 && high > 0) {
    if (extra_op.find(low-1) == extra_op.end()) extra_op[low-1] = extra_op[low] + 1;
    else extra_op[low-1] = min(extra_op[low-1], extra_op[low] + 1);
    if (extra_op.find(low+1) == extra_op.end()) extra_op[low+1] = extra_op[low] + 1;
    else extra_op[low+1] = min(extra_op[low+1], extra_op[low] + 1);
    if (extra_op.find(high-1) == extra_op.end()) extra_op[high-1] = extra_op[high] + 1;
    else extra_op[high-1] = min(extra_op[high-1], extra_op[high] + 1);
    if (extra_op.find(high+1) == extra_op.end()) extra_op[high+1] = extra_op[high] + 1;
    else extra_op[high+1] = min(extra_op[high+1], extra_op[high] + 1);
    if (low % 2 != 0) {
      low--;
    }
    if (high % 2 != 0) {
      high++;
    }
    if (extra_op.find(low/2) == extra_op.end()) extra_op[low/2] = extra_op[low] + 1;
    else extra_op[low/2] = min(extra_op[low/2], extra_op[low] + 1);
    low /= 2;
    if (extra_op.find(high/2) == extra_op.end()) extra_op[high/2] = extra_op[high] + 1;
    else extra_op[high/2] = min(extra_op[high/2], extra_op[high] + 1);
    high /= 2;
  }

  ll ans = (ll) 9e18;
  for (auto &[init, op]: extra_op) {
    // cerr << init << " " << op << endl;
    ans = min(ans, abs(x - init) + op);
  }

  cout << ans << endl;

  return 0;
}
