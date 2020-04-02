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
  vector<vector<int> > c(10, vector<int>(10));
  for (auto &x: c) {
    for (auto &y: x) cin >> y;
  }
  vector<vector<int> > a(h, vector<int>(w));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }

  vector<int> c1(10);
  for (int i = 0; i <= 9; i++) {
    priority_queue<Pii, vector<Pii>, greater<Pii> > pque;
    vector<bool> visited(10);
    pque.emplace(0, i);
    while (!pque.empty()) {
      auto now = pque.top();
      pque.pop();
      if (visited[now.second]) continue;
      visited[now.second] = true;
      if (now.second == 1) {
        c1[i] = now.first;
        break;
      }
      for (int j = 0; j <= 9; j++) {
        if (!visited[j]) pque.emplace(now.first + c[now.second][j], j);
      }
    }
  }

  int ans = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (a[i][j] == -1) continue;
      else ans += c1[a[i][j]];
    }
  }

  cout << ans << endl;

  return 0;
}
