#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<int> a(t), b(t);
  for (int i = 0; i < t; i++) {
    cin >> a[i] >> b[i];
  }

  vector<int> ans(t);
  for (int i = 0; i < t; i++) {
    ans[i] = (a[i] - (b[i] % a[i])) % a[i];
    for (int av = a[i]; av < (int) sqrt(b[i]) + 1; av++) {
      ans[i] = min(ans[i], ((av - (b[i] % av)) % av) + (av - a[i]));
    }
    for (int d = min((int) sqrt(b[i]) + 1, b[i] / a[i]); d > 0; d--) {
      int av = (b[i] + d - 1) / d;
      if (av < a[i]) continue;
      ans[i] = min(ans[i], ((av - (b[i] % av)) % av) + (av - a[i]));
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
