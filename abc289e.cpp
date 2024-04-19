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

  int t;
  cin >> t;
  vector<int> n(t);
  vector<int> m(t);
  vector<vector<int>> c(t);
  vector<vector<vector<int>>> graph(t);
  for (int i = 0; i < t; i++) {
    cin >> n[i] >> m[i];
    c[i] = vector<int>(n[i]);
    for (auto &x: c[i]) cin >> x;
    graph[i] = vector<vector<int>>(n[i]);
    for (int j = 0; j < m[i]; j++) {
      int u, v;
      cin >> u >> v;
      graph[i][u-1].push_back(v-1);
      graph[i][v-1].push_back(u-1);
    }
  }

  vector<int> ans(t);
  for (int i = 0; i < t; i++) {
    vector<vector<int>> min_steps(n[i], vector<int>(n[i], -1));
    min_steps[0][n[i]-1] = 0;
    queue<Pii> que;
    que.emplace(0, n[i]-1);
    while (!que.empty()) {
      auto [a, b] = que.front();
      que.pop();
      for (auto &x: graph[i][a]) {
        for (auto &y: graph[i][b]) {
          if (c[i][x] != c[i][y] && min_steps[x][y] == -1) {
            min_steps[x][y] = min_steps[a][b] + 1;
            que.emplace(x, y);
          } 
        }
      }
    }
    ans[i] = min_steps[n[i]-1][0];
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
