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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<ll> dp(n+2);
  dp[0] = 1;
  ll mul = 1;
  int limit = 1;
  for (int i = 1; i < n; i++) {
    mul++;
    dp[i] = (dp[i-1] * mul) % mod;

    limit += 2;
    if (a[i] < limit) {
      limit -= 2;
      mul--;
    }
  }

  ll ans = dp[n-1];

  cout << ans << endl;

  return 0;
}
