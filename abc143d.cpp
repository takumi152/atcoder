#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> l(n);
  for (auto &x: l) cin >> x;

  sort(l.begin(), l.end());

  int ans = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      int ab = l[i] + l[j];
      int ba = l[j] - l[i];
      // c < a + b
      auto up = lower_bound(l.begin() + j, l.end(), ab);
      int d1 = distance(l.begin() + j, up);
      // a - b > c
      // b - a > c
      auto down = lower_bound(l.begin() + j, l.end(), ba);
      int d2 = distance(l.begin() + j, down);
      ans += max(0, d1 - d2 - 1);
    }
  }

  cout << ans << endl;

  return 0;
}
