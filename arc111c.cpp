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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n), b(n), p(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;
  for (auto &x: p) {
    cin >> x;
    x--;
  }

  for (int i = 0; i < n; i++) {
    if (b[p[i]] >= a[i] && p[i] != i) {
      cout << -1 << endl;
      return 0;
    }
  }

  vector<int> q(n);
  for (int i = 0; i < n; i++) {
    q[p[i]] = i;
  }

  vector<int> check_order(n);
  for (int i = 0; i < n; i++) check_order[i] = i;
  sort(check_order.begin(), check_order.end(), [&](auto x, auto y){return a[x] < a[y];});

  vector<Pii> ans;
  for (int i = 0; i < n; i++) {
    int u = check_order[i];
    if (p[u] == u) continue;
    int v = q[u];
    swap(q[p[u]], q[p[v]]);
    swap(p[u], p[v]);
    ans.emplace_back(u+1, v+1);
  }

  cout << ans.size() << endl;
  for (auto &x: ans) cout << x.first << " " << x.second << endl;

  return 0;
}
