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

  vector<ll> p10(19);
  p10[0] = 1;
  for (int i = 1; i < 19; i++) p10[i] = p10[i-1] * 10;

  auto solve = [&](auto self, ll x, ll p) {
    if (p < 0) return 0LL;
    else if (x < p10[p]) return 0LL;
    else if (x >= p10[p] * 2) return p10[p] + self(self, p10[p] - 1, p-1);
    else return (x - p10[p] + 1) + self(self, x % p10[p], p-1);
  };

  ll ans = 0;
  for (int i = 0; i < 18; i++) {
    if (n >= p10[i+1]) ans += solve(solve, p10[i+1] - 1, i);
    else ans += solve(solve, n, i);
  }

  cout << ans << endl;

  return 0;
}
