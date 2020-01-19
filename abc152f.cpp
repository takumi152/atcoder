#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<vector<int> > edge(n);
  vector<Pii> unorderedEdge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
    unorderedEdge[i] = Pii(a-1, b-1);
  }
  int m;
  cin >> m;
  vector<Pii> uv(m);
  for (auto &x: uv) {
    cin >> x.first >> x.second;
    x.first--;
    x.second--;
  }

  // find path
  vector<unordered_set<int> > path(m);
  vector<unordered_set<int> > req(m);
  for (int i = 0; i < m; i++) {
    queue<pair<int, unordered_set<int> > > que;
    unordered_set<int> beginning;
    beginning.insert(uv[i].first);
    que.emplace(uv[i].first, beginning);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (now.first == uv[i].second) {
        path[i] = now.second;
        break;
      }
      for (auto &x: edge[now.first]) {
        if (now.second.find(x) != now.second.end()) continue;
        auto nextset = now.second;
        nextset.insert(x);
        que.emplace(x, nextset);
      }
    }
    for (int j = 0; j < n-1; j++) {
      if (path[i].find(unorderedEdge[j].first) != path[i].end() && path[i].find(unorderedEdge[j].second) != path[i].end()) req[i].insert(j);
    }
  }

  vector<int> mask(n-1);
  for (int i = 0; i < m; i++) {
    for (auto &x: req[i]) mask[x] |= (1 << i);
  }

  vector<vector<ll> > dp(n, vector<ll>(1 << m));
  dp[0][0] = 1;
  for (int i = 0; i < n-1; i++) {
    for (int j = 0; j < (1 << m); j++) dp[i+1][j] = dp[i][j];
    for (int j = 0; j < (1 << m); j++) dp[i+1][j|mask[i]] += dp[i][j];
  }

  ll ans = dp[n-1][(1<<m)-1];

  cout << ans << endl;

  return 0;
}
