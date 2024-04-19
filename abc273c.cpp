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

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<int> uniq_a = a;
  sort(uniq_a.begin(), uniq_a.end());
  auto it = unique(uniq_a.begin(), uniq_a.end());
  uniq_a.erase(it, uniq_a.end());

  vector<int> ans(n);
  for (int i = 0; i < n; i++) {
    ans[distance(upper_bound(uniq_a.begin(), uniq_a.end(), a[i]), uniq_a.end())]++;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
