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

ll floor_sum(ll n, ll m, ll a, ll b) {
  ll ans = 0;

  if (a >= m) {
      ans += (n - 1) * n * (a / m) / 2;
      a %= m;
  }
  if (b >= m) {
      ans += n * (b / m);
      b %= m;
  }

  ll y = (a * n + b) / m;
  ll x = y * m - b;
  if (y == 0) return ans;

  ans += (n - (x + a - 1) / a) * y;
  ans += floor_sum(y, a, m, (a - x % a) % a);
  return ans;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<ll> n(t), m(t), a(t), b(t);
  for (int i = 0; i < t; i++) cin >> n[i] >> m[i] >> a[i] >> b[i];

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    ans[i] = floor_sum(n[i], m[i], a[i], b[i]);
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
