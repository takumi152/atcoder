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
  return a * b / gcd(a, b);
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m;
  cin >> n >> m;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  for (auto &x: a) x /= 2;
  ll l = a[0];
  for (int i = 1; i < n; i++) {
    l = lcm(l, a[i]);
    if (l > m) {
      cout << 0 << endl;
      return 0;
    }
  }

  for (int i = 0; i < n; i++) {
    if ((l / a[i]) % 2 == 0) {
      cout << 0 << endl;
      return 0;
    }
  }

  ll ans = ((m / l) + 1) / 2;

  cout << ans << endl;


  return 0;
}
