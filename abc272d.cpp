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

  int n, m;
  cin >> n >> m;

  vector<Pii> available_movement;
  for (int i = 0; i * i <= m; i++) {
    for (int j = 0; i * i + j * j <= m; j++) {
      if (i * i + j * j == m) {
        available_movement.emplace_back(i, j);
      }
    }
  }

  vector<vector<int>> dist(n, vector<int>(n, -1));
  queue<pair<Pii, int>> que;
  que.emplace(Pii(0, 0), 0);
  while (!que.empty()) {
    auto [pos, d] = que.front();
    auto [px, py] = pos;
    que.pop();
    if (dist[px][py] != -1) continue;
    dist[px][py] = d;
    for (auto &movement: available_movement) {
      if (within_board(px + movement.first, py + movement.second, n, n) && dist[px + movement.first][py + movement.second] == -1) {
        que.emplace(Pii(px + movement.first, py + movement.second), d + 1);
      }
      if (within_board(px + movement.first, py - movement.second, n, n) && dist[px + movement.first][py - movement.second] == -1) {
        que.emplace(Pii(px + movement.first, py - movement.second), d + 1);
      }
      if (within_board(px - movement.first, py + movement.second, n, n) && dist[px - movement.first][py + movement.second] == -1) {
        que.emplace(Pii(px - movement.first, py + movement.second), d + 1);
      }
      if (within_board(px - movement.first, py - movement.second, n, n) && dist[px - movement.first][py - movement.second] == -1) {
        que.emplace(Pii(px - movement.first, py - movement.second), d + 1);
      }
    }
  }

  for (auto &x: dist) {
    for (auto &y: x) cout << y << " ";
    cout << endl;
  }

  return 0;
}
