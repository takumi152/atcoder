#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  int ch, cw;
  cin >> ch >> cw;
  int dh, dw;
  cin >> dh >> dw;
  vector<string> board(h);
  for (auto &x: board) cin >> x;

  priority_queue<vector<int>, vector<vector<int> >, greater<vector<int> > > pque;
  pque.push(vector<int>({0, ch-1, cw-1}));
  vector<vector<bool> > visited(h, vector<bool>(w));
  while (!pque.empty()) {
    auto now = pque.top();
    pque.pop();
    if (visited[now[1]][now[2]]) continue;
    visited[now[1]][now[2]] = true;

    if (now[1] == dh-1 && now[2] == dw-1) {
      cout << now[0] << endl;
      return 0;
    }

    if (now[1] > 0) {
      if (board[now[1]-1][now[2]] == '.' && !visited[now[1]-1][now[2]]) pque.push(vector<int>({now[0], now[1]-1, now[2]}));
    }
    if (now[1] < h-1) {
      if (board[now[1]+1][now[2]] == '.' && !visited[now[1]+1][now[2]]) pque.push(vector<int>({now[0], now[1]+1, now[2]}));
    }
    if (now[2] > 0) {
      if (board[now[1]][now[2]-1] == '.' && !visited[now[1]][now[2]-1]) pque.push(vector<int>({now[0], now[1], now[2]-1}));
    }
    if (now[2] < w-1) {
      if (board[now[1]][now[2]+1] == '.' && !visited[now[1]][now[2]+1]) pque.push(vector<int>({now[0], now[1], now[2]+1}));
    }

    for (int k = -2; k <= 2; k++) {
      for (int l = -2; l <= 2; l++) {
        if (now[1]+k < 0 || now[1]+k >= h || now[2]+l < 0 || now[2]+l >= w || abs(k)+abs(l) <= 1) continue;
        if (board[now[1]+k][now[2]+l] == '.' && !visited[now[1]+k][now[2]+l]) pque.push(vector<int>({now[0]+1, now[1]+k, now[2]+l}));
      }
    }
  }

  cout << -1 << endl;

  return 0;
}
