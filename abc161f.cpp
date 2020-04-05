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

bool f(ll n, ll k) {
  if (k == 1) return false;
  while (n > 1) {
    if (n % k == 0) n /= k;
    else if (n / k > 0) n %= k;
    else n -= k;
  }
  return n == 1;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n;
  cin >> n;

  ll ans = 0;
  bool nminus1 = false;
  for (ll i = 1; i * i <= n; i++) {
    if (f(n, i)) ans++;
    if (i != n/i && f(n, n/i)) ans++;
    if (i == n-1 || n/i == n-1) nminus1 = true;
  }
  if (!nminus1) ans++;

  cout << ans << endl;

  return 0;
}
