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

  int n, m;
  cin >> n >> m;
  vector<ll> d(n);
  for (auto &x: d) cin >> x;
  vector<vector<Pii> > edge(n);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    edge[u-1].emplace_back(v-1, i);
    edge[v-1].emplace_back(u-1, i);
  }

  vector<bool> decided(m);
  vector<bool> isBlack(n);
  vector<ll> cost(m, 1000000000);
  priority_queue<Pii, vector<Pii>, greater<Pii> > que;
  for (int i = 0; i < n; i++) que.emplace(d[i], i);
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    if (decided[now.second]) continue;
    bool good = false;
    for (auto &x: edge[now.second]) {
      if (decided[x.first] && isBlack[x.first]) {
        cost[x.second] = d[now.second];
        decided[now.second] = true;
        good = true;
        break;
      }
    }
    if (good) continue;
    for (auto &x: edge[now.second]) {
      if (decided[x.first] && d[x.first] < d[now.second]) {
        cost[x.second] = d[now.second] - d[x.first];
        decided[now.second] = true;
        good = true;
        break;
      }
    }
    if (good) continue;
    for (auto &x: edge[now.second]) {
      if (!decided[x.first] && d[x.first] == d[now.second]) {
        cost[x.second] = d[now.second];
        isBlack[x.first] = true;
        decided[x.first] = true;
        decided[now.second] = true;
        good = true;
        break;
      }
    }
    if (good) continue;
    for (auto &x: edge[now.second]) {
      if (decided[x.first] && d[x.first] >= d[now.second]) {
        cost[x.second] = d[now.second];
        isBlack[now.second] = true;
        decided[x.first] = true;
        decided[now.second] = true;
        good = true;
        break;
      }
    }
    if (good) continue;
    cout << -1 << endl; // impossible
    return 0;
  }

  for (int i = 0; i < n; i++) {
    if (isBlack[i]) cout << "B";
    else cout << "W";
  }
  cout << endl;

  for (auto &x: cost) cout << x << endl;

  return 0;
}
