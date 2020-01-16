#include <iostream>
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

  int n;
  ll x;
  cin >> n >> x;
  vector<ll> w(n);
  for (auto &y: w) cin >> y;

  if (n == 1) {
    if (w[0] == x) cout << 1 << endl;
    else cout << 0 << endl;
    return 0;
  }

  vector<ll> a(n/2), b(n-n/2);
  for (int i = 0; i < n/2; i++) a[i] = w[i];
  for (int i = n/2; i < n; i++) b[i-n/2] = w[i];
  unordered_map<ll, int> ac, bc;
  for (int i = 0; i < (1 << (n/2)); i++) {
    ll total = 0;
    int m = i;
    int p = 0;
    while (m > 0) {
      if (m & 1) total += a[p];
      m >>= 1;
      p++;
    }
    ac[total]++;
  }
  for (int i = 0; i < (1 << (n-n/2)); i++) {
    ll total = 0;
    int m = i;
    int p = 0;
    while (m > 0) {
      if (m & 1) total += b[p];
      m >>= 1;
      p++;
    }
    bc[total]++;
  }

  ll ans = 0;
  for (auto &aw: ac) ans += aw.second * bc[x - aw.first];

  cout << ans << endl;

  return 0;
}
