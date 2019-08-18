#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<double> A(N);
  for (auto &x: A) cin >> x;

  double divisor = 0.0;

  for (auto &x: A) divisor += 1.0 / x;

  double ans = 1.0 / divisor;

  cout << setprecision(16) << ans << endl;

  return 0;
}
