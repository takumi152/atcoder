#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n;
  cin >> n;

  // 1 1
  // 2 3
  // 3 5
  // 4 7
  // 5 9
  //
  // 2 4
  // 3 8
  // 4 12
  // 5 16
  // 6 20
  //
  // 3 9
  // 4 15
  // 5 21
  // 6 27
  // 7 33
  //
  // 4 16
  // 5 24
  // 6 32
  // 7 40
  // 8 48

  ll ans = 0;
  ll x = 1;
  while (x * x <= n) {
    ll base = x * x;
    ll increment = x * 2;
    ans = (ans + (n - base) / increment + 1) % mod;
    x++;
  }

  cout << ans << endl;

  return 0;
}
