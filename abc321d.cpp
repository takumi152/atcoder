#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m, p;
  cin >> n >> m >> p;
  vector<ll> a(n), b(m);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  sort(a.rbegin(), a.rend());

  vector<ll> price_deduction(100000001);
  {
    int pt = 0;
    ll inc = 0;
    while (pt < n) {
      if (a[pt] >= p) {
        price_deduction[0] += a[pt] - p;
        inc++;
        pt++;
      }
      else break;
    }
    for (int i = 1; i <= 100000000; i++) {
      price_deduction[i] = price_deduction[i-1] + inc;
      while (pt < n) {
        if (a[pt] >= p - i) {
          inc++;
          pt++;
        }
        else break;
      }
    }
  }

  ll ans = 0;
  {
    ll a_sum = 0;
    for (int i = 0; i < n; i++) {
      a_sum += a[i];
    }
    ans += a_sum * m;
    ll b_sum = 0;
    for (int i = 0; i < m; i++) {
      b_sum += b[i];
    }
    ans += b_sum * n;
    for (int i = 0; i < m; i++) {
      ans -= price_deduction[b[i]];
    }
  }

  cout << ans << endl;

  return 0;
}
