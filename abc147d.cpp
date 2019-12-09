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

int popcount(int x) {
  int a = 0;
  while (x > 0) {
    if (x & 1) a++;
    x >>= 1;
  }
  return a;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  ll ans = 0;
  ll x = 1;
  for (int k = 0; k < 61; k++) {
    ll t = 0;
    ll f = 0;
    for (int i = 0; i < n; i++) {
      if (a[i] & 1) t++;
      else f++;
    }
    for (int i = 0; i < n; i++) {
      if (a[i] & 1) {
        t--;
        ans = (ans + (x * f)) % mod;
      }
      else {
        f--;
        ans = (ans + (x * t)) % mod;
      }
    }
    for (int i = 0; i < n; i++) {
      a[i] >>= 1;
    }
    x = (x << 1) % mod;
  }

  cout << ans << endl;

  return 0;
}
