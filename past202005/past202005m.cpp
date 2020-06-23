#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }
  int s;
  cin >> s;
  s--;
  int k;
  cin >> k;
  vector<int> t(k);
  for (auto &x: t) {
    cin >> x;
    x--;
  }

  unordered_map<int, unordered_map<int, int> > distanceFromTo;
  vector<bool> destination(n);
  for (int i = 0; i < k; i++) destination[t[i]] = true;
  vector<bool> visited(n);
  queue<Pii> que;
  que.emplace(s, 0);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now.first]) continue;
    visited[now.first] = true;
    if (destination[now.first]) distanceFromTo[s][now.first] = now.second;
    for (auto &x: edge[now.first]) {
      if (!visited[x]) que.emplace(x, now.second+1);
    }
  }
  for (int i = 0; i < k; i++) {
    vector<bool> visited(n);
    queue<Pii> que;
    que.emplace(t[i], 0);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (visited[now.first]) continue;
      visited[now.first] = true;
      if (destination[now.first]) distanceFromTo[t[i]][now.first] = now.second;
      for (auto &x: edge[now.first]) {
        if (!visited[x]) que.emplace(x, now.second+1);
      }
    }
  }

  vector<vector<int> > dp(1 << k, vector<int>(k, 1000000007));
  dp[0][0] = 0;
  for (int l = 0; l < k; l++) {
    dp[0|(1<<l)][l] = min(dp[0|(1<<l)][l], dp[0][0] + distanceFromTo[s][t[l]]);
  }
  for (int i = 1; i < (1 << k); i++) {
    for (int j = 0; j < k; j++) {
      for (int l = 0; l < k; l++) {
        dp[i|(1<<l)][l] = min(dp[i|(1<<l)][l], dp[i][j] + distanceFromTo[t[j]][t[l]]);
      }
    }
  }

  int ans = 1000000007;
  for (int j = 0; j < k; j++) {
    if (dp[(1<<k)-1][j] < ans) ans = dp[(1<<k)-1][j];
  }

  cout << ans << endl;

  return 0;
}
