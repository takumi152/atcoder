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

  ll n, m, d;
  cin >> n >> m >> d;

  ll expect = 0;
  if (d == 0) expect = n * (m - 1);
  else expect = (n - d) * 2 * (m - 1);

  double ans = (double) expect / (double) (n * n);

  cout << setprecision(16) << ans << endl;

  return 0;
}
