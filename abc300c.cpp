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

const ll mod = 998244353;

inline bool within_board(int h, int w, int x, int y) {
  return 0 <= x && x < h && 0 <= y && y < w;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<vector<char>> board(h, vector<char>(w));
  for (auto &x: board) {
    for (auto &y: x) cin >> y;
  }

  vector<int> ans(min(h, w));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      const vector<int> dx = { 1,  1, -1, -1};
      const vector<int> dy = { 1, -1,  1, -1};
      bool is_cross = true;
      for (int d = 0; d < 4; d++) {
        if (!(within_board(h, w, i + dx[d], j + dy[d]) && board[i + dx[d]][j + dy[d]] == '#')) {
          is_cross = false;
        }
      }
      if (is_cross) {
        int sz = 9999999;
        for (int d = 0; d < 4; d++) {
          int px = i;
          int py = j;
          while (within_board(h, w, px, py) && board[px][py] == '#') {
            px += dx[d];
            py += dy[d];
          }
          sz = min(sz, abs(px - i) - 1);
        }
        if (sz >= 1) ans[sz - 1]++;
      }
    }
  }

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
