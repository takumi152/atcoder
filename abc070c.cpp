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
  vector<ll> t(n);
  for (auto &x: t) cin >> x;

  ll ans = 1;
  for (auto &x: t) ans = lcm(ans, x);

  cout << ans << endl;

  return 0;
}
