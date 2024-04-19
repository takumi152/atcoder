#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <fstream>
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

struct edge_info {
  int from;
  int to;
  int weight;

  edge_info() = default;
  edge_info(int from, int to, int weight) : from(from), to(to), weight(weight) {}
};

struct edge {
  int id;
  int to;
  int weight;

  edge(int id, int to, int weight) : id(id), to(to), weight(weight) {}
};

// enums

// constants
const vector<Pii> position_to_search_from = {
  Pii( 500, 1000),
  Pii(1000,  500),
  Pii( 500,    0),
  Pii(   0,  500),
};

// inputs
int vertex_count;
int edge_count;
int total_days;
int max_repair_per_day;
vector<edge_info> edge_infos;
vector<vector<edge>> graph;
vector<Pii> vertex_position;

// outputs
vector<int> date_of_edge_repair;

// internals
vector<int> vertex_to_search_from;
vector<vector<vector<ll>>> distance_of_day_from_to;
vector<index_set> edge_to_repair_for_day;

void get_input() {
  cin >> vertex_count >> edge_count >> total_days >> max_repair_per_day;
  
  edge_infos = vector<edge_info>(edge_count);
  graph = vector<vector<edge>>(vertex_count);
  for (int i = 0; i < edge_count; i++) {
    int from, to, weight;
    cin >> from >> to >> weight;
    edge_infos[i] = edge_info(from-1, to-1, weight);
    graph[from-1].push_back(edge(i, to-1, weight));
    graph[to-1].push_back(edge(i, from-1, weight));
  }

  vertex_position = vector<Pii>(vertex_count);
  for (int i = 0; i < vertex_count; i++) {
    cin >> vertex_position[i].first >> vertex_position[i].second;
  }
}

void calculate_distance_of_day(int day) {
  for (auto &v_start: vertex_to_search_from) {
    for (int v_target = 0; v_target < vertex_count; v_target++) {
      distance_of_day_from_to[day][v_start][v_target] = (ll)1e9;
    }
    priority_queue<Pii, vector<Pii>, greater<Pii>> que;
    que.emplace(0, v_start);
    while (!que.empty()) {
      auto [v_dist, v_now] = que.top();
      que.pop();
      if (v_dist >= distance_of_day_from_to[day][v_start][v_now]) continue;
      distance_of_day_from_to[day][v_start][v_now] = v_dist;
      for (auto &edge_next: graph[v_now]) {
        if (date_of_edge_repair[edge_next.id] != day && v_dist + edge_next.weight < distance_of_day_from_to[day][v_start][edge_next.to]) {
          que.emplace(v_dist + edge_next.weight, edge_next.to);
        }
      }
    }
  }
}

void init() {
  date_of_edge_repair = vector<int>(edge_count);
  for (int i = 0; i < edge_count; i++) {
    date_of_edge_repair[i] = (i % total_days) + 1;
  }
  // ifstream init_ans_file("0000.txt");
  // for (int i = 0; i < edge_count; i++) {
  //   int day;
  //   init_ans_file >> day;
  //   date_of_edge_repair[i] = day;
  // }

  {
    unordered_set<int> vertex_set_to_search_from;
    for (auto &[px, py]: position_to_search_from) {
      int nearest_vertex = 0;
      double nearest_distance = 1e300;
      for (int v = 0; v < vertex_count; v++) {
        int dist = (vertex_position[v].first - px) * (vertex_position[v].first - px) + (vertex_position[v].second - py) * (vertex_position[v].second - py);
        if (dist < nearest_distance) {
          nearest_vertex = v;
          nearest_distance = dist;
        }
      }
      vertex_set_to_search_from.insert(nearest_vertex);
    }
    for (auto &v: vertex_set_to_search_from) {
      vertex_to_search_from.push_back(v);
    }
  }
  distance_of_day_from_to = vector<vector<vector<ll>>>(total_days + 1, vector<vector<ll>>(vertex_count, vector<ll>(vertex_count)));
  edge_to_repair_for_day = vector<index_set>(total_days + 1, index_set(edge_count));
  for (int i = 0; i < edge_count; i++) {
    int repair_day = date_of_edge_repair[i];
    edge_to_repair_for_day[repair_day].include_element(i);
  }
  for (int i = 0; i <= total_days; i++) {
    calculate_distance_of_day(i);
  }
}

void solve_random() {
  auto euclid_distance = [&](int v1, int v2) {
    return sqrt((vertex_position[v1].first - vertex_position[v2].first) * (vertex_position[v1].first - vertex_position[v2].first) + (vertex_position[v1].second - vertex_position[v2].second) * (vertex_position[v1].second - vertex_position[v2].second));
  };

  auto a_star_search = [&](int v_start, int v_target) {
    const double h_factor = 1.2;
    vector<int> shortest_distance(vertex_count, (int) 1e9);
    vector<int> prev_edge(vertex_count);
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<tuple<int, int, int, int>>> que;
    que.emplace(euclid_distance(v_start, v_target) * h_factor, 0, v_start, -1);
    while (!que.empty()) {
      auto [h_dist, g_dist, v_now, last_edge] = que.top();
      que.pop();
      if (h_dist >= shortest_distance[v_now]) continue;
      shortest_distance[v_now] = h_dist;
      prev_edge[v_now] = last_edge;
      if (v_now == v_target) break;
      for (auto &e: graph[v_now]) {
        int g_dist_next = g_dist + e.weight;
        int h_dist_next = g_dist_next + euclid_distance(e.to, v_target) * h_factor;
        if (h_dist_next < shortest_distance[e.to]) {
          que.emplace(h_dist_next, g_dist_next, e.to, e.id);
        }
      }
    }
    vector<int> edge_in_path;
    {
      int v_now = v_target;
      while (v_now != v_start) {
        edge_in_path.push_back(prev_edge[v_now]);
        v_now = edge_infos[prev_edge[v_now]].from == v_now ? edge_infos[prev_edge[v_now]].to : edge_infos[prev_edge[v_now]].from;
      }
    }
    return edge_in_path;
  };

  const double time_limit = 0.200;
  int iter_count = 0;
  while (theTimer.time() < time_limit) {
    int next_repair_day = theRandom.nextUIntMod(total_days) + 1;
    int v_start = theRandom.nextUIntMod(vertex_count);
    int v_target = theRandom.nextUIntMod(vertex_count);
    if (v_start == v_target) continue;

    auto edge_in_path = a_star_search(v_start, v_target);
    vector<int> prev_days;
    for (auto &e: edge_in_path) {
      int prev_repair_day = date_of_edge_repair[e];
      prev_days.push_back(prev_repair_day);
      date_of_edge_repair[e] = next_repair_day;
      edge_to_repair_for_day[prev_repair_day].exclude_element(e);
      edge_to_repair_for_day[next_repair_day].include_element(e);
    }
    
    if (edge_to_repair_for_day[next_repair_day].included_size() > max_repair_per_day) {
      // rollback
      int i = 0;
      for (auto &e: edge_in_path) {
        int prev_repair_day = prev_days[i];
        edge_to_repair_for_day[next_repair_day].exclude_element(e);
        edge_to_repair_for_day[prev_repair_day].include_element(e);
        date_of_edge_repair[e] = prev_repair_day;
        i++;
      }
    }
    iter_count++;
  }
  cerr << "iter_count   = " << iter_count << endl;

  int fix_count = 0;
  while (true) {
    bool recheck_needed = false;
    for (int d = 1; d <= total_days; d++) {
      unionfind uf(vertex_count);
      for (int v = 0; v < vertex_count; v++) {
        for (auto &e: graph[v]) {
          if (edge_to_repair_for_day[d].is_excluded(e.id)) {
            uf.unite(v, e.to);
          }
        }
      }
      for (int v = 0; v < vertex_count; v++) {
        if (!uf.same(0, v)) {
          recheck_needed = true;
          for (int t = 0; t < 5; t++) {
            int edge_idx = theRandom.nextUIntMod(graph[v].size());
            auto &e = graph[v][edge_idx];
            if (!uf.same(v, e.to)) {
              int next_day;
              do {
                next_day = theRandom.nextUIntMod(total_days) + 1;
              } while (next_day == d || edge_to_repair_for_day[next_day].included_size() >= max_repair_per_day);
              edge_to_repair_for_day[d].exclude_element(e.id);
              edge_to_repair_for_day[next_day].include_element(e.id);
              date_of_edge_repair[e.id] = next_day;
              uf.unite(v, e.to);
              fix_count++;
            }
          }
        }
      }
    }

    if (!recheck_needed) break;
  }
  cerr << "fix_count    = " << fix_count << endl;
  cerr << "time_elapsed = " << theTimer.time() << endl;
}

void solve_sa() {
  for (int i = 1; i <= total_days; i++) {
    calculate_distance_of_day(i);
  }

  auto calc_score = [&]() {
    double score_raw = 0.0;
    for (int d = 1; d <= total_days; d++) {
      double day_score = 0.0;
      for (auto &i: vertex_to_search_from) {
        for (int j = 0; j < vertex_count; j++) {
          day_score += distance_of_day_from_to[d][i][j] - distance_of_day_from_to[0][i][j];
        }
      }
      score_raw += day_score / (vertex_to_search_from.size() * (vertex_count - 1));
    }
    double score = 1000.0 * (score_raw / total_days);
    return score;
  };

  double score = calc_score();
  double last_score = score;
  double best_score = score;

  const double base_temperature = 1e2;
  const double target_temperature = 1e0;
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;

  double time_start = theTimer.time();
  const double time_limit = 5.900;
  int iter_count = 0;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.50) {
      int day1 = theRandom.nextUIntMod(total_days) + 1;
      int day2 = theRandom.nextUIntMod(total_days) + 1;
      if (day1 == day2) continue;
      if (edge_to_repair_for_day[day1].included.empty()) continue;
      if ((int) edge_to_repair_for_day[day2].included_size() >= max_repair_per_day) continue;
      int idx1 = theRandom.nextUIntMod(edge_to_repair_for_day[day1].included_size());

      int id1 = edge_to_repair_for_day[day1].included[idx1];
      edge_to_repair_for_day[day1].exclude_element(id1);
      edge_to_repair_for_day[day2].include_element(id1);
      date_of_edge_repair[id1] = day2;
      calculate_distance_of_day(day1);
      calculate_distance_of_day(day2);

      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= last_score) {
        last_score = score;
        if (score < best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        edge_to_repair_for_day[day1].include_element(id1);
        edge_to_repair_for_day[day2].exclude_element(id1);
        date_of_edge_repair[id1] = day1;
        calculate_distance_of_day(day1);
        calculate_distance_of_day(day2);
        score = last_score;
      }
    }
    else if (roll < 1.00) {
      int day1 = theRandom.nextUIntMod(total_days) + 1;
      int day2 = theRandom.nextUIntMod(total_days) + 1;
      if (day1 == day2) continue;
      if (edge_to_repair_for_day[day1].included.empty() || edge_to_repair_for_day[day2].included.empty()) continue;
      int idx1 = theRandom.nextUIntMod(edge_to_repair_for_day[day1].included_size());
      int idx2 = theRandom.nextUIntMod(edge_to_repair_for_day[day2].included_size());

      int id1 = edge_to_repair_for_day[day1].included[idx1];
      int id2 = edge_to_repair_for_day[day2].included[idx2];
      edge_to_repair_for_day[day1].exclude_element(id1);
      edge_to_repair_for_day[day2].exclude_element(id2);
      edge_to_repair_for_day[day1].include_element(id2);
      edge_to_repair_for_day[day2].include_element(id1);
      swap(date_of_edge_repair[id1], date_of_edge_repair[id2]);
      auto prev_distance1 = distance_of_day_from_to[day1];
      auto prev_distance2 = distance_of_day_from_to[day2];
      calculate_distance_of_day(day1);
      calculate_distance_of_day(day2);

      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= last_score) {
        last_score = score;
        if (score < best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        edge_to_repair_for_day[day1].include_element(id1);
        edge_to_repair_for_day[day2].include_element(id2);
        edge_to_repair_for_day[day1].exclude_element(id2);
        edge_to_repair_for_day[day2].exclude_element(id1);
        swap(date_of_edge_repair[id1], date_of_edge_repair[id2]);
        calculate_distance_of_day(day1);
        calculate_distance_of_day(day2);
        score = last_score;
      }
    }
    else if (roll < 1.00) {
      int day1 = theRandom.nextUIntMod(total_days) + 1;
      int day2 = theRandom.nextUIntMod(total_days) + 1;
      int day3 = theRandom.nextUIntMod(total_days) + 1;
      if (day1 == day2 || day1 == day3 || day2 == day3) continue;
      if (edge_to_repair_for_day[day1].included.empty() || edge_to_repair_for_day[day2].included.empty() || edge_to_repair_for_day[day3].included.empty()) continue;
      int idx1 = theRandom.nextUIntMod(edge_to_repair_for_day[day1].included_size());
      int idx2 = theRandom.nextUIntMod(edge_to_repair_for_day[day2].included_size());
      int idx3 = theRandom.nextUIntMod(edge_to_repair_for_day[day3].included_size());

      int id1 = edge_to_repair_for_day[day1].included[idx1];
      int id2 = edge_to_repair_for_day[day2].included[idx2];
      int id3 = edge_to_repair_for_day[day3].included[idx3];
      edge_to_repair_for_day[day1].exclude_element(id1);
      edge_to_repair_for_day[day1].include_element(id2);
      edge_to_repair_for_day[day2].exclude_element(id2);
      edge_to_repair_for_day[day2].include_element(id3);
      edge_to_repair_for_day[day3].exclude_element(id3);
      edge_to_repair_for_day[day3].include_element(id1);
      swap(date_of_edge_repair[id1], date_of_edge_repair[id2]);
      swap(date_of_edge_repair[id2], date_of_edge_repair[id3]);
      calculate_distance_of_day(day1);
      calculate_distance_of_day(day2);
      calculate_distance_of_day(day3);

      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 1 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= last_score) {
        last_score = score;
        if (score < best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        edge_to_repair_for_day[day1].include_element(id1);
        edge_to_repair_for_day[day1].exclude_element(id2);
        edge_to_repair_for_day[day2].include_element(id2);
        edge_to_repair_for_day[day2].exclude_element(id3);
        edge_to_repair_for_day[day3].include_element(id3);
        edge_to_repair_for_day[day3].exclude_element(id1);
        swap(date_of_edge_repair[id2], date_of_edge_repair[id3]);
        swap(date_of_edge_repair[id1], date_of_edge_repair[id2]);
        calculate_distance_of_day(day1);
        calculate_distance_of_day(day2);
        calculate_distance_of_day(day3);
        score = last_score;
      }
    }

    // temperature *= 1.0 - decay_rate;
    temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
    iter_count++;
  }

  cerr << "iter_count   = " << iter_count << endl;
  cerr << "score        = " << score << endl;
  cerr << "best_score   = " << best_score << endl;
  cerr << "temperature  = " << temperature << endl;
}

void solve() {
  solve_random();
  solve_sa();
}

void output_ans() {
  for (int i = 0; i < edge_count; i++) {
    cout << date_of_edge_repair[i] << " ";
  }
  cout << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  get_input();

  init();
  solve();

  output_ans();

  return 0;
}
