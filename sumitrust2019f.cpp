#include <iostream>
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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll t1, t2;
  cin >> t1 >> t2;
  ll a1, a2;
  cin >> a1 >> a2;
  ll b1, b2;
  cin >> b1 >> b2;

  ll as = t1 * a1 + t2 * a2;
  ll bs = t1 * b1 + t2 * b2;
  if (as == bs) {
    cout << "infinity" << endl;
    return 0;
  }

  ll as1 = t1 * a1;
  ll as2 = t2 * a2;
  ll bs1 = t1 * b1;
  ll bs2 = t2 * b2;
  if (as1 > bs1) {
    if (as > bs) {
      cout << 0 << endl;
      return 0;
    }
  }
  else if (bs1 > as1) {
    if (bs > as) {
      cout << 0 << endl;
      return 0;
    }
  }

  ll diff1 = as1 - bs1;
  ll diff2 = bs2 - as2;
  if (diff1 < 0) diff1 = -diff1;
  if (diff2 < 0) diff2 = -diff2;
  ll delta = diff1 - diff2;
  if (delta < 0) delta = -delta;
  ll ans = 0;
  ans += ((diff1 / delta) + 1) * 2 - 1;
  if (diff1 % delta == 0) ans -= 1;
  
  cout << ans << endl;

  return 0;
}
