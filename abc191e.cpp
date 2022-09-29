#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<Pii> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    edge[a-1].emplace_back(b-1, c);
  }

  vector<int> ans(n, -1);
  for (int i = 0; i < n; i++) {
    vector<bool> visited(n);
    priority_queue<Pii, vector<Pii>, greater<Pii> > que;
    que.emplace(0, i);
    while (!que.empty()) {
      auto [t, now] = que.top();
      que.pop();
      if (visited[now] && now == i) {
        ans[i] = t;
        break;
      }
      if (visited[now]) continue;
      visited[now] = true;
      for (auto &[next, cost]: edge[now]) {
        if (!visited[next] || next == i) que.emplace(t+cost, next);
      }
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
