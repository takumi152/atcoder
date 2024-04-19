#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <random>
#include <functional>
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
typedef unsigned long long int ull;
typedef pair<int, int> Pii;
typedef unsigned char uchar;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;
mt19937 theMersenne(1);

// hyper parameters

// structs
struct index_set {
  vector<int> included;
  vector<int> included_pos;
  vector<int> excluded;
  vector<int> excluded_pos;

  index_set() {};

  index_set(int n) {
    included.reserve(n);
    excluded.reserve(n);

    for (int i = 0; i < n; i++) excluded.push_back(i);

    included_pos = vector<int>(n, -1);
    excluded_pos = vector<int>(n);
    for (int i = 0; i < n; i++) excluded_pos[i] = i;
  }

  void include(int index) {
    assert(0 <= index && index < (int) excluded.size());
    included.push_back(excluded[index]);
    included_pos[excluded[index]] = included.size()-1;
    swap(excluded_pos[excluded[index]], excluded_pos[excluded[excluded.size()-1]]);
    excluded_pos[excluded[index]] = -1;
    swap(excluded[index], excluded[excluded.size()-1]);
    excluded.pop_back();
  }

  void exclude(int index) {
    assert(0 <= index && index < (int) included.size());
    excluded.push_back(included[index]);
    excluded_pos[included[index]] = excluded.size()-1;
    swap(included_pos[included[index]], included_pos[included[included.size()-1]]);
    included_pos[included[index]] = -1;
    swap(included[index], included[included.size()-1]);
    included.pop_back();
  }

  bool is_included(int element) {
    return included_pos[element] != -1;
  }

  bool is_excluded(int element) {
    return excluded_pos[element] != -1;
  }

  bool include_element(int element) {
    assert(0 <= element && element < (int) excluded_pos.size());
    if (is_excluded(element)) {
      include(excluded_pos[element]);
      return true;
    }
    else return false;
  }

  bool exclude_element(int element) {
    assert(0 <= element && element < (int) included_pos.size());
    if (is_included(element)) {
      exclude(included_pos[element]);
      return true;
    }
    else return false;
  }

  int included_size() {
    return included.size();
  }

  int excluded_size() {
    return excluded.size();
  }
};

struct min_segtree {
  int n;
  vector<double> data;

  min_segtree() {
    init(1);
  }

  min_segtree(const int s) {
    init(s);
  }

  min_segtree(const int s, const double u) {
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = u;
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = min(data[i*2+1], data[i*2+2]);
    }
  }

  min_segtree(const vector<double> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = min(data[i*2+1], data[i*2+2]);
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<double>(2*n-1, 1e308);
  }

  void update(int x, double v) {
    int p = x + n - 1;

    // update bottom
    data[p] = v;

    // update rest
    while (p > 0) {
      p = (p - 1) >> 1;
      data[p] = min(data[p*2+1], data[p*2+2]);
    }
  }

  double query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 1e308; // out of range

    if (l <= a && b <= r) return data[p];// fully covered

    double vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    double vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return min(vl, vr);
  }
};

struct normal_prob_distribution {
  double mean;
  double variance;

  normal_prob_distribution() : mean(0.0), variance(1.0) {}
  normal_prob_distribution(double mean, double variance) : mean(mean), variance(variance) {}

  // 二項分布を正規分布で近似したものを返す
  static normal_prob_distribution approximation_of_binary_distribution(int n, double p) {
    return normal_prob_distribution(n * p, n * p * (1.0 - p));
  }

  // 正規分布の線形結合
  normal_prob_distribution operator+(const normal_prob_distribution& rhs) const {
    return normal_prob_distribution(this->mean + rhs.mean, this->variance + rhs.variance);
  }
};

// enums

// constants
constexpr int graph_size = 100;
constexpr int graph_num = 100;
constexpr double error_rate = 0.40;

// inputs

// outputs

// internals
vector<vector<index_set>> graph_edges;
vector<vector<normal_prob_distribution>> graph_degree_dist;
vector<vector<normal_prob_distribution>> graph_degree_dist_sorted;
min_segtree min_score_tree;

// 単変量正規分布同士のKL情報量を計算する
double kl_divergence(normal_prob_distribution dist1, normal_prob_distribution dist2) {
  return (dist1.mean - dist2.mean) * (dist1.mean - dist2.mean);
}

void update_score(int g1, int g2) {
  if (g1 == g2) return;
  if (g1 > g2) swap(g1, g2);

  // ソートした次数列の組について、双方向のKL情報量の和を求める
  // 面倒なので各次元は独立であるとみなして計算する
  double score = 0.0;
  for (int k = 0; k < graph_size; k++) {
    score += kl_divergence(graph_degree_dist_sorted[g1][k], graph_degree_dist_sorted[g2][k]);
  }

  min_score_tree.update(g1 * graph_num + g2, sqrt(score));
}

void init() {
  graph_edges = vector<vector<index_set>>(graph_num, vector<index_set>(graph_size, index_set(graph_size)));
  // ランダムな初期グラフを生成
  for (int i = 0; i < graph_num; i++) {
    for (int j = 0; j < graph_size; j++) {
      double edge_prob = theRandom.nextDouble();
      for (int k = j + 1; k < graph_size; k++) {
        if (theRandom.nextDouble() < edge_prob) {
          graph_edges[i][j].include_element(k);
          graph_edges[i][k].include_element(j);
        }
      }
    }
  }

  graph_degree_dist = vector<vector<normal_prob_distribution>>(graph_num, vector<normal_prob_distribution>(graph_size));
  for (int i = 0; i < graph_num; i++) {
    for (int j = 0; j < graph_size; j++) {
      graph_degree_dist[i][j] = normal_prob_distribution::approximation_of_binary_distribution(graph_edges[i][j].excluded.size(), error_rate) + normal_prob_distribution::approximation_of_binary_distribution(graph_edges[i][j].included.size(), 1.0 - error_rate);
    }
  }

  graph_degree_dist_sorted = graph_degree_dist;
  for (int i = 0; i < graph_num; i++) {
    sort(graph_degree_dist_sorted[i].rbegin(), graph_degree_dist_sorted[i].rend(), [&](auto &a, auto &b){return a.mean > b.mean;});
  }

  min_score_tree = min_segtree(graph_num * graph_num);
  for (int i = 0; i < graph_num; i++) {
    for (int j = i + 1; j < graph_num; j++) update_score(i, j);
  }
}

double calc_score() {
  return min_score_tree.query(0, graph_num * graph_num);
}

void solve() {
  double score = calc_score();
  double last_score = score;
  double best_score = score;

  const double base_temperature = 1e-3;
  const double target_temperature = 1e-4;
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;

  double time_start = theTimer.time();
  const double time_limit = 19999.900;
  int iter_count = 0;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.90) {
      int g = theRandom.nextUIntMod(graph_num);
      int v1 = theRandom.nextUIntMod(graph_size);
      
      int v2;
      if (theRandom.nextDouble() < 0.5) {
        if (graph_edges[g][v1].excluded.empty()) continue;
        v2 = graph_edges[g][v1].excluded[theRandom.nextUIntMod(graph_edges[g][v1].excluded.size())];
      }
      else {
        if (graph_edges[g][v1].included.empty()) continue;
        v2 = graph_edges[g][v1].included[theRandom.nextUIntMod(graph_edges[g][v1].included.size())];
      }
      if (v1 == v2) continue;

      if (graph_edges[g][v1].is_excluded(v2)) {
        graph_edges[g][v1].include_element(v2);
        graph_edges[g][v2].include_element(v1);
      }
      else {
        graph_edges[g][v1].exclude_element(v2);
        graph_edges[g][v2].exclude_element(v1);
      }
      
      graph_degree_dist[g][v1] = normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][v1].excluded.size(), error_rate) + normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][v1].included.size(), 1.0 - error_rate);
      graph_degree_dist[g][v2] = normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][v2].excluded.size(), error_rate) + normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][v2].included.size(), 1.0 - error_rate);

      graph_degree_dist_sorted[g] = graph_degree_dist[g];
      sort(graph_degree_dist_sorted[g].rbegin(), graph_degree_dist_sorted[g].rend(), [&](auto &a, auto &b){return a.mean > b.mean;});

      for (int i = 0; i < graph_num; i++) update_score(g, i);
      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        if (score > best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        if (graph_edges[g][v1].is_excluded(v2)) {
          graph_edges[g][v1].include_element(v2);
          graph_edges[g][v2].include_element(v1);
        }
        else {
          graph_edges[g][v1].exclude_element(v2);
          graph_edges[g][v2].exclude_element(v1);
        }

        graph_degree_dist[g][v1] = normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][v1].excluded.size(), error_rate) + normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][v1].included.size(), 1.0 - error_rate);
        graph_degree_dist[g][v2] = normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][v2].excluded.size(), error_rate) + normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][v2].included.size(), 1.0 - error_rate);

        graph_degree_dist_sorted[g] = graph_degree_dist[g];
        sort(graph_degree_dist_sorted[g].rbegin(), graph_degree_dist_sorted[g].rend(), [&](auto &a, auto &b){return a.mean > b.mean;});

        for (int i = 0; i < graph_num; i++) update_score(g, i);
        score = last_score;
      }
    }
    else if (roll < 1.00) {
      int g = theRandom.nextUIntMod(graph_num);
      int v = theRandom.nextUIntMod(graph_size);

      vector<bool> prev_edge_info(graph_size);
      for (int i = 0; i < graph_size; i++) prev_edge_info[i] = graph_edges[g][v].is_included(i);

      double edge_chance = theRandom.nextDouble();
      for (int i = 0; i < graph_size; i++) {
        if (v == i) continue;
        double edge_roll = theRandom.nextDouble();
        if (edge_roll < edge_chance && graph_edges[g][v].is_excluded(i)) {
          graph_edges[g][v].include_element(i);
          graph_edges[g][i].include_element(v);
        }
        else if (edge_roll > edge_chance && graph_edges[g][v].is_included(i)) {
          graph_edges[g][v].exclude_element(i);
          graph_edges[g][i].exclude_element(v);
        }
      }

      for (int i = 0; i < graph_size; i++) graph_degree_dist[g][i] = normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][i].excluded.size(), error_rate) + normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][i].included.size(), 1.0 - error_rate);

      graph_degree_dist_sorted[g] = graph_degree_dist[g];
      sort(graph_degree_dist_sorted[g].rbegin(), graph_degree_dist_sorted[g].rend(), [&](auto &a, auto &b){return a.mean > b.mean;});

      for (int i = 0; i < graph_num; i++) update_score(g, i);
      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        if (score > best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        for (int i = 0; i < graph_size; i++) {
          if (prev_edge_info[i] && graph_edges[g][v].is_excluded(i)) {
            graph_edges[g][v].include_element(i);
            graph_edges[g][i].include_element(v);
          }
          else if (!prev_edge_info[i] && graph_edges[g][v].is_included(i)) {
            graph_edges[g][v].exclude_element(i);
            graph_edges[g][i].exclude_element(v);
          }
        }
        
        for (int i = 0; i < graph_size; i++) graph_degree_dist[g][i] = normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][i].excluded.size(), error_rate) + normal_prob_distribution::approximation_of_binary_distribution(graph_edges[g][i].included.size(), 1.0 - error_rate);

        graph_degree_dist_sorted[g] = graph_degree_dist[g];
        sort(graph_degree_dist_sorted[g].rbegin(), graph_degree_dist_sorted[g].rend(), [&](auto &a, auto &b){return a.mean > b.mean;});

        for (int i = 0; i < graph_num; i++) update_score(g, i);
        score = last_score;
      }
    }

    // temperature *= 1.0 - decay_rate;
    temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
    iter_count++;
  }

  vector<vector<int>> graph_degree(graph_num, vector<int>(graph_size));
  for (int i = 0; i < graph_num; i++) {
    for (int j = 0; j < graph_size; j++) graph_degree[i][j] = graph_edges[i][j].included.size();
    sort(graph_degree[i].rbegin(), graph_degree[i].rend());
  }
  sort(graph_degree.rbegin(), graph_degree.rend());
  for (auto &x: graph_degree) {
    for (auto &y: x) cerr << setw(2) << y << " ";
    cerr << endl;
  }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  init();
  solve();

  return 0;
}
