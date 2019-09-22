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
  vector<double> v(N);
  for (auto &x: v) cin >> x;

  sort(v.begin(), v.end());

  double value = v[0];
  for (int i = 1; i < N; i++) {
    value = (value + v[i]) / 2.0;
  }

  cout << setprecision(16) << value << endl;

  return 0;
}
