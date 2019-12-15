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
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<int> lc(n);
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < j; i++) {
      if (a[i] < a[j]) lc[j]++;
    }
  }

  ll ans = 0;
  for (int k = 0; k < n; k++) {
    for (int j = 0; j < k; j++) {
      if (a[j] > a[k]) ans += lc[j];
    }
  }

  cout << ans << endl;

  return 0;
}
