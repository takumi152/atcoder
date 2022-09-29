#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<double> a(n);
  for (auto &x: a) cin >> x;

  auto calc_loss = [&](double x) {
    double loss = 0.0;

    for (int i = 0; i < n; i++) {
      loss += x + a[i] - min(a[i], 2.0 * x);
    }

    loss /= (double) n;
    return loss;
  };

  double low = 0.0;
  double high = 1e10;
  for (int i = 0; i < 200; i++) {
    double center_low = (low * 2.0 + high) / 3.0;
    double center_high = (low + high * 2.0) / 3.0;

    double loss_center_low = calc_loss(center_low);
    double loss_center_high = calc_loss(center_high);

    if (loss_center_low < loss_center_high) high = center_high;
    else low = center_low;
  }

  double ans = calc_loss((low + high) / 2.0);

  cout << setprecision(20) << ans << endl;

  return 0;
}
