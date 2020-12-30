#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  vector<int> spare;
  for (int i = 0; i < n; i++) {
    spare.push_back(-(a[i] - b[i]) + 1);
  }

  reverse(spare.begin(), spare.end());
  reverse(a.begin(), a.end());

  int ans = n;
  int lim = 1000000007;
  int undestroyed = 0;
  for (int i = 0; i < (int) spare.size(); i++) {
    if (spare[i] > 0) {
      int d = max(undestroyed, min((int) spare.size(), spare[i]));
      if (d < ans) ans = d;
      if (a[i] < lim) undestroyed++;
    }
    else {
      lim = min(lim, -spare[i] + 1);
    }
  }
  ans = min(ans, undestroyed);

  cout << ans << endl;

  return 0;
}
