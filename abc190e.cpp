#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }
  int k;
  cin >> k;
  vector<int> c(k);
  for (auto &x: c) {
    cin >> x;
    x--;
  }

  unordered_map<int, int> target_id;
  for (int i = 0; i < k; i++) target_id[c[i]] = i;

  vector<vector<int> > dist(k, vector<int>(k, -1));
  for (int i = 0; i < k; i++) {
    vector<bool> visited(n);
    queue<Pii> que;
    que.emplace(c[i], 0);
    while (!que.empty()) {
      auto [now, d] = que.front();
      que.pop();
      if (visited[now]) continue;
      visited[now] = true;
      if (target_id.find(now) != target_id.end()) dist[i][target_id[now]] = d;
      for (auto &next: edge[now]) {
        if (!visited[next]) que.emplace(next, d+1);
      }
    }
    for (int j = 0; j < k; j++) {
      if (dist[i][j] == -1) {
        cout << -1 << endl;
        return 0;
      }
    }
  }

  vector<vector<int> > dp(1 << k, vector<int>(k, 998244353));
  for (int i = 0; i < k; i++) dp[1<<i][i] = 1;
  for (int i = 0; i < (1 << k); i++) {
    for (int u = 0; u < k; u++) {
      for (int v = 0; v < k; v++) {
        dp[i|(1<<v)][v] = min(dp[i|(1<<v)][v], dp[i][u] + dist[u][v]);
      }
    }
  }

  int ans = 998244353;
  for (int i = 0; i < k; i++) ans = min(ans, dp[(1<<k)-1][i]);

  cout << ans << endl;

  return 0;
}
