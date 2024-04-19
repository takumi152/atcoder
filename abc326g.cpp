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

// enums

// constants
struct {
  vector<int> x = {0, 1, 0, -1};
  vector<int> y = {1, 0, -1, 0};
} delta;

// inputs
int skill_num;
int achievement_num;
vector<int> skill_cost;
vector<int> achievement_reward;
vector<vector<int>> achievement_requirement;

// outputs
int ans;

// state
vector<int> skill_level;

vector<int> best_skill_level;

// score
int total_skill_cost;
int total_achievement_reward;
vector<int> missing_skill_num;

void get_first_input() {
  cin >> skill_num >> achievement_num;
  skill_cost = vector<int>(skill_num);
  for (auto &x: skill_cost) cin >> x;
  achievement_reward = vector<int>(achievement_num);
  for (auto &x: achievement_reward) cin >> x;
  achievement_requirement = vector<vector<int>>(achievement_num, vector<int>(skill_num));
  for (auto &x: achievement_requirement) {
    for (auto &y: x) cin >> y;
  }
}

void init() {
  skill_level = vector<int>(skill_num, 5);
  best_skill_level = skill_level;

  missing_skill_num = vector<int>(achievement_num);
}

void update_score_full() {
  total_skill_cost = 0;
  total_achievement_reward = 0;
  for (auto &x: missing_skill_num) x = 0;

  for (int i = 0; i < skill_num; i++) {
    total_skill_cost += (skill_level[i] - 1) * skill_cost[i];
  }

  for (int i = 0; i < achievement_num; i++) {
    for (int j = 0; j < skill_num; j++) {
      if (skill_level[j] < achievement_requirement[i][j]) {
        missing_skill_num[i]++;
      }
    }
  }

  for (int i = 0; i < achievement_num; i++) {
    if (missing_skill_num[i] == 0) {
      total_achievement_reward += achievement_reward[i];
    }
  }
}

void update_score_partial(int skill_id, bool is_after_update) {
  total_skill_cost += ((skill_level[skill_id] - 1) * skill_cost[skill_id]) * (is_after_update ? 1 : -1);
  for (int i = 0; i < achievement_num; i++) {
    if (skill_level[skill_id] < achievement_requirement[i][skill_id]) {
      missing_skill_num[i] += is_after_update ? 1 : -1;
      if (missing_skill_num[i] == 0 && !is_after_update) {
        total_achievement_reward += achievement_reward[i];
      }
      else if (missing_skill_num[i] == 1 && is_after_update) {
        total_achievement_reward -= achievement_reward[i];
      }
    }
  }
}

int get_score() {
  return total_achievement_reward - total_skill_cost;
}

void solve() {
  update_score_full();

  int score = get_score();
  int last_score = score;
  int best_score = score;

  const double base_temperature = 1e8;
  const double target_temperature = 1e-1;
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;

  int iter_count = 0;
  double time_start = theTimer.time();
  const double time_limit = 1.900;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 1.00) {
      int s = theRandom.nextUIntMod(skill_num);
      int l = theRandom.nextUIntMod(5) + 1;
      if (skill_level[s] == l) continue;

      update_score_partial(s, false);

      int orig_skill_level = skill_level[s];
      skill_level[s] = l;

      update_score_partial(s, true);

      score = get_score();

      #ifdef DEBUG
      if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        if (score > best_score) {
          best_score = score;
          best_skill_level = skill_level;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        update_score_partial(s, false);
        skill_level[s] = orig_skill_level;
        update_score_partial(s, true);
        assert(get_score() == last_score);
        score = last_score;
      }
    }

    if ((iter_count & 0xffff) == 0) {
      skill_level = best_skill_level;
      update_score_full();
      assert(get_score() == best_score);
      score = get_score();
      last_score = score;
    }

    // temperature *= 1.0 - decay_rate;
    temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
    iter_count++;
  }

  cerr << "iter_count   = " << iter_count << endl;
  cerr << "score        = " << score << endl;
  cerr << "best_score   = " << best_score << endl;
  cerr << "temperature  = " << temperature << endl;

  ans = max(0, best_score);
}

void output_ans() {
  cout << ans << endl;
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
