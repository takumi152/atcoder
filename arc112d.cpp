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

int solve(int h, int w, vector<string> &board) {
  vector<int> dx = {-1, 1, 0, 0};
  vector<int> dy = {0, 0, -1, 1};

  vector<vector<bool> > visited(h, vector<bool>(w));

  vector<bool> row_accessible(h);
  row_accessible[0] = true;
  row_accessible[h-1] = true;

  auto bfs1 = [&](int ix, int iy) {
    queue<Pii> que;
    que.emplace(ix, iy);
    while (!que.empty()) {
      auto [x, y] = que.front();
      que.pop();
      if (visited[x][y]) continue;
      visited[x][y] = true;
      row_accessible[x] = true;
      for (int d = 0; d < 4; d++) {
        for (int i = x + dx[d], j = y + dy[d]; i >= 0 && i < h && j >= 0 && j < w; i += dx[d], j += dy[d]) {
          if (board[i][j] == '#') {
            if (!visited[i][j]) que.emplace(i, j);
            break;
          }
        }
      }
    }
  };

  for (int i = 0; i < h; i++) {
    if (board[i][0] == '#') bfs1(i, 0);
    if (board[i][w-1] == '#') bfs1(i, w-1);
  }
  for (int j = 0; j < w; j++) {
    if (board[0][j] == '#') bfs1(0, j);
    if (board[h-1][j] == '#') bfs1(h-1, j);
  }

  int ans = 0;
  for (int i = 1; i < h-1; i++) {
    if (!row_accessible[i]) {
      ans++;
      bfs1(i, 0);
    }
  }

  return ans;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<string> board(h);
  for (auto &x: board) cin >> x;

  int ans1 = solve(h, w, board);

  vector<string> board2(w);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      board2[j].push_back(board[i][j]);
    }
  }

  int ans2 = solve(w, h, board2);

  cout << min(ans1, ans2) << endl;

  return 0;
}
