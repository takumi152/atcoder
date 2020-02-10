#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  vector<int> p(n);
  for (auto &x: p) cin >> x;

  vector<double> p_sum(n+1);
  for (int i = 0; i < n; i++) p_sum[i+1] = p_sum[i] + (double) (p[i] - 1) / 2.0 + 1.0;

  double maximum = 0;
  for (int i = 0; i < n - k + 1; i++) {
    if (p_sum[i+k] - p_sum[i] > maximum) maximum = p_sum[i+k] - p_sum[i];
  }

  cout << setprecision(16) << maximum << endl;

  return 0;
}
