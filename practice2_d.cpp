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

struct max_flow_graph {
public:
  max_flow_graph() : _n(0) {}
  max_flow_graph(int n) : _n(n), g(n) {}

  int add_edge(int from, int to, ll cap) {
    assert(0 <= from && from < _n);
    assert(0 <= to && to < _n);
    assert(0 <= cap);
    int m = int(pos.size());
    pos.push_back({from, int(g[from].size())});
    int from_id = int(g[from].size());
    int to_id = int(g[to].size());
    if (from == to) to_id++;
    g[from].push_back(_edge{to, to_id, cap});
    g[to].push_back(_edge{from, from_id, 0});
    return m;
  }

  struct edge {
    int from, to;
    ll cap, flow;
  };

  edge get_edge(int i) {
    int m = int(pos.size());
    assert(0 <= i && i < m);
    auto _e = g[pos[i].first][pos[i].second];
    auto _re = g[_e.to][_e.rev];
    return edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap};
  }
  vector<edge> edges() {
    int m = int(pos.size());
    vector<edge> result;
    for (int i = 0; i < m; i++) {
      result.push_back(get_edge(i));
    }
    return result;
  }
  void change_edge(int i, ll new_cap, ll new_flow) {
    int m = int(pos.size());
    assert(0 <= i && i < m);
    assert(0 <= new_flow && new_flow <= new_cap);
    auto& _e = g[pos[i].first][pos[i].second];
    auto& _re = g[_e.to][_e.rev];
    _e.cap = new_cap - new_flow;
    _re.cap = new_flow;
  }

  ll flow(int s, int t) {
    return flow(s, t, numeric_limits<ll>::max());
  }
  ll flow(int s, int t, ll flow_limit) {
    assert(0 <= s && s < _n);
    assert(0 <= t && t < _n);
    assert(s != t);

    vector<int> level(_n), iter(_n);
    queue<int> que;

    auto bfs = [&]() {
      fill(level.begin(), level.end(), -1);
      level[s] = 0;
      while (!que.empty()) que.pop();
      que.push(s);
      while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (auto e : g[v]) {
          if (e.cap == 0 || level[e.to] >= 0) continue;
          level[e.to] = level[v] + 1;
          if (e.to == t) return;
          que.push(e.to);
        }
      }
    };
    auto dfs = [&](auto self, int v, ll up) {
      if (v == s) return up;
      ll res = 0;
      int level_v = level[v];
      for (int& i = iter[v]; i < int(g[v].size()); i++) {
        _edge& e = g[v][i];
        if (level_v <= level[e.to] || g[e.to][e.rev].cap == 0) continue;
        ll d = self(self, e.to, min(up - res, g[e.to][e.rev].cap));
        if (d <= 0) continue;
        g[v][i].cap += d;
        g[e.to][e.rev].cap -= d;
        res += d;
        if (res == up) break;
      }
      return res;
    };

    ll flow = 0;
    while (flow < flow_limit) {
      bfs();
      if (level[t] == -1) break;
      fill(iter.begin(), iter.end(), 0);
      while (flow < flow_limit) {
        ll f = dfs(dfs, t, flow_limit - flow);
        if (!f) break;
        flow += f;
      }
    }
    return flow;
  }

  vector<bool> min_cut(int s) {
    vector<bool> visited(_n);
    queue<int> que;
    que.push(s);
    while (!que.empty()) {
      int p = que.front();
      que.pop();
      visited[p] = true;
      for (auto e : g[p]) {
        if (e.cap && !visited[e.to]) {
          visited[e.to] = true;
          que.push(e.to);
        }
      }
    }
    return visited;
  }

private:
  int _n;
  struct _edge {
    int to, rev;
    ll cap;
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

  max_flow_graph mf(n*m+2);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (s[i][j] == '#') continue;
      if ((i + j) % 2 == 0) {
        mf.add_edge(n*m, i*m+j, 1);
        if (i > 0) {
          if (s[i-1][j] == '.') mf.add_edge(i*m+j, (i-1)*m+j, 1);
        }
        if (i < n-1) {
          if (s[i+1][j] == '.') mf.add_edge(i*m+j, (i+1)*m+j, 1);
        }
        if (j > 0) {
          if (s[i][j-1] == '.') mf.add_edge(i*m+j, i*m+(j-1), 1);
        }
        if (j < m-1) {
          if (s[i][j+1] == '.') mf.add_edge(i*m+j, i*m+(j+1), 1);
        }
      }
      else {
        mf.add_edge(i*m+j, n*m+1, 1);
      }
    }
  }

  int ans = mf.flow(n*m, n*m+1);

  auto edge = mf.edges();
  for (auto &e: edge) {
    if (e.from == n*m || e.to == n*m+1) continue;
    if (e.flow == 0) continue;
    if (e.from/m == e.to/m) {
      if (e.from%m < e.to%m) {
        s[e.from/m][e.from%m] = '>';
        s[e.to/m][e.to%m] = '<';
      }
      else {
        s[e.from/m][e.from%m] = '<';
        s[e.to/m][e.to%m] = '>';
      }
    }
    else {
      if (e.from/m < e.to/m) {
        s[e.from/m][e.from%m] = 'v';
        s[e.to/m][e.to%m] = '^';
      }
      else {
        s[e.from/m][e.from%m] = '^';
        s[e.to/m][e.to%m] = 'v';
      }
    }
  }

  cout << ans << endl;
  for (auto &x: s) cout << x << endl;

  return 0;
}
