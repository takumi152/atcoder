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

  ll n, t;
  cin >> n >> t;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  if (n == 1) {
    if (a[0] <= t) cout << a[0] << endl;
    else cout << 0 << endl;
    return 0;
  }

  vector<ll> a1, a2;
  for (int i = 0; i < n / 2; i++) a1.push_back(a[i]);
  for (int i = n / 2; i < n; i++) a2.push_back(a[i]);

  vector<ll> halfenum1, halfenum2;
  for (int i = 0; i < (1 << (int) a1.size()); i++) {
    ll total = 0;
    for (int j = 0; j < (int) a1.size(); j++) {
      if (i & (1 << j)) total += a1[j];
    }
    halfenum1.push_back(total);
  }
  for (int i = 0; i < (1 << (int) a2.size()); i++) {
    ll total = 0;
    for (int j = 0; j < (int) a2.size(); j++) {
      if (i & (1 << j)) total += a2[j];
    }
    halfenum2.push_back(total);
  }

  sort(halfenum1.begin(), halfenum1.end());
  sort(halfenum2.begin(), halfenum2.end());

  ll ans = 0;
  int p2 = halfenum2.size()-1;
  for (int i = 0; i < (int) halfenum1.size(); i++) {
    while (p2 > 0) {
      if (halfenum1[i] + halfenum2[p2] <= t) break;
      p2--;
    }
    if (halfenum1[i] + halfenum2[p2] <= t && halfenum1[i] + halfenum2[p2] > ans) ans = halfenum1[i] + halfenum2[p2];
  }

  cout << ans << endl;

  return 0;
}
