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

  ll n, k;
  cin >> n >> k;
  vector<ll> a(n), f(n);
  for (auto &x: a) cin >> x;
  for (auto &x: f) cin >> x;

  sort(a.begin(), a.end());
  sort(f.rbegin(), f.rend());

  ll ok = (ll) 1e18;
  ll ng = -1;
  while (abs(ok - ng) > 1) {
    ll target = (ok + ng) / 2;
    ll needed = 0;
    for (int i = 0; i < n; i++) needed += max(0LL, (a[i] * f[i] - target + f[i] - 1) / f[i]);
    if (needed <= k) ok = target;
    else ng = target;
  }

  cout << ok << endl;

  return 0;
}
