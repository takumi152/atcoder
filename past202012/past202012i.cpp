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

  int n, m, k;
  cin >> n >> m >> k;
  vector<int> h(n);
  for (auto &x: h) cin >> x;
  vector<int> c(k);
  for (auto &x: c) cin >> x;
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    if (h[a-1] < h[b-1]) edge[a-1].push_back(b-1);
    else edge[b-1].push_back(a-1);
  }

  vector<int> ans(n, -1);
  vector<bool> visited(n);
  queue<Pii> que;
  for (auto &x: c) que.emplace(x-1, 0);
  while (!que.empty()) {
    auto [v, d] = que.front();
    que.pop();
    if (visited[v]) continue;
    visited[v] = true;
    ans[v] = d;
    for (auto &next: edge[v]) {
      if (!visited[next]) que.emplace(next, d+1);
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
