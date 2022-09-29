#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  vector<int> a(n);
  for (auto &x: a) cin >> x;
  vector<int> b(q);
  for (auto &x: b) cin >> x;

  sort(a.begin(), a.end());
  vector<int> ans(q);
  for (int i = 0; i < q; i++) {
    auto it = lower_bound(a.begin(), a.end(), b[i]);
    ans[i] = distance(it, a.end());
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
