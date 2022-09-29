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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, l, w;
  cin >> n >> l >> w;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  int p = 0;
  ll now = 0;
  ll ans = 0;
  while (p < n) {
    while (p < n) {
      if (a[p] <= now) {
        now = a[p] + w;
        p++;
      }
      else break;
    }
    ll sheet_to_add;
    if (p < n) sheet_to_add = ((a[p] - now) + (w - 1)) / w;
    else sheet_to_add = ((l - now) + (w - 1)) / w;
    ans += sheet_to_add;
    now += sheet_to_add * w;
  }

  cout << ans << endl;

  return 0;
}
