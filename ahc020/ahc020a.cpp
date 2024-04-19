#ifdef __GNUC__
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")
#endif

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
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <random>
#include <functional>
#include <cmath>
#include <cassert>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

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
    #ifdef _MSC_VER
      #ifdef LOCAL
        return __rdtsc() * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return __rdtsc() * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return __rdtsc() * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        return __rdtsc() * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
      #endif
    #else
      unsigned long long l, h;
        __asm__ ("rdtsc" : "=a"(l), "=d"(h));
      #ifdef LOCAL
        return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
      #endif
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
struct unionfind {
  vector<int> group;
  vector<int> rank;

  unionfind() {
    unionfind(0);
  }

  unionfind(int n) {
    group = vector<int>(n);
    rank = vector<int>(n);
    for (int i = 0; i < n; i++) group[i] = i;
  }

  int root(int x) {
    if (group[x] == x) return x;
    return group[x] = root(group[x]);
  }

  void unite(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    if (rank[rx] > rank[ry]) group[ry] = rx;
    else if (rank[ry] > rank[rx]) group[rx] = ry;
    else if (rx != ry) {
      group[ry] = rx;
      rank[rx]++;
    }
  }

  bool same(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    return rx == ry;
  }
};

// enums

// constants
constexpr int antenna_num = 100;

constexpr int max_antenna_power = 5000;

// inputs
int cable_num, house_num;
vector<Pii> antenna_pos; // x, y
vector<int> cable_weight;
vector<Pii> cable_antenna_pair;
vector<vector<tuple<int, int, int>>> cable_connection; // to_antenna, weight, id
vector<Pii> house_pos; // x, y

// outputs

// internals
vector<vector<int>> distance_from_antenna_to_house;
vector<tuple<int, int, int, int>> sorted_cables; // from_antenna, to_antenna, weight, id
vector<vector<Pii>> shortest_cables_to_root; // id, weight;

vector<vector<Pii>> sorted_distance_from_antenna_to_house; // id, distance

void get_first_input() {
  int _antenna_num;
  cin >> _antenna_num >> cable_num >> house_num;
  antenna_pos = vector<Pii>(antenna_num);
  for (auto &x: antenna_pos) cin >> x.first >> x.second;
  cable_weight = vector<int>(cable_num);
  cable_antenna_pair = vector<Pii>(cable_num);
  cable_connection = vector<vector<tuple<int, int, int>>>(antenna_num);
  for (int i = 0; i < cable_num; i++) {
    int a, b, w;
    cin >> a >> b >> w;
    cable_weight[i] = w;
    cable_antenna_pair[i] = Pii(a-1, b-1);
    cable_connection[a-1].emplace_back(b-1, w, i);
    cable_connection[b-1].emplace_back(a-1, w, i);
  }
  house_pos = vector<Pii>(house_num);
  for (auto &x: house_pos) cin >> x.first >> x.second;
}

void init() {
  distance_from_antenna_to_house = vector<vector<int>>(antenna_num, vector<int>(house_num));
  for (int i = 0; i < antenna_num; i++) {
    for (int j = 0; j < house_num; j++) {
      distance_from_antenna_to_house[i][j] = ceil(sqrt((antenna_pos[i].first - house_pos[j].first) * (antenna_pos[i].first - house_pos[j].first) + (antenna_pos[i].second - house_pos[j].second) * (antenna_pos[i].second - house_pos[j].second)));
    }
  }
  sorted_cables = vector<tuple<int, int, int, int>>();
  for (int i = 0; i < antenna_num; i++) {
    for (auto &[j, w, id]: cable_connection[i]) {
      if (i < j) {
        sorted_cables.emplace_back(i, j, w, id);
      }
    }
  }
  sort(sorted_cables.begin(), sorted_cables.end(), [&](const auto &a, const auto &b){return get<2>(a) < get<2>(b);});
  shortest_cables_to_root = vector<vector<Pii>>(antenna_num);
  for (int i = 0; i < antenna_num; i++) {
    vector<bool> visited(antenna_num);
    vector<int> prev_antenna(antenna_num, -1);
    vector<int> prev_cable(antenna_num, -1);
    vector<int> prev_weight(antenna_num, -1);
    priority_queue<tuple<int, int, int, int, int>, vector<tuple<int, int, int, int, int>>, greater<tuple<int, int, int, int, int>>> que; // cost, now, prev_antenna, prev_cable, prev_weight
    que.emplace(0, i, -1, -1, -1);
    while (!que.empty()) {
      auto [cost, now, prev_a, prev_c, prev_w] = que.top();
      que.pop();
      if (visited[now]) continue;
      visited[now] = true;
      prev_antenna[now] = prev_a;
      prev_cable[now] = prev_c;
      prev_weight[now] = prev_w;
      if (now == 0) break;
      for (auto &[next_a, weight, next_c]: cable_connection[now]) {
        if (!visited[next_a]) {
          que.emplace(cost + weight, next_a, now, next_c, weight);
        }
      }
    }
    int now_a = 0;
    while (now_a != i) {
      shortest_cables_to_root[i].emplace_back(prev_cable[now_a], prev_weight[now_a]);
      now_a = prev_antenna[now_a];
    }
  }

  sorted_distance_from_antenna_to_house = vector<vector<Pii>>(antenna_num, vector<Pii>(house_num));
  for (int i = 0; i < antenna_num; i++) {
    for (int j = 0; j < house_num; j++) {
      sorted_distance_from_antenna_to_house[i][j] = Pii(j, distance_from_antenna_to_house[i][j]);
    }
    sort(sorted_distance_from_antenna_to_house[i].begin(), sorted_distance_from_antenna_to_house[i].end(), [&](const auto &a, const auto &b){return a.second < b.second;});
  }
}

void solve() {
  vector<int> antenna_power(antenna_num);
  {
    for (int j = 0; j < house_num; j++) {
      int best_antenna = 0;
      int best_distance = 999999;
      for (int i = 0; i < antenna_num; i++) {
        if (distance_from_antenna_to_house[i][j] < best_distance) {
          best_antenna = i;
          best_distance = distance_from_antenna_to_house[i][j];
        }
      }
      antenna_power[best_antenna] = max(antenna_power[best_antenna], best_distance);
    }
 
    vector<int> antenna_cover_num(antenna_num);
    vector<int> house_cover_num(house_num);
    for (int i = 0; i < antenna_num; i++) {
      for (int j = 0; j < house_num; j++) {
        if (antenna_power[i] >= sorted_distance_from_antenna_to_house[i][j].second) {
          antenna_cover_num[i]++;
          house_cover_num[sorted_distance_from_antenna_to_house[i][j].first]++;
        }
        else break;
      }
    }

    ll not_covered_num = 0;
    for (int i = 0; i < house_num; i++) {
      if (house_cover_num[i] == 0) not_covered_num++;
    }
 
    auto calc_score = [&]() {
      ll score = 0;
      ll antenna_used = 0;
      for (int i = 0; i < antenna_num; i++) {
        score += antenna_power[i] * antenna_power[i];
        if (antenna_power[i] > 0) antenna_used++;
      }
 
      static vector<bool> cable_used(cable_num);
      for (int i = 0; i < cable_num; i++) cable_used[i] = false;
      // for (int i = 0; i < antenna_num; i++) {
      //   if (antenna_power[i] > 0) {
      //     for (auto &[c_id, c_weight]: shortest_cables_to_root[i]) {
      //       if (!cable_used[c_id]) {
      //         cable_used[c_id] = true;
      //         score += c_weight;
      //       }
      //     }
      //   }
      // }
      static unionfind uf(cable_num);
      for (int i = 0; i < cable_num; i++) uf.group[i] = i;
      for (int i = 0; i < cable_num; i++) uf.rank[i] = 0;
      ll powered_cable = 0;
      for (auto &[a1, a2, c_weight, c_id]: sorted_cables) {
        if ((a1 == 0 || antenna_power[a1] > 0) && antenna_power[a2] > 0 && !uf.same(a1, a2)) {
          cable_used[c_id] = true;
          uf.unite(a1, a2);
          powered_cable++;
          score += c_weight;
        }
      }

      score += not_covered_num * (ll) 1e9;
      score += ((antenna_used + (antenna_power[0] == 0 ? 1 : 0)) - (powered_cable + 1)) * (ll) 1e9;
 
      return score;
    };
 
    auto output_ans = [&]() {
      static vector<bool> cable_used(cable_num);
      for (int i = 0; i < cable_num; i++) cable_used[i] = false;
      // for (int i = 0; i < antenna_num; i++) {
      //   if (antenna_power[i] > 0) {
      //     for (auto &[c_id, _]: shortest_cables_to_root[i]) {
      //       if (!cable_used[c_id]) {
      //         cable_used[c_id] = true;
      //       }
      //     }
      //   }
      // }
      static unionfind uf(cable_num);
      for (int i = 0; i < cable_num; i++) uf.group[i] = i;
      for (int i = 0; i < cable_num; i++) uf.rank[i] = 0;
      for (auto &[a1, a2, _, c_id]: sorted_cables) {
        if ((a1 == 0 || antenna_power[a1] > 0) && antenna_power[a2] > 0 && !uf.same(a1, a2)) {
          cable_used[c_id] = true;
          uf.unite(a1, a2);
        }
      }
 
      for (int i = 0; i < antenna_num; i++) {
        cout << antenna_power[i] << " ";
      }
      cout << endl;
      for (int i = 0; i < cable_num; i++) {
        cout << cable_used[i] << " ";
      }
      cout << endl;
    };

    ll score = calc_score();
    ll last_score = score;
    ll best_score = score;

    const double base_temperature = 2e6;
    const double target_temperature = 1e4;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    const int range_start = 500;
    const int range_end = 10;
    int range = range_start;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.900;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 0.40) {
        int a = theRandom.nextUIntMod(antenna_num);
        if (antenna_power[a] == 0) continue;
        int p = antenna_power[a] - (theRandom.nextUIntMod(range) + 1);
        if (p < 0) p = 0;

        int p_orig = antenna_power[a];
        while (antenna_cover_num[a] > 0) {
          if (p < sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]-1].second) {
            house_cover_num[sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]-1].first]--;
            if (house_cover_num[sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]-1].first] == 0) not_covered_num++;
            antenna_cover_num[a]--;
          }
          else {
            break;
          }
        }
        antenna_power[a] = p;

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << range << " " << theTimer.time() << endl;
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
          while (sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]].second <= p_orig) {
            house_cover_num[sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]].first]++;
            if (house_cover_num[sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]].first] == 1) not_covered_num--;
            antenna_cover_num[a]++;
          }
          antenna_power[a] = p_orig;
          score = last_score;
        }
      }
      else if (roll < 0.80) {
        int a = theRandom.nextUIntMod(antenna_num);
        if (antenna_power[a] == max_antenna_power) continue;
        int p = antenna_power[a] + (theRandom.nextUIntMod(range) + 1);
        if (p > max_antenna_power) p = max_antenna_power;

        int p_orig = antenna_power[a];
        while (sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]].second <= p) {
          house_cover_num[sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]].first]++;
          if (house_cover_num[sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]].first] == 1) not_covered_num--;
          antenna_cover_num[a]++;
        }
        antenna_power[a] = p;

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << range << " " << theTimer.time() << endl;
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
          while (antenna_cover_num[a] > 0 && p_orig < sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]-1].second) {
            house_cover_num[sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]-1].first]--;
            if (house_cover_num[sorted_distance_from_antenna_to_house[a][antenna_cover_num[a]-1].first] == 0) not_covered_num++;
            antenna_cover_num[a]--;
          }
          antenna_power[a] = p_orig;
          score = last_score;
        }
      }
      else if (roll < 1.00) {
        int a1 = theRandom.nextUIntMod(antenna_num);
        if (antenna_power[a1] == 0) continue;
        int p1 = antenna_power[a1] - (theRandom.nextUIntMod(range) + 1);
        if (p1 < 0) p1 = 0;
        int a2 = theRandom.nextUIntMod(antenna_num);
        if (antenna_power[a2] == max_antenna_power) continue;
        int p2 = antenna_power[a2] + (theRandom.nextUIntMod(range) + 1);
        if (p2 > max_antenna_power) p2 = max_antenna_power;
        if (a1 == a2) continue;

        int p1_orig = antenna_power[a1];
        while (antenna_cover_num[a1] > 0) {
          if (p1 < sorted_distance_from_antenna_to_house[a1][antenna_cover_num[a1]-1].second) {
            house_cover_num[sorted_distance_from_antenna_to_house[a1][antenna_cover_num[a1]-1].first]--;
            if (house_cover_num[sorted_distance_from_antenna_to_house[a1][antenna_cover_num[a1]-1].first] == 0) not_covered_num++;
            antenna_cover_num[a1]--;
          }
          else {
            break;
          }
        }
        antenna_power[a1] = p1;
        int p2_orig = antenna_power[a2];
        while (sorted_distance_from_antenna_to_house[a2][antenna_cover_num[a2]].second <= p2) {
          house_cover_num[sorted_distance_from_antenna_to_house[a2][antenna_cover_num[a2]].first]++;
          if (house_cover_num[sorted_distance_from_antenna_to_house[a2][antenna_cover_num[a2]].first] == 1) not_covered_num--;
          antenna_cover_num[a2]++;
        }
        antenna_power[a2] = p2;

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << range << " " << theTimer.time() << endl;
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
          while (sorted_distance_from_antenna_to_house[a1][antenna_cover_num[a1]].second <= p1_orig) {
            house_cover_num[sorted_distance_from_antenna_to_house[a1][antenna_cover_num[a1]].first]++;
            if (house_cover_num[sorted_distance_from_antenna_to_house[a1][antenna_cover_num[a1]].first] == 1) not_covered_num--;
            antenna_cover_num[a1]++;
          }
          antenna_power[a1] = p1_orig;
          while (antenna_cover_num[a2] > 0 && p2_orig < sorted_distance_from_antenna_to_house[a2][antenna_cover_num[a2]-1].second) {
            house_cover_num[sorted_distance_from_antenna_to_house[a2][antenna_cover_num[a2]-1].first]--;
            if (house_cover_num[sorted_distance_from_antenna_to_house[a2][antenna_cover_num[a2]-1].first] == 0) not_covered_num++;
            antenna_cover_num[a2]--;
          }
          antenna_power[a2] = p2_orig;
          score = last_score;
        }
      }

      // temperature *= 1.0 - decay_rate;
      temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      // range = (int) exp(log(range_start) - ((log(range_start) - log(range_end)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      range = (int) ((double) range_start * (1.0 - (theTimer.time() - time_start) * (1.0 / (time_limit - time_start))) + (double) range_end * (theTimer.time() - time_start) * (1.0 / (time_limit - time_start)));
      iter_count++;
    }

    cerr << "iter_count   = " << iter_count << endl;
    cerr << "score        = " << score << endl;
    cerr << "best_score   = " << best_score << endl;
    cerr << "temperature  = " << temperature << endl;

    output_ans();
  }

  // vector<bool> cable_to_use(cable_num);
  // {
  //   for (int i = 0; i < antenna_num; i++) {
  //     if (antenna_power[i] > 0) {
  //       for (auto &[c, w]: shortest_cables_to_root[i]) {
  //         if (!cable_to_use[c]) {
  //           cable_to_use[c] = true;
  //         }
  //       }
  //     }
  //   }
 
  //   auto calc_score = [&]() {
  //     ll score = 0;
  //     for (int i = 0; i < cable_num; i++) {
  //       if (cable_to_use[i]) {
  //         score += cable_weight[i];
  //       }
  //     }
 
  //     return score;
  //   };
 
  //   auto output_ans = [&]() {
  //     for (int i = 0; i < antenna_num; i++) {
  //       cout << antenna_power[i] << " ";
  //     }
  //     cout << endl;
  //     for (int i = 0; i < cable_num; i++) {
  //       cout << cable_to_use[i] << " ";
  //     }
  //     cout << endl;
  //   };
 
  //   ll score = calc_score();
  //   ll last_score = score;
  //   ll best_score = score;
 
  //   const double base_temperature = 1e6;
  //   const double target_temperature = 5e4;
  //   // const double decay_rate = 4e-5;
  //   double temperature = base_temperature;
 
  //   int iter_count = 0;
  //   double time_start = theTimer.time();
  //   const double time_limit = 1.950;
 
  //   while (theTimer.time() < time_limit) {
  //     double roll = theRandom.nextDouble();
  //     if (roll < 1.00) {
  //       int c1 = theRandom.nextUIntMod(cable_num);
  //       if (!cable_to_use[c1]) continue;
 
  //       cable_to_use[c1] = false;
 
  //       vector<int> added_cable_set;
  //       auto check_contain_powered_and_root_antenna = [&](int begin_antenna) {
  //         static vector<bool> visited(antenna_num);
  //         for (int i = 0; i < antenna_num; i++) visited[i] = false;
  //         queue<int> que;
  //         que.push(begin_antenna);
  //         while (!que.empty()) {
  //           auto now = que.front();
  //           que.pop();
  //           if (visited[now]) continue;
  //           visited[now] = true;
  //           if (antenna_power[now] > 0) return true;
  //           for (auto &[next_v, _, next_c]: cable_connection[now]) {
  //             if (cable_to_use[next_c] && !visited[next_v]) {
  //               que.push(next_v);
  //             }
  //           }
  //         }
  //         return false;
  //       };
  //       auto powered1 = check_contain_powered_and_root_antenna(cable_antenna_pair[c1].first);
  //       auto powered2 = check_contain_powered_and_root_antenna(cable_antenna_pair[c1].second);
  //       if (powered1 && powered2) {
  //         auto find_optimal_connection = [&]() {
  //           static vector<bool> visited(antenna_num);
  //           for (int i = 0; i < antenna_num; i++) visited[i] = false;
  //           static vector<int> prev_antenna(antenna_num);
  //           for (int i = 0; i < antenna_num; i++) prev_antenna[i] = -1;
  //           static vector<int> prev_cable(antenna_num);
  //           for (int i = 0; i < antenna_num; i++) prev_cable[i] = -1;
  //           int target_antenna = -1;
  //           priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<tuple<int, int, int, int>>> que;
  //           que.emplace(0, 0, -1, -1);
  //           while (!que.empty() && target_antenna == -1) {
  //             auto [cost, now, prev_a, prev_c] = que.top();
  //             que.pop();
  //             if (visited[now]) continue;
  //             visited[now] = true;
  //             prev_antenna[now] = prev_a;
  //             prev_cable[now] = prev_c;
  //             if (cost > 0 && antenna_power[now] > 0) {
  //               target_antenna = now;
  //               break;
  //             }
  //             for (auto &[next_a, weight, next_c]: cable_connection[now]) {
  //               if (!visited[next_a]) {
  //                 if (cable_to_use[next_c]) que.emplace(cost, next_a, now, next_c);
  //                 else que.emplace(cost + weight, next_a, now, next_c);
  //               }
  //             }
  //           }
  //           vector<int> cable_to_add;
  //           int now_a = target_antenna;
  //           while (now_a != 0) {
  //             if (!cable_to_use[prev_cable[now_a]]) {
  //               cable_to_add.push_back(prev_cable[now_a]);
  //             }
  //             now_a = prev_antenna[now_a];
  //           }
  //           return cable_to_add;
  //         };
  //         added_cable_set = find_optimal_connection();
  //         for (auto &c: added_cable_set) cable_to_use[c] = true;
  //       }
 
  //       auto check_all_powered_reachable = [&]() {
  //         static vector<bool> visited(antenna_num);
  //         for (int i = 0; i < antenna_num; i++) visited[i] = false;
  //         queue<int> que;
  //         que.push(0);
  //         while (!que.empty()) {
  //           auto now = que.front();
  //           que.pop();
  //           if (visited[now]) continue;
  //           visited[now] = true;
  //           for (auto &[next_a, _, next_c]: cable_connection[now]) {
  //             if (cable_to_use[next_c] && !visited[next_a]) {
  //               que.push(next_a);
  //             }
  //           }
  //         }
  //         for (int i = 0; i < antenna_num; i++) {
  //           if (antenna_power[i] > 0 && !visited[i]) {
  //             return false;
  //           }
  //         }
  //         return true;
  //       };
  //       if (!check_all_powered_reachable()) {
  //         for (auto &c: added_cable_set) cable_to_use[c] = false;
  //         cable_to_use[c1] = true;
  //         continue;
  //       }
 
  //       score = calc_score();
 
  //       #ifdef DEBUG
  //       if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
  //       #endif
 
  //       if (score <= last_score) {
  //         last_score = score;
  //         if (score < best_score) {
  //           best_score = score;
  //         }
  //       }
  //       else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
  //         last_score = score;
  //       }
  //       else { // rollback
  //         for (auto &c: added_cable_set) cable_to_use[c] = false;
  //         cable_to_use[c1] = true;
  //         score = last_score;
  //       }
  //     }
 
  //     // temperature *= 1.0 - decay_rate;
  //     temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
  //     iter_count++;
  //   }
 
  //   cerr << "iter_count   = " << iter_count << endl;
  //   cerr << "score        = " << score << endl;
  //   cerr << "best_score   = " << best_score << endl;
  //   cerr << "temperature  = " << temperature << endl;
 
  //   output_ans();
  // }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  get_first_input();

  init();
  solve();

  return 0;
}
