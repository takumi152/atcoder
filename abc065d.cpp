#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<ll> x(N), y(N);
  for (int i = 0; i < N; i++) cin >> x[i] >> y[i];

  vector<pair<ll, int> > xi(N), yi(N);
  for (int i = 0; i < N; i++) {
    xi[i] = make_pair(x[i], i);
    yi[i] = make_pair(y[i], i);
  }
  sort(xi.begin(), xi.end());
  sort(yi.begin(), yi.end());

  vector<set<Pii> > edge(2*N);
  for (int i = 0; i < N; i++) {
    edge[i].emplace(N+i, 0);
    edge[N+i].emplace(i, 0);
  }

  for (int i = 0; i < N-1; i++) {
    edge[xi[i].second].emplace(xi[i+1].second, xi[i+1].first - xi[i].first);
    edge[xi[i+1].second].emplace(xi[i].second, xi[i+1].first - xi[i].first);
    edge[N+yi[i].second].emplace(N+yi[i+1].second, yi[i+1].first - yi[i].first);
    edge[N+yi[i+1].second].emplace(N+yi[i].second, yi[i+1].first - yi[i].first);
  }

  priority_queue<pair<ll, int>, vector<pair<ll, int> >, greater<pair<ll, int> > > pque;
  pque.emplace(0, 0);
  vector<bool> visited(2*N);
  int nodeToVisit = 2*N;
  ll ans = 0;
  while (!pque.empty()) {
    auto now = pque.top();
    pque.pop();
    auto &cost = now.first;
    auto &node = now.second;
    if (visited[node]) continue;
    visited[node] = true;
    nodeToVisit--;
    ans += cost;
    if (nodeToVisit <= 0) break;
    for (auto &x: edge[node]) {
      pque.emplace(x.second, x.first);
    }
  }

  cout << ans << endl;

  return 0;
}
