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

inline bool within_board(int x, int y, int h, int w) {
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

  const vector<int> dx = { 1,  1,  0, -1, -1, -1,  0,  1};
  const vector<int> dy = { 0,  1,  1,  1,  0, -1, -1, -1};
  const string target = "snuke";
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      for (int d = 0; d < 8; d++) {
        bool good = true;
        int px = i;
        int py = j;
        for (int k = 0; k < 5; k++) {
          if (!(within_board(px, py, h, w) && board[px][py] == target[k])) {
            good = false;
            break;
          }
          px += dx[d];
          py += dy[d];
        }
        if (good) {
          px = i;
          py = j;
          for (int k = 0; k < 5; k++) {
            cout << px + 1 << " " << py + 1 << endl;
            px += dx[d];
            py += dy[d];
          }
          return 0;
        }
      }
    }
  }

  return 0;
}
