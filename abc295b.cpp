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

  int r, c;
  cin >> r >> c;
  vector<vector<char>> board(r, vector<char>(c));
  for (auto &x: board) {
    for (auto &y: x) cin >> y;
  }

  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      if (!(board[i][j] == '.' || board[i][j] == '#')) {
        int power = board[i][j] - '0';
        for (int p = 0; p < r; p++) {
          for (int q = 0; q < c; q++) {
            if (board[p][q] == '#' && abs(i - p) + abs(j - q) <= power) {
              board[p][q] = '.';
            }
          }
        }
        board[i][j] = '.';
      }
    }
  }

  for (auto &x: board) {
    for (auto &y: x) cout << y;
    cout << endl;
  }

  return 0;
}
