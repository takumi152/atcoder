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

  int t;
  cin >> t;
  vector<ll> n(t), a(t), b(t);
  for (int i = 0; i < t; i++) cin >> n[i] >> a[i] >> b[i];

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    if (a[i] + b[i] > n[i]) continue;
    if (a[i] < b[i]) swap(a[i], b[i]);
    ans[i] = ((n[i] - b[i] + 1) * (n[i] - a[i] + 1)) % mod;
    ll k = (n[i] - a[i] - b[i] - b[i] + 2);
    ll l = ((min(b[i], n[i] - a[i] - b[i] + 2) - 1) * (min(b[i], n[i] - a[i] - b[i] + 2)) / 2) % mod;
    ans[i] = (ans[i] * ((max(0LL, k) * (n[i] - a[i] + 1) + l * 2) % mod)) % mod;
    ll m = ((((n[i] - b[i] + 1) * (n[i] - a[i] + 1)) % mod) - ((max(0LL, k) * (n[i] - a[i] + 1) + l * 2) % mod)) % mod;
    if (m < 0) m += mod;
    ans[i] = (ans[i] + m * ((max(0LL, k) * (n[i] - a[i] + 1) + l * 2) % mod)) % mod;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
