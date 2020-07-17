#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w, k;
  cin >> h >> w >> k;
  int x1, y1, x2, y2;
  cin >> x1 >> y1 >> x2 >> y2;
  x1--;
  y1--;
  x2--;
  y2--;
  vector<string> c(h);
  for (auto &x: c) cin >> x;

  vector<vector<int> > dist(h, vector<int>(w, 1000000007));
  vector<vector<bool> > visited(h, vector<bool>(w));
  dist[x1][y1] = 0;
  queue<Pii> que;
  que.emplace(x1, y1);
  while (!que.empty()) {
    auto [x, y] = que.front();
    que.pop();
    if (visited[x][y]) continue;
    visited[x][y] = true;
    // up
    for (int i = x - 1; i >= max(0, x - k); i--) {
      if (c[i][y] == '@') break;
      if (dist[i][y] < dist[x][y] + 1) break;
      if (dist[i][y] > dist[x][y] + 1 && !visited[i][y]) que.emplace(i, y);
      dist[i][y] = dist[x][y] + 1;
    }
    // down
    for (int i = x + 1; i <= min(h - 1, x + k); i++) {
      if (c[i][y] == '@') break;
      if (dist[i][y] < dist[x][y] + 1) break;
      if (dist[i][y] > dist[x][y] + 1 && !visited[i][y]) que.emplace(i, y);
      dist[i][y] = dist[x][y] + 1;
    }
    // left
    for (int j = y - 1; j >= max(0, y - k); j--) {
      if (c[x][j] == '@') break;
      if (dist[x][j] < dist[x][y] + 1) break;
      if (dist[x][j] > dist[x][y] + 1 && !visited[x][j]) que.emplace(x, j);
      dist[x][j] = dist[x][y] + 1;
    }
    // right
    for (int j = y + 1; j <= min(w - 1, y + k); j++) {
      if (c[x][j] == '@') break;
      if (dist[x][j] < dist[x][y] + 1) break;
      if (dist[x][j] > dist[x][y] + 1 && !visited[x][j]) que.emplace(x, j);
      dist[x][j] = dist[x][y] + 1;
    }
  }

  if (dist[x2][y2] != 1000000007) cout << dist[x2][y2] << endl;
  else cout << -1 << endl;

  return 0;
}
