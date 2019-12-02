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

  int n, a, b;
  cin >> n >> a >> b;
  vector<ll> h(n);
  for (auto &x: h) cin >> x;

  sort(h.rbegin(), h.rend());

  ll delta = a - b;

  ll left = 0;
  ll right = 1000000007;
  while (left < right) {
    ll center = (left + right) / 2;
    ll damage = b * center;
    ll hit = 0;
    for (int i = 0; i < n; i++) {
      ll remaining = h[i] - damage;
      if (remaining > 0) hit += (remaining + delta - 1) / delta;
    }
    if (hit <= center) right = center;
    else left = center + 1;
  }

  ll ans = left;

  cout << ans << endl;

  return 0;
}
