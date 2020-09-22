#include <iostream>
#include <iomanip>
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

  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  ll mult = 0;
  for (int i = 0; i < n; i++) mult = (mult + a[i]) % mod;

  ll ans = 0;
  for (int i = 0; i < n; i++) {
    mult -= a[i];
    if (mult < 0) mult += mod;
    ans = (ans + (a[i] * mult)) % mod;
  }

  cout << ans << endl;

  return 0;
}
