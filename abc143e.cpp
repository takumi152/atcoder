#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;
const ll fuel = (ll) 1e14;
const ll inf = (ll) 1e18;

ll L;

vector<ll> dijkstraDistOnly(const vector<set<Pii> > &edge, int start) {
  vector<ll> length(edge.size(), inf);
  vector<bool> visited(edge.size(), false);
  priority_queue<pair<ll, int> > que;
  que.push(pair<ll, int>(0, start)); // negative distance
  length[start] = 0;
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    if (visited[now.second]) continue;
    visited[now.second] = true;
    for (auto x : edge[now.second]) {
      if (visited[x.first]) continue;
      ll d = -now.first + x.second;
      if (d % fuel > L) {
        d = (d / fuel * fuel) + fuel + x.second;
      }
      if (d < length[x.first]) {
        length[x.first] = d;
        que.push(pair<ll, int>(-d, x.first));
      }
    }
  }
  return length;
}

vector<vector<ll> > dijkstraAll(const vector<set<Pii> > &edge) {
  int n = edge.size();
  vector<vector<ll> > d;
  for (int i = 0; i < n; i++) {
    d.push_back(dijkstraDistOnly(edge, i));
  }
  return d;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m >> L;
  vector<set<Pii> > edge;
  for (int i = 0; i < n; i++) edge.push_back(set<Pii>());
  for (int i = 0; i < m; i++) {
    ll a, b, c;
    cin >> a >> b >> c;
    if (c > L) continue;
    edge[a-1].insert(Pii(b-1, c)); // zero index
    edge[b-1].insert(Pii(a-1, c)); // zero index
  }
  int q;
  cin >> q;
  vector<int> s(q), t(q);
  for (int i = 0; i < q; i++) cin >> s[i] >> t[i];
  for (int i = 0; i < q; i++) {
    s[i]--;
    t[i]--;
  }

  auto distanceMatrix = dijkstraAll(edge);

  for (int i = 0; i < q; i++) {
    if (distanceMatrix[s[i]][t[i]] >= inf) cout << -1 << endl;
    else cout << (distanceMatrix[s[i]][t[i]] / fuel) << endl;
  }

  return 0;
}
