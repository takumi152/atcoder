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

  ll h;
  cin >> h;

  ll now = h;
  ll count = 1;
  ll ans = 0;
  while (now > 0) {
    ans += count;
    count *= 2;
    now /= 2;
  }

  cout << ans << endl;

  return 0;
}
