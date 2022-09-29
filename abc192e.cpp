#include <iostream>
#include <iomanip>
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

  int n, m, x, y;
  cin >> n >> m >> x >> y;
  vector<vector<vector<ll> > > edge(n);
  for (int i = 0; i < m; i++) {
    ll a, b, t, k;
    cin >> a >> b >> t >> k;
    edge[a-1].push_back(vector<ll>({b-1, t, k}));
    edge[b-1].push_back(vector<ll>({a-1, t, k}));
  }

  vector<bool> visited(n);
  priority_queue<pair<ll, int>, vector<pair<ll, int> >, greater<pair<ll, int> > > que;
  que.emplace(0, x-1);
  while (!que.empty()) {
    auto [t, now] = que.top();
    que.pop();
    if (visited[now]) continue;
    visited[now] = true;
    if (now == y-1) {
      cout << t << endl;
      return 0;
    }
    for (auto &next: edge[now]) {
      if (visited[next[0]]) continue;
      ll delay = next[2] - t % next[2];
      if (delay == next[2]) delay = 0;
      que.emplace(t + delay + next[1], next[0]);
    }
  }

  cout << -1 << endl;

  return 0;
}
