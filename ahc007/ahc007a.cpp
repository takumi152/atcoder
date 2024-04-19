#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <random>
#include <cmath>
#include <cassert>

#include <x86intrin.h>

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned short nextUShort() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUShortMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x0000ffffffffffff) * mod) >> 48;
  }
  inline unsigned int nextUInt() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUIntMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x00000000ffffffff) * mod) >> 32;
  }
  inline unsigned long long int nextULL() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline double nextDouble() {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return (double)x * 5.42101086242752217e-20;
  }
};

struct timer {
  double t = 0.0;
  double lastStop = 0.0;
  bool stopped = false;
  timer() {
    restart();
  }
  inline void restart() {
    t = now();
    stopped = false;
  }
  inline void start() {
    if (stopped) {
      t += now() - lastStop;
      stopped = false;
    }
  }
  inline void stop() {
    if (!stopped) {
      lastStop = now();
      stopped = true;
    }
  }
  inline double time() {
    if (stopped) return lastStop - t;
    else return now() - t;
  }
  inline double now() {
    unsigned long long l, h;
    __asm__ ("rdtsc" : "=a"(l), "=d"(h));
    #ifdef LOCAL
    return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
    #else
    //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
    //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
    return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
    #endif
  }
};

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;
mt19937 theMersenne(1);

// structs
struct unionfind {
  vector<int> group;

  unionfind(int n) {
    group = vector<int>(n);
    for (int i = 0; i < n; i++) group[i] = i;
  }

  int root(int x) {
    if (group[x] == x) return x;
    return group[x] = root(group[x]);
  }

  void unite(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    if (rx == ry) return;
    group[rx] = ry;
  }

  bool same(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    return rx == ry;
  }
};

// hyper parameters
double unchosen_length_base_factor = 1.6186213868072936;
double unchosen_length_increase_factor = 0.0001134135481355772;
int unchosen_length_increase_begin = 0;

// constants
constexpr int n = 400;
constexpr int m = 1995;

// inputs
vector<Pii> vertex_pos(n);
vector<Pii> edge_raw(m);

// outputs

// internals
vector<vector<Pii> > edge(n);

vector<int> base_length(m);
vector<double> actual_or_estimated_length(m);

vector<bool> included_in_tree(m);

int get_input() {
  int len;
  cin >> len;
  return len;
}

void answer(int ans) {
  cout << ans << endl;
}

void solve() {
  for (int i = 0; i < m; i++) {
    auto [a, b] = edge_raw[i];
    edge[a].emplace_back(i, b); // id, connect_to
    edge[b].emplace_back(i, a);
  }

  for (int i = 0; i < m; i++) {
    auto [a, b] = edge_raw[i];
    base_length[i] = round(sqrt((vertex_pos[a].first - vertex_pos[b].first) * (vertex_pos[a].first - vertex_pos[b].first) + (vertex_pos[a].second - vertex_pos[b].second) * (vertex_pos[a].second - vertex_pos[b].second)));
    actual_or_estimated_length[i] = (double) base_length[i] * (unchosen_length_base_factor + unchosen_length_increase_factor * (double) max(0, i - unchosen_length_increase_begin));
  }

  double score = 0.0;
  {
    unionfind uf(n);
    priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > que;
    for (int i = 0; i < m; i++) que.emplace(actual_or_estimated_length[i], i);
    while (!que.empty()) {
      auto [len, id] = que.top();
      que.pop();
      auto [va, vb] = edge_raw[id];
      if (uf.same(va, vb)) continue;
      uf.unite(va, vb);
      included_in_tree[id] = true;
      score += len;
    }
  }

  for (int target_edge = 0; target_edge < m; target_edge++) {
    int target_edge_len = get_input();

    double global_best_score = score;
    if (included_in_tree[target_edge]) global_best_score += target_edge_len - actual_or_estimated_length[target_edge];
    auto global_best_included_in_tree = included_in_tree;
    double score_orig = score;
    auto included_in_tree_orig = included_in_tree;

    {
      auto update_target_included = [&]() {
        auto connection_split = [&](int va, int vb) {
          vector<bool> visited(n);
          queue<int> que;
          que.push(va);
          while (!que.empty()) {
            auto now = que.front();
            que.pop();
            visited[now] = true;
            for (auto &[next_edge_id, next_vertex]: edge[now]) {
              if (included_in_tree[next_edge_id] && !visited[next_vertex] && next_vertex != vb) que.push(next_vertex);
            }
          }

          return visited;
        };

        auto connected_on_va = connection_split(edge_raw[target_edge].first, edge_raw[target_edge].second);

        int best_edge = target_edge;
        double best_length = (double) target_edge_len;
        for (int next_edge = target_edge + 1; next_edge < m; next_edge++) {
          if (connected_on_va[edge_raw[next_edge].first] != connected_on_va[edge_raw[next_edge].second] && actual_or_estimated_length[next_edge] < best_length) {
            best_edge = next_edge;
            best_length = actual_or_estimated_length[next_edge];
          }
        }

        if (best_edge != target_edge) {
          included_in_tree[best_edge] = true;
          included_in_tree[target_edge] = false;
          score += actual_or_estimated_length[best_edge] - actual_or_estimated_length[target_edge];
          return true;
        }
        else {
          return false;
        }
      };

      auto update_target_excluded = [&]() {
        auto find_loop = [&](int va, int vb) {
          vector<int> prev_edge(n);
          vector<int> prev_vertex(n);
          vector<bool> visited(n);
          queue<int> que;
          que.push(va);
          while (!que.empty()) {
            auto now_vertex = que.front();
            que.pop();
            visited[now_vertex] = true;
            if (now_vertex == vb) break;
            for (auto &[next_edge_id, next_vertex]: edge[now_vertex]) {
              if (included_in_tree[next_edge_id] && !visited[next_vertex]) {
                prev_edge[next_vertex] = next_edge_id;
                prev_vertex[next_vertex] = now_vertex;
                que.push(next_vertex);
              }
            }
          }

          vector<int> loop_edge;
          int vp = vb;
          while (vp != va) {
            loop_edge.push_back(prev_edge[vp]);
            vp = prev_vertex[vp];
          }

          return loop_edge;
        };

        auto loop_edge = find_loop(edge_raw[target_edge].first, edge_raw[target_edge].second);

        int worst_edge = target_edge;
        double worst_length = (double) target_edge_len;

        for (auto &e: loop_edge) {
          if (e > target_edge && actual_or_estimated_length[e] > worst_length) {
            worst_edge = e;
            worst_length = actual_or_estimated_length[e];
          }
        }

        if (worst_edge != target_edge) {
          included_in_tree[target_edge] = true;
          included_in_tree[worst_edge] = false;
          score += actual_or_estimated_length[target_edge] - actual_or_estimated_length[worst_edge];
          return true;
        }
        else {
          return false;
        }
      };

      while (true) {
        if (included_in_tree[target_edge]) {
          bool updated = update_target_included();
          if (!updated) break;
        }
        else {
          bool updated = update_target_excluded();
          if (!updated) break;
        }
      }

      if (included_in_tree[target_edge]) {
        score += target_edge_len - actual_or_estimated_length[target_edge];
      }
    }

    if (score < global_best_score) {
      global_best_score = score;
      global_best_included_in_tree = included_in_tree;
    }

    score = score_orig;
    included_in_tree = included_in_tree_orig;

    {
      auto update_target_included = [&](bool force = false) {
        auto connection_split = [&](int va, int vb) {
          vector<bool> visited(n);
          queue<int> que;
          que.push(va);
          while (!que.empty()) {
            auto now = que.front();
            que.pop();
            visited[now] = true;
            for (auto &[next_edge_id, next_vertex]: edge[now]) {
              if (included_in_tree[next_edge_id] && !visited[next_vertex] && next_vertex != vb) que.push(next_vertex);
            }
          }

          return visited;
        };

        auto connected_on_va = connection_split(edge_raw[target_edge].first, edge_raw[target_edge].second);

        int best_edge = target_edge;
        double best_length = (double) target_edge_len;
        if (force) best_length = 1e100;
        for (int next_edge = target_edge + 1; next_edge < m; next_edge++) {
          if (connected_on_va[edge_raw[next_edge].first] != connected_on_va[edge_raw[next_edge].second] && actual_or_estimated_length[next_edge] < best_length) {
            best_edge = next_edge;
            best_length = actual_or_estimated_length[next_edge];
          }
        }

        if (best_edge != target_edge) {
          included_in_tree[best_edge] = true;
          included_in_tree[target_edge] = false;
          score += actual_or_estimated_length[best_edge] - actual_or_estimated_length[target_edge];
          return true;
        }
        else {
          return false;
        }
      };

      auto update_target_excluded = [&](bool force = false) {
        auto find_loop = [&](int va, int vb) {
          vector<int> prev_edge(n);
          vector<int> prev_vertex(n);
          vector<bool> visited(n);
          queue<int> que;
          que.push(va);
          while (!que.empty()) {
            auto now_vertex = que.front();
            que.pop();
            visited[now_vertex] = true;
            if (now_vertex == vb) break;
            for (auto &[next_edge_id, next_vertex]: edge[now_vertex]) {
              if (included_in_tree[next_edge_id] && !visited[next_vertex]) {
                prev_edge[next_vertex] = next_edge_id;
                prev_vertex[next_vertex] = now_vertex;
                que.push(next_vertex);
              }
            }
          }

          vector<int> loop_edge;
          int vp = vb;
          while (vp != va) {
            loop_edge.push_back(prev_edge[vp]);
            vp = prev_vertex[vp];
          }

          return loop_edge;
        };

        auto loop_edge = find_loop(edge_raw[target_edge].first, edge_raw[target_edge].second);

        int worst_edge = target_edge;
        double worst_length = (double) target_edge_len;
        if (force) worst_length = -1e100;

        for (auto &e: loop_edge) {
          if (e > target_edge && actual_or_estimated_length[e] > worst_length) {
            worst_edge = e;
            worst_length = actual_or_estimated_length[e];
          }
        }

        if (worst_edge != target_edge) {
          included_in_tree[target_edge] = true;
          included_in_tree[worst_edge] = false;
          score += actual_or_estimated_length[target_edge] - actual_or_estimated_length[worst_edge];
          return true;
        }
        else {
          return false;
        }
      };

      if (included_in_tree[target_edge]) {
        update_target_included(true);
      }
      else {
        update_target_excluded(true);
      }

      if (included_in_tree[target_edge]) {
        score += target_edge_len - actual_or_estimated_length[target_edge];
      }
    }

    if (score < global_best_score) {
      global_best_score = score;
      global_best_included_in_tree = included_in_tree;
    }

    score = global_best_score;
    included_in_tree = global_best_included_in_tree;

    if (included_in_tree[target_edge]) answer(1);
    else answer(0);
    actual_or_estimated_length[target_edge] = target_edge_len;
  }

  int optimal_score = 0;
  {
    unionfind uf(n);
    priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > que;
    for (int i = 0; i < m; i++) que.emplace(actual_or_estimated_length[i], i);
    while (!que.empty()) {
      auto [len, id] = que.top();
      que.pop();
      auto [va, vb] = edge_raw[id];
      if (uf.same(va, vb)) continue;
      uf.unite(va, vb);
      optimal_score += len;
    }
  }

  cerr << "score = " << (int) (1e9 * (double) optimal_score / score) << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  if (argc > 1) unchosen_length_base_factor = strtod(argv[1], nullptr);
  if (argc > 2) unchosen_length_increase_factor = strtod(argv[2], nullptr);
  if (argc > 3) unchosen_length_increase_begin = strtol(argv[3], nullptr, 10);

  for (auto &x: vertex_pos) cin >> x.first >> x.second;
  for (auto &x: edge_raw) cin >> x.first >> x.second;

  solve();

  return 0;
}
