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

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n), b(n);
  for (int i = 0; i < n; i++) cin >> a[i] >> b[i];

  vector<double> burn_time(n+1);
  for (int i = 0; i < n; i++) burn_time[i+1] = burn_time[i] + (double) a[i] / (double) b[i];

  double collision_time = burn_time[n] / 2;
  double ans = 0.0;
  for (int i = 0; i < n; i++) {
    if (burn_time[i+1] < collision_time) ans += a[i];
    else {
      ans += a[i] * (1.0 - (burn_time[i+1] - collision_time) / (burn_time[i+1] - burn_time[i]));
      break;
    }
  }

  cout << setprecision(20) << ans << endl;

  return 0;
}
