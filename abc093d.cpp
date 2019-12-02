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

ll isqrt(ll x) {
  ll left = 0;
  ll right = min((1LL << 31) - 1, x + 1);
  while (left + 1 < right) {
    ll center = (left + right) / 2;
    if (center * center <= x) left = center;
    else right = center;
  }
  return left;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int q;
  cin >> q;
  vector<ll> a(q), b(q);
  for (int i = 0; i < q; i++) cin >> a[i] >> b[i];

  vector<ll> ans(q);
  for (int i = 0; i < q; i++) {
    if (a[i] < b[i]) swap(a[i], b[i]); // WLOG a >= b
    ll t = isqrt(a[i] * b[i] - 1);
    ans[i] = t * 2 - 1;
    ll x = t, y = t;
    if (t * (t + 1) < a[i] * b[i]) {
      x++;
      ans[i]++;
    }
    if (a[i] == b[i]) ans[i]++; // corner
    while ((x + 1) * y >= a[i] * b[i]) {
      x++;
      y--;
    }
    ans[i]--;
    if (x > a[i]) ans[i]--;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
