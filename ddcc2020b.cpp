#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  ll length = 0;
  for (auto &x: a) length += x;
  ll partial = 0;
  ll ans = (ll)9e18;
  for (int i = 0; i < n - 1; i++) {
    partial += a[i];
    ans = min(ans, abs(length - partial * 2));
  }

  cout << ans << endl;

  return 0;
}
