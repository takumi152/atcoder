#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int x, y;
  cin >> x >> y;

  int ans = 0;
  if (x == 3) ans += 100000;
  else if (x == 2) ans += 200000;
  else if (x == 1) ans += 300000;
  if (y == 3) ans += 100000;
  else if (y == 2) ans += 200000;
  else if (y == 1) ans += 300000;
  if (x == 1 && y == 1) ans += 400000;

  cout << ans << endl;

  return 0;
}
