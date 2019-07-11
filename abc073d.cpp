#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;
const ll inf = (ll) 1e18;

vector<vector<ll> > warshallFloyd(const vector<set<Pii> > &edge) {
  int n = edge.size();
  vector<vector<ll> > d(n, vector<ll>(n, inf));
  for (int i = 0; i < n; i++) {
    d[i][i] = 0;
    for (Pii x : edge[i]) {
      d[i][x.first] = x.second;
    }
  }
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
      }
    }
  }
  return d;
}

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

  int N, M, R;
  cin >> N >> M >> R;
  vector<int> r;
  for (int i = 0; i < R; i++) {
    int buf;
    cin >> buf;
    r.push_back(buf-1); // zero index
  }
  vector<set<Pii> > edge;
  for (int i = 0; i < N; i++) edge.push_back(set<Pii>());
  for (int i = 0; i < M; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    edge[a-1].insert(Pii(b-1, c)); // zero index
    edge[b-1].insert(Pii(a-1, c)); // zero index
  }

  vector<vector<ll> > distanceMatrix = warshallFloyd(edge);
  //vector<vector<ll> > distanceMatrix = dijkstraAll(edge);
  ll shortest = inf;
  stack<vector<int> > st;
  st.push(vector<int>());
  while (!st.empty()) {
    vector<int> now = st.top();
    st.pop();
    if (now.size() == R) {
      ll dist = 0;
      for (int i = 0; i < R-1; i++) dist += distanceMatrix[now[i]][now[i+1]];
      if (dist < shortest) shortest = dist;
      continue;
    }
    set<int> contain;
    for (int x : now) contain.insert(x);
    for (int x : r) {
      auto it = contain.find(x);
      if (it == contain.end()) {
        vector<int> next = now;
        next.push_back(x);
        st.push(next);
      }
    }
  }

  cout << shortest << endl;
  return 0;
}
