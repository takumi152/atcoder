#include <iostream>
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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<string> s(h);
  for (auto &x: s) cin >> x;

  vector<vector<bool> > visited(h, vector<bool>(w));
  vector<vector<int> > flip(h, vector<int>(w));
  priority_queue<vector<int>, vector<vector<int> >, greater<vector<int> > > que;
  if (s[0][0] == '.') que.emplace(vector<int>({0, 0, 0}));
  else que.emplace(vector<int>({1, 0, 0}));
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    if (visited[now[1]][now[2]]) continue;
    visited[now[1]][now[2]] = true;
    flip[now[1]][now[2]] = now[0];
    if (now[1] < h-1) {
      if (!visited[now[1]+1][now[2]]) {
        if (s[now[1]][now[2]] == '.' && s[now[1]+1][now[2]] == '#') que.emplace(vector<int>({now[0]+1, now[1]+1, now[2]}));
        else que.emplace(vector<int>({now[0], now[1]+1, now[2]}));
      }
    }
    if (now[2] < w-1) {
      if (!visited[now[1]][now[2]+1]) {
        if (s[now[1]][now[2]] == '.' && s[now[1]][now[2]+1] == '#') que.emplace(vector<int>({now[0]+1, now[1], now[2]+1}));
        else que.emplace(vector<int>({now[0], now[1], now[2]+1}));
      }
    }
  }

  cout << flip[h-1][w-1] << endl;

  return 0;
}
