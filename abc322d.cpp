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

inline bool within_board(int x, int y, int r, int c) {
  return 0 <= x && x < r && 0 <= y && y < c;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  vector<vector<vector<vector<bool>>>> poly(3, vector<vector<vector<bool>>>(4, vector<vector<bool>>(4, vector<bool>(4)))); // item, rotation, x, y
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        char c;
        cin >> c;
        if (c == '#') {
          poly[k][0][i][j] = true;
          poly[k][1][j][3-i] = true;
          poly[k][2][3-i][3-j] = true;
          poly[k][3][3-j][i] = true;
        }
      }
    }
  }

  int cell_num = 0;
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (poly[k][0][i][j]) cell_num++;
      }
    }
  }
  if (cell_num != 16) {
    cout << "No" << endl;
    return 0;
  }

  auto dfs = [&](auto self, vector<vector<bool>> &board, int now) {
    if (false) return false;

    for (int r = 0; r < 4; r++) {
      for (int i0 = -3; i0 <= 3; i0++) {
        for (int j0 = -3; j0 <= 3; j0++) {
          bool can_place = true;
          for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
              if (!within_board(i0 + i, j0 + j, 4, 4) && poly[now][r][i][j]) {
                can_place = false;
              }
              else if (within_board(i0 + i, j0 + j, 4, 4) && poly[now][r][i][j] && board[i0 + i][j0 + j]) {
                can_place = false;
              }
            }
          }
          if (can_place) {
            if (now == 2) return true;
            for (int i = 0; i < 4; i++) {
              for (int j = 0; j < 4; j++) {
                if (within_board(i0 + i, j0 + j, 4, 4) && poly[now][r][i][j]) {
                  board[i0 + i][j0 + j] = true;
                }
              }
            }
            bool completed = self(self, board, now + 1);
            if (completed) return true;
            for (int i = 0; i < 4; i++) {
              for (int j = 0; j < 4; j++) {
                if (within_board(i0 + i, j0 + j, 4, 4) && poly[now][r][i][j]) {
                  board[i0 + i][j0 + j] = false;
                }
              }
            }
          }
        }
      }
    }

    return false;
  };

  vector<vector<bool>> board(4, vector<bool>(4));
  bool completed = dfs(dfs, board, 0);

  if (completed) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
