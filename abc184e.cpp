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
  vector<string> a(h);
  for (auto &x: a) cin >> x;

  unordered_map<char, vector<Pii> > teleporterLocation;
  Pii start;
  Pii goal;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if ('a' <= a[i][j] && a[i][j] <= 'z') teleporterLocation[a[i][j]].emplace_back(i, j);
      else if (a[i][j] == 'S') start = Pii(i, j);
      else if (a[i][j] == 'G') goal = Pii(i, j);
    }
  }

  vector<vector<bool> > visited(h, vector<bool>(w));
  unordered_map<char, bool> teleporterVisited;
  queue<vector<int> > que;
  que.push(vector<int>({start.first, start.second, 0}));
  int ans = -1;
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now[0]][now[1]]) continue;
    visited[now[0]][now[1]] = true;
    if (now[0] == goal.first && now[1] == goal.second) {
      ans = now[2];
      break;
    }
    if ('a' <= a[now[0]][now[1]] && a[now[0]][now[1]] <= 'z') {
      if (!teleporterVisited[a[now[0]][now[1]]]) {
        teleporterVisited[a[now[0]][now[1]]] = true;
        for (auto &x: teleporterLocation[a[now[0]][now[1]]]) {
          if (!visited[x.first][x.second]) que.push(vector<int>({x.first, x.second, now[2]+1}));
        }
      }
    }
    if (now[0] > 0) {
      if (a[now[0]-1][now[1]] != '#' && !visited[now[0]-1][now[1]]) que.push(vector<int>({now[0]-1, now[1], now[2]+1}));
    }
    if (now[0] < h-1) {
      if (a[now[0]+1][now[1]] != '#' && !visited[now[0]+1][now[1]]) que.push(vector<int>({now[0]+1, now[1], now[2]+1}));
    }
    if (now[1] > 0) {
      if (a[now[0]][now[1]-1] != '#' && !visited[now[0]][now[1]-1]) que.push(vector<int>({now[0], now[1]-1, now[2]+1}));
    }
    if (now[1] < w-1) {
      if (a[now[0]][now[1]+1] != '#' && !visited[now[0]][now[1]+1]) que.push(vector<int>({now[0], now[1]+1, now[2]+1}));
    }
  }

  cout << ans << endl;

  return 0;
}
