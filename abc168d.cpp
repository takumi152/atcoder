#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

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

  vector<int> ans(n);
  vector<bool> visited(n);
  queue<Pii> que;
  que.emplace(0, 0);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now.first]) continue;
    visited[now.first] = true;
    ans[now.first] = now.second + 1;
    for (auto &next: edge[now.first]) {
      if (!visited[next]) que.emplace(next, now.first);
    }
  }

  cout << "Yes" << endl;
  for (int i = 1; i < n; i++) cout << ans[i] << endl;

  return 0;
}
