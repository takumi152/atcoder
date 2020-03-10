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

ll gcd(ll a, ll b) {
  while (b > 0) {
    ll rem = a % b;
    a = b;
    b = rem;
  }
  return a;
}

ll lcm(ll a, ll b) {
  return a / gcd(a, b) * b;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<ll> t(n), a(n);
  for (int i = 0; i < n; i++) cin >> t[i] >> a[i];

  ll takahashi = t[0];
  ll aoki = a[0];
  for (int i = 1; i < n; i++) {
    ll tmul = (takahashi + t[i] - 1) / t[i];
    ll amul = (aoki + a[i] - 1) / a[i];
    ll maxmul = max(tmul, amul);
    takahashi = t[i] * maxmul;
    aoki = a[i] * maxmul;
  }

  ll ans = takahashi + aoki;

  cout << ans << endl;

  return 0;
}
