#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<int> a(m);
  for (auto &x: a) cin >> x;

  vector<int> ans(n);
  int p = 0;
  for (int i = 0; i < n; i++) {
    while (p < m) {
      if (a[p] < i + 1) {
        p++;
      }
      else break;
    }
    ans[i] = a[p] - (i + 1);
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
