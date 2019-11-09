#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> d(n);
  for (auto &x: d) cin >> x;

  if (d[0] > 0) {
    cout << 0 << endl; // impossible
    return 0;
  }

  sort(d.begin(), d.end());

  if (d[1] == 0) {
    cout << 0 << endl; // impossible
    return 0;
  }

  ll ans = 1;
  ll lastCount = 1;
  ll count = 1;
  int lastDist = d[0];
  for (int i = 1; i < n; i++) {
    if (d[i] == lastDist) {
      ans = (ans * lastCount) % mod;
      count++;
    }
    else if (d[i] == lastDist + 1){
      lastCount = count;
      count = 1;
      lastDist = d[i];
      ans = (ans * lastCount) % mod;
    }
    else {
      cout << 0 << endl; // impossible
      return 0;
    }
  }

  cout << ans << endl;

  return 0;
}
