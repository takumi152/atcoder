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

  double a, b, h, m;
  cin >> a >> b >> h >> m;

  pair<double, double> pa = pair<double, double>(cos((h * 60.0 + m) / 720.0 * 3.141592653589793238 * 2.0) * a, sin((h * 60.0 + m) / 720.0 * 3.141592653589793238 * 2.0) * a);
  pair<double, double> pb = pair<double, double>(cos(m / 60.0 * 3.141592653589793238 * 2.0) * b, sin(m / 60.0 * 3.141592653589793238 * 2.0) * b);

  double ans = sqrt((pa.first - pb.first) * (pa.first - pb.first) + (pa.second - pb.second) * (pa.second - pb.second));

  cout << setprecision(16) << ans << endl;

  return 0;
}
