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

// constants
constexpr int n = 1000;
constexpr int m = 50;

// inputs
vector<int> target_x(n * 2 + 1);
vector<int> target_y(n * 2 + 1);

// outputs

// internals
vector<int> initial_set;

vector<int> global_route_order;
vector<int> global_route_order_prev;
vector<int> global_route_order_best;

vector<Pii> route_position(n, Pii(-1, -1));

void output_result() {
  vector<int> deliver_set;
  for (int i = 1; i < 2 * m + 1; i++) {
    if (global_route_order[i] < 1000) deliver_set.push_back(global_route_order[i]);
  }

  cout << deliver_set.size();
  for (auto &x: deliver_set) cout << " " << x + 1;
  cout << endl;

  cout << global_route_order.size();
  for (auto &x: global_route_order) cout << " " << target_x[x] << " " << target_y[x];
  cout << endl;
}

void solve() {
  vector<int> initial_order(n);
  for (int i = 0; i < n; i++) initial_order[i] = i;
  sort(initial_order.begin(), initial_order.end(), [&](auto &a, auto &b){return abs(400 - target_x[a]) + abs(400 - target_y[a]) + abs(400 - target_x[a + 1000]) + abs(400 - target_y[a + 1000]) < abs(400 - target_x[b]) + abs(400 - target_y[b]) + abs(400 - target_x[b + 1000]) + abs(400 - target_y[b + 1000]);});

  global_route_order.push_back(n * 2);
  for (int i = 0; i < m; i++) {
    global_route_order.push_back(initial_order[i]);
    route_position[initial_order[i]].first = i + 1;
  }
  for (int i = 0; i < m; i++) {
    global_route_order.push_back(initial_order[i] + 1000);
    route_position[initial_order[i]].second = m + i + 1;
  }
  global_route_order.push_back(n * 2);

  global_route_order_prev = global_route_order;
  global_route_order_best = global_route_order;

  auto calc_score = [&]() {
    int score = 0;
    for (int i = 0; i < m * 2 + 1; i++) {
      score += abs(target_x[global_route_order[i]] - target_x[global_route_order[i + 1]]) + abs(target_y[global_route_order[i]] - target_y[global_route_order[i + 1]]);
    }
    return score;
  };

  auto update_route_position = [&]() {
    for (int i = 1; i < m * 2 + 1; i++) {
      if (global_route_order[i] < 1000) route_position[global_route_order[i]].first = i;
      else route_position[global_route_order[i] - 1000].second = i;
    }
  };

  int score = calc_score();
  int last_score = score;
  int best_score = score;

  double base_temperature = 1e2;
  double target_temperature = 5e-1;
  double temperature = base_temperature;
  // double decay_rate = 3.0e-6;
  double time_start = theTimer.time();
  double time_limit = 1.950;
  int no_update_count = 0;
  int kick_threshold = 10000;
  int best_restore_interval = 100000;
  int iter_count = 0;
  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.50) { // insert new
      int p10 = theRandom.nextUIntMod(m * 2) + 1;
      int i1 = global_route_order[p10] % 1000;
      int i2 = theRandom.nextUIntMod(n);
      if (i1 == i2 || route_position[i2].first != -1) continue;
      int p21 = theRandom.nextUIntMod(m * 2 - 1) + 1;
      int p22 = theRandom.nextUIntMod(m * 2 - 1) + 1;
      if (p21 > p22) swap(p21, p22);

      int p11 = route_position[i1].first;
      int p12 = route_position[i1].second;
      global_route_order.erase(global_route_order.begin() + p12);
      global_route_order.erase(global_route_order.begin() + p11);

      { // optimal insertion if consistent
        int p21_best = 0;
        int p21_best_dist = 10000;
        int p22_best = 0;
        int p22_best_dist = 10000;
        for (int i = 0; i < m * 2 - 1; i++) {
          int p21_dist = abs(target_x[global_route_order[i]] - target_x[i2]) + abs(target_y[global_route_order[i]] - target_y[i2]) + abs(target_x[global_route_order[i+1]] - target_x[i2]) + abs(target_y[global_route_order[i+1]] - target_y[i2]);
          if (p21_dist < p21_best_dist) {
            p21_best = i + 1;
            p21_best_dist = p21_dist;
          }
          int p22_dist = abs(target_x[global_route_order[i]] - target_x[i2 + 1000]) + abs(target_y[global_route_order[i]] - target_y[i2 + 1000]) + abs(target_x[global_route_order[i+1]] - target_x[i2 + 1000]) + abs(target_y[global_route_order[i+1]] - target_y[i2 + 1000]);
          if (p22_dist < p22_best_dist) {
            p22_best = i + 1;
            p22_best_dist = p22_dist;
          }
        }
        if (p21_best <= p22_best) {
          p21 = p21_best;
          p22 = p22_best;
        }
      }
      global_route_order.insert(global_route_order.begin() + p22, i2 + 1000);
      global_route_order.insert(global_route_order.begin() + p21, i2);

      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= last_score) {
        route_position[i1].first = -1;
        route_position[i1].second = -1;
        update_route_position();
        global_route_order_prev = global_route_order;
        no_update_count = 0;
        last_score = score;
        if (score < best_score) {
          global_route_order_best = global_route_order;
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
        route_position[i1].first = -1;
        route_position[i1].second = -1;
        update_route_position();
        global_route_order_prev = global_route_order;
        no_update_count = 0;
        last_score = score;
      }
      else { // rollback
        no_update_count++;
        if (no_update_count > kick_threshold) {
          route_position[i1].first = -1;
          route_position[i1].second = -1;
          update_route_position();
          global_route_order_prev = global_route_order;
          no_update_count = 0;
          last_score = score;
        }
        else {
          global_route_order = global_route_order_prev;
          score = last_score;
        }
      }
    }
    else if (roll < 1.00) { // move existing
      int p10 = theRandom.nextUIntMod(m * 2) + 1;
      int i1 = global_route_order[p10] % 1000;
      int p21 = theRandom.nextUIntMod(m * 2 - 1) + 1;
      int p22 = theRandom.nextUIntMod(m * 2 - 1) + 1;
      if (p21 > p22) swap(p21, p22);

      int p11 = route_position[i1].first;
      int p12 = route_position[i1].second;
      global_route_order.erase(global_route_order.begin() + p12);
      global_route_order.erase(global_route_order.begin() + p11);

      // { // optimal insertion if consistent
      //   int p21_best = 0;
      //   int p21_best_dist = 10000;
      //   int p22_best = 0;
      //   int p22_best_dist = 10000;
      //   for (int i = 0; i < m * 2 - 1; i++) {
      //     int p21_dist = abs(target_x[global_route_order[i]] - target_x[i1]) + abs(target_y[global_route_order[i]] - target_y[i1]) + abs(target_x[global_route_order[i+1]] - target_x[i1]) + abs(target_y[global_route_order[i+1]] - target_y[i1]);
      //     if (p21_dist < p21_best_dist) {
      //       p21_best = i + 1;
      //       p21_best_dist = p21_dist;
      //     }
      //     int p22_dist = abs(target_x[global_route_order[i]] - target_x[i1 + 1000]) + abs(target_y[global_route_order[i]] - target_y[i1 + 1000]) + abs(target_x[global_route_order[i+1]] - target_x[i1 + 1000]) + abs(target_y[global_route_order[i+1]] - target_y[i1 + 1000]);
      //     if (p22_dist < p22_best_dist) {
      //       p22_best = i + 1;
      //       p22_best_dist = p22_dist;
      //     }
      //   }
      //   if (p21_best <= p22_best) {
      //     p21 = p21_best;
      //     p22 = p22_best;
      //   }
      // }

      global_route_order.insert(global_route_order.begin() + p22, i1 + 1000);
      global_route_order.insert(global_route_order.begin() + p21, i1);

      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= last_score) {
        update_route_position();
        global_route_order_prev = global_route_order;
        no_update_count = 0;
        last_score = score;
        if (score < best_score) {
          global_route_order_best = global_route_order;
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
        update_route_position();
        global_route_order_prev = global_route_order;
        no_update_count = 0;
        last_score = score;
      }
      else { // rollback
        no_update_count++;
        if (no_update_count > kick_threshold) {
          update_route_position();
          global_route_order_prev = global_route_order;
          no_update_count = 0;
          last_score = score;
        }
        else {
          global_route_order = global_route_order_prev;
          score = last_score;
        }
      }
    }

    #ifdef DEBUG
    if (iter_count % 10000 == 0) output_result();
    #endif

    if (iter_count % best_restore_interval == 0) {
      global_route_order = global_route_order_best;
      route_position = vector<Pii>(n, Pii(-1, -1));
      update_route_position();
      global_route_order_prev = global_route_order;
      score = best_score;
      last_score = best_score;
    }

    iter_count++;
    // temperature *= 1.0 - decay_rate;
    // temperature = base_temperature * ((time_limit - theTimer.time()) / (time_limit - time_start));
    temperature = exp(log(base_temperature) * ((time_limit - theTimer.time()) / (time_limit - time_start)) + log(target_temperature) * (1.0 - (time_limit - theTimer.time()) / (time_limit - time_start)));
  }

  cerr << "iter_count   = " << iter_count << endl;
  cerr << "score        = " << score << endl;
  cerr << "best_score   = " << best_score << endl;
  cerr << "temperature  = " << temperature << endl;

  global_route_order = global_route_order_best;

  output_result();
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  for (int i = 0; i < n; i++) cin >> target_x[i] >> target_y[i] >> target_x[i + 1000] >> target_y[i + 1000];
  // start / end
  target_x[n * 2] = 400;
  target_y[n * 2] = 400;

  solve();

  return 0;
}
