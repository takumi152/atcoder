#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  int ans = max(0, (n + 1) / 2 - 1);
  cout << ans << endl;

  return 0;
}
