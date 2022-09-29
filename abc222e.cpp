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

  int n, m, k;
  cin >> n >> m >> k;
  vector<int> a(m);
  for (auto &x: a) {
    cin >> x;
    x--;
  }
  vector<vector<Pii> > edge(n);
  for (int i = 0; i < n-1; i++) {
    int x, y;
    cin >> x >> y;
    edge[x-1].emplace_back(y-1, i);
    edge[y-1].emplace_back(x-1, i);
  }

  int total_pass = 0;
  vector<int> pass_count(n-1);
  for (int i = 0; i < m-1; i++) {
    vector<bool> visited(n);
    vector<Pii> prev(n);
    queue<int> que;
    que.push(a[i]);
    visited[a[i]] = true;
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (now == a[i+1]) break;
      for (auto &[next, edge_id]: edge[now]) {
        if (!visited[next]) {
          prev[next] = Pii(now, edge_id);
          visited[next] = true;
          que.push(next);
        }
      }
    }
    int p = a[i+1];
    while (p != a[i]) {
      pass_count[prev[p].second]++;
      total_pass++;
      p = prev[p].first;
    }
  }

  vector<ll> dp(200001);
  dp[100000 - total_pass] = 1;
  for (int i = 0; i < n-1; i++) {
    for (int j = 200000 - (pass_count[i] * 2); j >= 0; j--) dp[j + (pass_count[i] * 2)] = (dp[j + (pass_count[i] * 2)] + dp[j]) % mod;
  }

  cout << dp[100000 + k] << endl;

  return 0;
}
