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

  Pii start = Pii(h-1, 0);
  for (int i = 0; i < w; i++) {
    if (s[h-1][i] == 's') {
      start = Pii(h-1, i);
      break;
    }
  }

  vector<vector<bool> > visited(h, vector<bool>(w));
  queue<pair<Pii, string> > que;
  que.emplace(start, "");
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    Pii pos = now.first;
    string action = now.second;
    if (visited[pos.first][pos.second]) continue;
    visited[pos.first][pos.second] = true;
    if (pos.first == 0) {
      cout << action << endl;
      return 0;
    }
    for (int i = max(pos.second-1, 0); i <= min(pos.second+1, w-1); i++) {
      if (!visited[pos.first-1][i] && s[pos.first-1][i] == '.') {
        string next = action;
        if (i == pos.second-1) next += "L";
        if (i == pos.second) next += "S";
        if (i == pos.second+1) next += "R";
        que.emplace(Pii(pos.first-1, i), next);
      }
    }
  }

  cout << "impossible" << endl;

  return 0;
}
