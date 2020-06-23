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

  int x;
  cin >> x;

  int ans = 0;
  int deg = 0;
  while (true) {
    deg = (deg + x) % 360;
    ans++;
    if (deg == 0) break;
  }

  cout << ans << endl;

  return 0;
}
