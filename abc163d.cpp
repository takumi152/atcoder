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

  ll n, k;
  cin >> n >> k;

  ll minimum = 0;
  ll maximum = 0;
  ll ans = 0;
  for (int i = 0; i <= n; i++) {
    minimum += i;
    maximum += n-i;
    if (i+1 >= k) ans = (ans + (maximum - minimum + 1)) % mod;
  }

  cout << ans << endl;

  return 0;
}
