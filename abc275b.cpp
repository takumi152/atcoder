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

  ll a, b, c, d, e, f;
  cin >> a >> b >> c >> d >> e >> f;

  ll abc = ((((a % mod) * (b % mod)) % mod) * (c % mod)) % mod;
  ll def = ((((d % mod) * (e % mod)) % mod) * (f % mod)) % mod;
  ll ans = (abc - def) % mod;
  if (ans < 0) ans += mod;

  cout << ans << endl;

  return 0;
}
