#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <stack>
#include <queue>

using namespace std;

typedef pair<int, int> Pii;
typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<vector<pair<int, ll> > > edge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    edge[a-1].emplace_back(b-1, c);
    edge[b-1].emplace_back(a-1, c);
  }
  int q, k;
  cin >> q >> k;
  k--;
  vector<int> x(q), y(q);
  for (int i = 0; i < q; i++) {
    cin >> x[i] >> y[i];
    x[i]--;
    y[i]--;
  }

  vector<ll> distanceToK(n);
  vector<bool> visited(n);
  queue<pair<int, ll> > que;
  que.emplace(k, 0);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now.first]) continue;
    visited[now.first] = true;
    distanceToK[now.first] = now.second;
    for (auto &z: edge[now.first]) {
      if (!visited[z.first]) que.emplace(z.first, now.second + z.second);
    }
  }

  vector<ll> ans(q);
  for (int i = 0; i < q; i++) ans[i] = distanceToK[x[i]] + distanceToK[y[i]];

  for (auto &z: ans) cout << z << endl;

  return 0;
}
