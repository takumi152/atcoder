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

  int h, w;
  cin >> h >> w;
  int r, c;
  cin >> r >> c;
  vector<string> s(h);
  for (auto &x: s) cin >> x;

  vector<vector<bool> > visited(h, vector<bool>(w));
  queue<Pii> que;
  que.emplace(r-1, c-1);
  while (!que.empty()) {
    auto [x, y] = que.front();
    que.pop();
    if (visited[x][y]) continue;
    visited[x][y] = true;
    if (x > 0) {
      if (!visited[x-1][y] && (s[x-1][y] == '.' || s[x-1][y] == 'v')) que.emplace(x-1, y);
    }
    if (x < h-1) {
      if (!visited[x+1][y] && (s[x+1][y] == '.' || s[x+1][y] == '^')) que.emplace(x+1, y);
    }
    if (y > 0) {
      if (!visited[x][y-1] && (s[x][y-1] == '.' || s[x][y-1] == '>')) que.emplace(x, y-1);
    }
    if (y < w-1) {
      if (!visited[x][y+1] && (s[x][y+1] == '.' || s[x][y+1] == '<')) que.emplace(x, y+1);
    }
  }

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (s[i][j] == '#') cout << '#';
      else if (visited[i][j]) cout << 'o';
      else cout << 'x';
    }
    cout << endl;
  }

  return 0;
}
