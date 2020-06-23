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

  int n, m;
  cin >> n >> m;
  vector<int> a(m);
  for (auto &x: a) cin >> x;

  vector<int> tolerance(n);
  vector<int> ans(m);
  for (int i = 0; i < m; i++) {
    ans[i] = distance(tolerance.begin(), upper_bound(tolerance.begin(), tolerance.end(), -a[i]));
    if (ans[i] == n) ans[i] = -1;
    else {
      tolerance[ans[i]] = -a[i];
      ans[i]++;
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
