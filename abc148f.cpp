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

  int n, u, v;
  cin >> n >> u >> v;
  u--;
  v--;
  vector<vector<int> > edge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }

  vector<int> distU(n), distV(n);

  queue<Pii> que;
  vector<bool> visited(n);
  que.emplace(u, 0);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now.first]) continue;
    visited[now.first] = true;
    distU[now.first] = now.second;
    for (auto &x: edge[now.first]) {
      if (visited[x]) continue;
      que.emplace(x, now.second + 1);
    }
  }

  visited = vector<bool>(n);
  que.emplace(v, 0);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now.first]) continue;
    visited[now.first] = true;
    distV[now.first] = now.second;
    for (auto &x: edge[now.first]) {
      if (visited[x]) continue;
      que.emplace(x, now.second + 1);
    }
  }

  int ans = 0;
  int uNow = u, uLast = u, vNow = v, vLast = v;
  while (uNow != vNow) {
    uLast = uNow;
    vLast = vNow;
    for (auto &x: edge[uNow]) {
      if (distV[x] < distV[uNow]) {
        uNow = x;
        break;
      }
    }
    if (uNow == vNow) break;
    for (auto &x: edge[vNow]) {
      if (distU[x] < distU[vNow]) {
        vNow = x;
        break;
      }
    }
    ans++;
  }

  int banVertex = vNow;
  uNow = uLast;
  vNow = vLast;

  int longest = 0;
  visited = vector<bool>(n);
  visited[banVertex] = true;
  que.emplace(uNow, 0);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now.first]) continue;
    visited[now.first] = true;
    if (now.second > longest) longest = now.second;
    for (auto &x: edge[now.first]) {
      if (visited[x]) continue;
      que.emplace(x, now.second + 1);
    }
  }

  ans += longest;

  cout << ans << endl;

  return 0;
}
