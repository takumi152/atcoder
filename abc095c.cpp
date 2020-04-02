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

  int a, b, c, x, y;
  cin >> a >> b >> c >> x >> y;

  int ans = 1000000007;
  for (int i = 0; i <= max(x, y); i++) {
    int cost = (a * max(0, x - i)) + (b * max(0, y - i)) + (c * 2 * i);
    if (cost < ans) ans = cost;
  }

  cout << ans << endl;

  return 0;
}
