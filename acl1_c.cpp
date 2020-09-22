#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cassert>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

struct min_cost_flow_graph {
public:
  min_cost_flow_graph() {}
  min_cost_flow_graph(int n) : _n(n), g(n) {}

  int add_edge(int from, int to, ll cap, ll cost) {
    assert(0 <= from && from < _n);
    assert(0 <= to && to < _n);
    int m = int(pos.size());
    pos.push_back({from, int(g[from].size())});
    int from_id = int(g[from].size());
    int to_id = int(g[to].size());
    if (from == to) to_id++;
    g[from].push_back(_edge{to, to_id, cap, cost});
    g[to].push_back(_edge{from, from_id, 0, -cost});
    return m;
  }

  struct edge {
    int from, to;
    ll cap, flow;
    ll cost;
  };

  edge get_edge(int i) {
    int m = int(pos.size());
    assert(0 <= i && i < m);
    auto _e = g[pos[i].first][pos[i].second];
    auto _re = g[_e.to][_e.rev];
    return edge{
      pos[i].first, _e.to, _e.cap + _re.cap, _re.cap, _e.cost,
    };
  }
  vector<edge> edges() {
    int m = int(pos.size());
    vector<edge> result(m);
    for (int i = 0; i < m; i++) {
      result[i] = get_edge(i);
    }
    return result;
  }

  pair<ll, ll> flow(int s, int t) {
    return flow(s, t, numeric_limits<ll>::max());
  }
  pair<ll, ll> flow(int s, int t, ll flow_limit) {
    return slope(s, t, flow_limit).back();
  }
  vector<pair<ll, ll> > slope(int s, int t) {
    return slope(s, t, numeric_limits<ll>::max());
  }
  vector<pair<ll, ll> > slope(int s, int t, ll flow_limit) {
    assert(0 <= s && s < _n);
    assert(0 <= t && t < _n);
    assert(s != t);
    // variants (C = maxcost):
    // -(n-1)C <= dual[s] <= dual[i] <= dual[t] = 0
    // reduced cost (= e.cost + dual[e.from] - dual[e.to]) >= 0 for all edge
    vector<ll> dual(_n, 0), dist(_n);
    vector<int> pv(_n), pe(_n);
    vector<bool> vis(_n);
    auto dual_ref = [&]() {
      fill(dist.begin(), dist.end(), numeric_limits<ll>::max());
      fill(pv.begin(), pv.end(), -1);
      fill(pe.begin(), pe.end(), -1);
      fill(vis.begin(), vis.end(), false);
      struct Q {
        ll key;
        int to;
        bool operator<(Q r) const { return key > r.key; }
      };
      priority_queue<Q> que;
      dist[s] = 0;
      que.push(Q{0, s});
      while (!que.empty()) {
        int v = que.top().to;
        que.pop();
        if (vis[v]) continue;
        vis[v] = true;
        if (v == t) break;
        // dist[v] = shortest(s, v) + dual[s] - dual[v]
        // dist[v] >= 0 (all reduced cost are positive)
        // dist[v] <= (n-1)C
        for (int i = 0; i < int(g[v].size()); i++) {
          auto e = g[v][i];
          if (vis[e.to] || !e.cap) continue;
          // |-dual[e.to] + dual[v]| <= (n-1)C
          // cost <= C - -(n-1)C + 0 = nC
          ll cost = e.cost - dual[e.to] + dual[v];
          if (dist[e.to] - dist[v] > cost) {
            dist[e.to] = dist[v] + cost;
            pv[e.to] = v;
            pe[e.to] = i;
            que.push(Q{dist[e.to], e.to});
          }
        }
      }
      if (!vis[t]) {
        return false;
      }

      for (int v = 0; v < _n; v++) {
        if (!vis[v]) continue;
        // dual[v] = dual[v] - dist[t] + dist[v]
        //         = dual[v] - (shortest(s, t) + dual[s] - dual[t]) + (shortest(s, v) + dual[s] - dual[v])
        //         = - shortest(s, t) + dual[t] + shortest(s, v)
        //         = shortest(s, v) - shortest(s, t) >= 0 - (n-1)C
        dual[v] -= dist[t] - dist[v];
      }
      return true;
    };
    ll flow = 0;
    ll cost = 0, prev_cost_per_flow = -1;
    vector<pair<ll, ll> > result;
    result.push_back({flow, cost});
    while (flow < flow_limit) {
      if (!dual_ref()) break;
      ll c = flow_limit - flow;
      for (int v = t; v != s; v = pv[v]) {
        c = min(c, g[pv[v]][pe[v]].cap);
      }
      for (int v = t; v != s; v = pv[v]) {
        auto& e = g[pv[v]][pe[v]];
        e.cap -= c;
        g[v][e.rev].cap += c;
      }
      ll d = -dual[s];
      flow += c;
      cost += c * d;
      if (prev_cost_per_flow == d) {
        result.pop_back();
      }
      result.push_back({flow, cost});
      prev_cost_per_flow = d;
    }
    return result;
  }

private:
  int _n;

  struct _edge {
    int to, rev;
    ll cap;
    ll cost;
  };

  vector<Pii> pos;
  vector<vector<_edge> > g;
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  int vertex_needed = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (s[i][j] == 'o') {
        vector<vector<bool> > visited(n, vector<bool>(m));
        queue<Pii> que;
        que.emplace(i, j);
        while (!que.empty()) {
          auto [k, l] = que.front();
          que.pop();
          if (visited[k][l]) continue;
          visited[k][l] = true;
          vertex_needed++;
          if (k < n-1) {
            if (s[k+1][l] != '#' && !visited[k+1][l]) que.emplace(k+1, l);
          }
          if (l < m-1) {
            if (s[k][l+1] != '#' && !visited[k][l+1]) que.emplace(k, l+1);
          }
        }
      }
    }
  }

  min_cost_flow_graph mcf(n*m+2+vertex_needed);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      mcf.add_edge(i*m+j, n*m+1, 1, 0);
    }
  }
  int next_vertex = n*m+2;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (s[i][j] == 'o') {
        mcf.add_edge(n*m, next_vertex, 1, 0);
        vector<vector<bool> > visited(n, vector<bool>(m));
        vector<vector<int> > vertex_num(n, vector<int>(m, -1));
        queue<Pii> que;
        vertex_num[i][j] = next_vertex;
        next_vertex++;
        que.emplace(i, j);
        while (!que.empty()) {
          auto [k, l] = que.front();
          que.pop();
          if (visited[k][l]) continue;
          visited[k][l] = true;
          mcf.add_edge(vertex_num[k][l], k*m+l, 1, 1000000000 - (k - i) - (l - j));
          if (k < n-1) {
            if (s[k+1][l] != '#' && !visited[k+1][l]) {
              if (vertex_num[k+1][l] < 0) {
                vertex_num[k+1][l] = next_vertex;
                next_vertex++;
              }
              mcf.add_edge(vertex_num[k][l], vertex_num[k+1][l], 1, 0);
              que.emplace(k+1, l);
            }
          }
          if (l < m-1) {
            if (s[k][l+1] != '#' && !visited[k][l+1]) {
              if (vertex_num[k][l+1] < 0) {
                vertex_num[k][l+1] = next_vertex;
                next_vertex++;
              }
              mcf.add_edge(vertex_num[k][l], vertex_num[k][l+1], 1, 0);
              que.emplace(k, l+1);
            }
          }
        }
      }
    }
  }

  auto f = mcf.flow(n*m, n*m+1);
  ll ans = 1000000000LL * f.first - f.second;

  cout << ans << endl;

  return 0;
}
