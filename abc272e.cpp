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
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<vector<int>> values_at_iter(m+1);
  for (int i = 0; i < n; i++) {
    int iter_start = max(0, -a[i] / (i + 1));
    for (int j = iter_start; j <= m; j++) {
      if (a[i] + (i + 1) * j >= n) break;
      if (a[i] + (i + 1) * j < 0) continue;
      values_at_iter[j].push_back(a[i] + (i + 1) * j);
    }
  }

  for (auto &x: values_at_iter) {
    sort(x.begin(), x.end());
    auto it = unique(x.begin(), x.end());
    x.erase(it, x.end());
    sort(x.begin(), x.end());
  }
  
  vector<int> ans(m);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < values_at_iter[i+1].size(); j++) {
      if (values_at_iter[i+1][j] == j) ans[i]++;
      else break;
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
