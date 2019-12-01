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

  double a, b, x;
  cin >> a >> b >> x;

  constexpr double pi = 3.14159265358979323846264;

  double levelHeight = x / (a * a);
  double cornerAngle = atan(levelHeight / (a / 2.0));

  double eps = 1e-10;
  double left = 0.0;
  double right = pi / 2.0;
  while (right - left > eps) {
    double center = (left + right) / 2.0;
    double waterHeight;
    if (center < cornerAngle) waterHeight = levelHeight + (a / 2.0) * tan(center);
    else waterHeight = sqrt((2.0 * (x / a)) / tan(center)) * tan(center);
    if (waterHeight < b) left = center;
    else right = center;
  }

  double ans = ((left + right) / 2.0) * (180 / pi);
  
  cout << setprecision(16) << ans << endl;

  return 0;
}
