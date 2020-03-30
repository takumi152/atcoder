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

  int n, x, y;
  cin >> n >> x >> y;

  x--;
  y--;

  vector<int> ans(n-1);
  for (int i = 0; i < n-1; i++) {
    vector<int> dist(n);
    vector<bool> visited(n);
    queue<Pii> que;
    que.emplace(i, 0);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (visited[now.first]) continue;
      visited[now.first] = true;
      dist[now.first] = now.second;
      if (now.first > 0) {
        if (!visited[now.first-1]) que.emplace(now.first-1, now.second+1);
      }
      if (now.first < n-1) {
        if (!visited[now.first+1]) que.emplace(now.first+1, now.second+1);
      }
      if (now.first == x) {
        if (!visited[y]) que.emplace(y, now.second+1);
      }
      if (now.first == y) {
        if (!visited[x]) que.emplace(x, now.second+1);
      }
    }
    for (int j = i; j < n; j++) {
      if (dist[j] > 0) ans[dist[j]-1]++;
    }
  }

  for (auto &z: ans) cout << z << endl;

  return 0;
}
