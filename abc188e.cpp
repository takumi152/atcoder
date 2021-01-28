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

vector<int> topological_sort(vector<vector<int> > &edge) {
  int n = edge.size();

  vector<int> in_degree(n);
  for (int i = 0; i < n; i++) {
    for (auto &x: edge[i]) in_degree[x]++;
  }

  vector<int> visit_order;
  queue<int> que;
  for (int i = 0; i < n; i++) {
    if (in_degree[i] == 0) que.push(i);
  }
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    visit_order.push_back(now);
    for (auto &x: edge[now]) {
      in_degree[x]--;
      if (in_degree[x] == 0) que.push(x);
    }
  }

  return visit_order;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<int> a(n);
  for (auto &x: a) cin >> x;
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
  }

  vector<ll> dp(n, 2000000007);
  auto visit_order = topological_sort(edge);
  for (auto &x: visit_order) {
    for (auto &y: edge[x]) dp[y] = min(dp[y], min(dp[x], (ll) a[x]));
  }

  ll ans = -2000000007;
  for (int i = 0; i < n; i++) {
    ans = max(ans, (ll) a[i] - dp[i]);
  }

  cout << ans << endl;

  return 0;
}
