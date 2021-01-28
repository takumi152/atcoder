#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll a, b, c, k;
  cin >> a >> b >> c >> k;

  ll ans = 0;
  ll rem = k;
  if (rem < a) {
    ans += rem;
    rem = 0;
  }
  else {
    ans += a;
    rem -= a;
  }
  if (rem < b) {
    rem = 0;
  }
  else {
    rem -= b;
  }
  if (rem < c) {
    ans -= rem;
    rem = 0;
  }
  else {
    ans -= c;
    rem -= c;
  }

  cout << ans << endl;

  return 0;
}
