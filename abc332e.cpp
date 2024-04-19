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
        //return __rdtsc() * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
        return __rdtsc() * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C6i (Xeon Platinum 8375C)
        // return __rdtsc() * 4.3478260869565215e-10; // 1 / 2.3e9, for yukicoder judge
      #endif
    #else
      unsigned long long l, h;
        __asm__ ("rdtsc" : "=a"(l), "=d"(h));
      #ifdef LOCAL
        return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        //return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
        return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C6i (Xeon Platinum 8375C)
        // return (double)(l | h << 32) * 4.3478260869565215e-10; // 1 / 2.3e9, for yukicoder judge
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

// enums

// constants

// inputs
int bag_num, item_num;
vector<double> item_weights;

// outputs
double ans;

// environment
double weight_average;

// state
vector<int> item_to_bag_assignment;

// score
double bag_weight_variance;

void get_first_input() {
  cin >> item_num >> bag_num;
  item_weights = vector<double>(item_num);
  for (auto &x: item_weights) cin >> x;
}

void init() {
  weight_average = 0.0;
  for (int i = 0; i < item_num; i++) weight_average += item_weights[i];
  weight_average /= bag_num;

  item_to_bag_assignment = vector<int>(item_num);
}

void update_score_full() {
  bag_weight_variance = 0.0;
  static vector<double> weight_sum(bag_num);
  for (int i = 0; i < bag_num; i++) weight_sum[i] = 0;
  for (int i = 0; i < item_num; i++) {
    weight_sum[item_to_bag_assignment[i]] += item_weights[i];
  }
  for (int i = 0; i < bag_num; i++) {
    bag_weight_variance += ((double) weight_sum[i] - weight_average) * ((double) weight_sum[i] - weight_average);
  }
  bag_weight_variance /= bag_num;
}

double get_score() {
  return bag_weight_variance;
}

void solve() {
  update_score_full();

  double score = get_score();
  double last_score = score;
  double best_score = score;

  const double base_temperature = 1e18;
  const double target_temperature = 1e-3;
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;

  int iter_count = 0;
  double time_start = theTimer.time();
  const double time_limit = 1.980;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.50) {
      int i = theRandom.nextUIntMod(item_num);
      int b = theRandom.nextUIntMod(bag_num);

      int orig_b = item_to_bag_assignment[i];
      item_to_bag_assignment[i] = b;

      update_score_full();

      score = get_score();

      #ifdef DEBUG
      if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
        item_to_bag_assignment[i] = orig_b;
        score = last_score;
      }
    }
    else if (roll < 1.00) {
      int i1 = theRandom.nextUIntMod(item_num);
      int i2 = theRandom.nextUIntMod(item_num);
      if (i1 == i2) continue;

      swap(item_to_bag_assignment[i1], item_to_bag_assignment[i2]);

      update_score_full();

      score = get_score();

      #ifdef DEBUG
      if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
        swap(item_to_bag_assignment[i1], item_to_bag_assignment[i2]);
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

  ans = best_score;
}

void output_ans() {
  cout << setprecision(20) << ans << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  get_first_input();

  init();
  solve();

  output_ans();

  return 0;
}
