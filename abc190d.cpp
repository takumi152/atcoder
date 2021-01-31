#include <iostream>
#include <iomanip>
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

  ll n;
  cin >> n;

  int ans = 0;
  ll x = n;
  ll p = 1;
  while (x > 0) {
    if (x % p == 0) ans += 2;
    x -= p;
    p++;
  }

  cout << ans << endl;

  return 0;
}
