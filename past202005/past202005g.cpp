#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, x, y;
  cin >> n >> x >> y;
  vector<vector<bool> > board(2000, vector<bool>(2000, true));
  for (int i = 0; i < n; i++) {
    int ox, oy;
    cin >> ox >> oy;
    board[ox+1000][oy+1000] = false;
  }

  queue<vector<int> > que;
  vector<vector<bool> > visited(2000, vector<bool>(2000));
  que.push(vector<int>({0, 1000, 1000}));
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now[1]][now[2]]) continue;
    visited[now[1]][now[2]] = true;
    if (now[1] == x+1000 && now[2] == y+1000) {
      cout << now[0] << endl;
      return 0;
    }
    if (now[1] < 1999 && now[2] < 1999) {
      if (!visited[now[1]+1][now[2]+1] && board[now[1]+1][now[2]+1]) que.push(vector<int>({now[0]+1, now[1]+1, now[2]+1}));
    }
    if (now[2] < 1999) {
      if (!visited[now[1]][now[2]+1] && board[now[1]][now[2]+1]) que.push(vector<int>({now[0]+1, now[1], now[2]+1}));
    }
    if (now[1] > 0 && now[2] < 1999) {
      if (!visited[now[1]-1][now[2]+1] && board[now[1]-1][now[2]+1]) que.push(vector<int>({now[0]+1, now[1]-1, now[2]+1}));
    }
    if (now[1] < 1999) {
      if (!visited[now[1]+1][now[2]] && board[now[1]+1][now[2]]) que.push(vector<int>({now[0]+1, now[1]+1, now[2]}));
    }
    if (now[1] > 0) {
      if (!visited[now[1]-1][now[2]] && board[now[1]-1][now[2]]) que.push(vector<int>({now[0]+1, now[1]-1, now[2]}));
    }
    if (now[2] > 0) {
      if (!visited[now[1]][now[2]-1] && board[now[1]][now[2]-1]) que.push(vector<int>({now[0]+1, now[1], now[2]-1}));
    }
  }

  cout << -1 << endl;

  return 0;
}
