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
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
  }

  int ans = 0;
  for (int i = 0; i < n; i++) {
    vector<bool> visited(n);
    queue<int> que;
    que.push(i);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (visited[now]) continue;
      visited[now] = true;
      for (auto &next: edge[now]) {
        if (!visited[next]) que.push(next);
      }
    }

    for (int j = 0; j < n; j++) {
      if (visited[j]) ans++;
    }
  }

  cout << ans << endl;

  return 0;
}
