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

  int n, m;
  cin >> n >> m;
  vector<int> r(m), b(m);
  for (auto &x: r) cin >> x;
  for (auto &x: b) cin >> x;

  vector<ll> r_sum(n), b_sum(n);
  for (int i = 0; i < m; i++) {
    r_sum[r[i]-1]++;
    b_sum[b[i]-1]++;
  }

  vector<ll> factorial(m+1);
  factorial[0] = 1;
  for (ll i = 1; i <= n; i++) {
    factorial[i] = (factorial[i-1] * i) % mod;
  }

  vector<ll> dp1(1 << n);
  for (int i = 1; i < (1 << n); i++) {
    ll rs = 0, bs = 0;
    for (int j = i; j > 0; j &= ~(1 << (31 - __builtin_clz((unsigned int) j)))) {
      int idx = (31 - __builtin_clz((unsigned int) j));
      rs += r_sum[idx];
      bs += b_sum[idx];
    }
    if (rs == bs) {
      
    }
  }


  return 0;
}
