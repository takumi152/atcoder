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
#include <bitset>
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

// hyper parameters

// structs
struct point {
  int x;
  int y;

  point(): x(0), y(0) {}

  point(int x, int y): x(x), y(y) {}

  bool operator==(const point &rhs) const {
    return this->x == rhs.x && this->y == rhs.y;
  }

  point operator+(const point &rhs) const {
    point res = *this;
    res.x += rhs.x;
    res.y += rhs.y;
    return res;
  }

  point operator-(const point &rhs) const {
    point res = *this;
    res.x -= rhs.x;
    res.y -= rhs.y;
    return res;
  }
};

struct line {
  point start;
  point end;

  line(): start(point(0, 0)), end(point(1, 1)) {}

  line(point start, point end): start(start), end(end) {}

  int cross_product(const point &target) const {
    point a = this->end - this->start;
    point b = target - this->start;
    return a.x * b.y - b.x * a.y;
  }
};

// constants
constexpr int cut_limit = 100;
constexpr int group_type_count = 10;

// inputs
int n;
vector<int> group_count_target;
vector<point> berry_position;

// outputs
vector<line> line_to_cut;

// internals
vector<int> group_count;
vector<bitset<cut_limit> > cut_group;
unordered_map<bitset<cut_limit>, int> group_map;

void get_input() {
  int _k;
  cin >> n >> _k;
  group_count_target = vector<int>(group_type_count+1);
  for (int i = 1; i <= 10; i++) cin >> group_count_target[i];
  berry_position = vector<point>(n);
  for (auto &pos: berry_position) cin >> pos.x >> pos.y;
}

void output_ans() {
  assert((int) line_to_cut.size() <= cut_limit);
  cout << line_to_cut.size() << endl;
  for (auto &l: line_to_cut) {
    cout << l.start.x << " " << l.start.y << " " << l.end.x << " " << l.end.y << endl;
  }
}

void init() {
  line_to_cut = vector<line>(cut_limit);
  group_count = vector<int>(n+1);
  cut_group = vector<bitset<cut_limit> >(n);
}

void solve() {
  auto calc_score = [&]() {
    for (int j = 0; j < cut_limit; j++) {
      for (int i = 0; i < n; i++) {
        cut_group[i][j] = line_to_cut[j].cross_product(berry_position[i]) > 0;
      }
    }

    group_map.clear();
    group_count = vector<int>(n+1);
    for (int i = 0; i < n; i++) {
      group_map[cut_group[i]]++;
    }
    for (auto &[_, cnt]: group_map) {
      group_count[cnt]++;
    }

    int score = 0;
    for (int i = 1; i <= 10; i++) {
      score += min(group_count[i], group_count_target[i]);
    }
    return score;
  };
  auto calc_score_delta = [&](int j) {
    for (int i = 0; i < n; i++) {
      cut_group[i][j] = line_to_cut[j].cross_product(berry_position[i]) > 0;
    }

    group_map.clear();
    group_count = vector<int>(n+1);
    for (int i = 0; i < n; i++) {
      group_map[cut_group[i]]++;
    }
    for (auto &[_, cnt]: group_map) {
      group_count[cnt]++;
    }

    int score = 0;
    for (int i = 1; i <= 10; i++) {
      score += min(group_count[i], group_count_target[i]);
    }
    return score;
  };
  auto calc_score_delta_x = [&](int j, int x_begin, int x_end) {
    int i_begin = distance(berry_position.begin(), lower_bound(berry_position.begin(), berry_position.end(), point(x_begin, 0), [](auto &a, auto &b){return a.x < b.x;}));
    int i_end = distance(berry_position.begin(), upper_bound(berry_position.begin(), berry_position.end(), point(x_end, 0), [](auto &a, auto &b){return a.x < b.x;}));
    for (int i = i_begin; i < i_end; i++) {
      auto &map_ref_before = group_map[cut_group[i]];
      group_count[map_ref_before]--;
      map_ref_before--;
      group_count[map_ref_before]++;

      cut_group[i][j] = line_to_cut[j].cross_product(berry_position[i]) > 0;

      auto &map_ref_after = group_map[cut_group[i]];
      group_count[map_ref_after]--;
      map_ref_after++;
      group_count[map_ref_after]++;
    }

    int score = 0;
    for (int i = 1; i <= 10; i++) {
      score += min(group_count[i], group_count_target[i]);
    }
    return score;
  };

  sort(berry_position.begin(), berry_position.end(), [](auto &a, auto &b){return a.x < b.x;});
  {
    for (int i = 0; i < 7; i++) {
      line_to_cut[i].start.x = -15000;
      line_to_cut[i].start.y = -7500 + 2500 * i;
      line_to_cut[i].end.x = 15000;
      line_to_cut[i].end.y = -7499 + 2500 * i;
    }
    int cut_idx = 7;
    int berry_idx = 0;
    vector<int> subgroup_count(9);
    while (berry_idx < n && cut_idx < cut_limit) {
      subgroup_count[(berry_position[berry_idx].y + 10001) / 2500]++;
      if (subgroup_count[(berry_position[berry_idx].y + 10001) / 2500] > 10) {
        line_to_cut[cut_idx].start.x = berry_position[berry_idx].x - 1;
        line_to_cut[cut_idx].start.y = -15000;
        line_to_cut[cut_idx].end.x = berry_position[berry_idx].x;
        line_to_cut[cut_idx].end.y = 15000;
        subgroup_count = vector<int>(9);
        cut_idx++;
      }
      else {
        berry_idx++;
      }
    }
    while (cut_idx < cut_limit) {
      line_to_cut[cut_idx].start.x = 10000;
      line_to_cut[cut_idx].start.y = -15000;
      line_to_cut[cut_idx].end.x = 10001;
      line_to_cut[cut_idx].end.y = 15000;
      cut_idx++;
    }
  }

  output_ans();

  {
    int score = calc_score();
    int last_score = score;
    int best_score = score;

    const double base_temperature = 1e0;
    const double target_temperature = 5e-2;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    double time_start = theTimer.time();
    const double time_limit = 2.500;
    int iter_count = 0;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 0.90) {
        int i = theRandom.nextUIntMod(cut_limit - 7) + 7;

        line prev_cut = line_to_cut[i];
        int x = prev_cut.start.x + theRandom.nextUIntMod(400) - 200;
        if (abs(x) > 10000) continue;

        line next_cut = prev_cut;
        next_cut.start.x = x;
        next_cut.start.y = -15000;
        next_cut.end.x = x + 1;
        next_cut.end.y = 15000;
        line_to_cut[i] = next_cut;

        score = calc_score_delta_x(i, min(prev_cut.start.x, x), max(prev_cut.start.x, x));

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            output_ans();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          line_to_cut[i] = prev_cut;
          calc_score_delta(i);
          score = last_score;
        }
      }
      else if (roll < 1.00) {
        int i = theRandom.nextUIntMod(cut_limit - 7) + 7;

        line prev_cut = line_to_cut[i];
        int x = theRandom.nextUIntMod(20001) - 10000;

        line next_cut = prev_cut;
        next_cut.start.x = x;
        next_cut.start.y = -15000;
        next_cut.end.x = x + 1;
        next_cut.end.y = 15000;
        line_to_cut[i] = next_cut;

        score = calc_score_delta_x(i, min(prev_cut.start.x, x), max(prev_cut.start.x, x));

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            output_ans();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          line_to_cut[i] = prev_cut;
          calc_score_delta(i);
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

  {
    int score = calc_score();
    int last_score = score;
    int best_score = score;

    const double base_temperature = 1e-2;
    const double target_temperature = 1e-3;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    double time_start = theTimer.time();
    const double time_limit = 2.980;
    int iter_count = 0;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 1.00) {
        int i = theRandom.nextUIntMod(cut_limit);

        line prev_cut = line_to_cut[i];
        int start_x = prev_cut.start.x + theRandom.nextUIntMod(400) - 200;
        int start_y = prev_cut.start.y + theRandom.nextUIntMod(400) - 200;
        int end_x = prev_cut.end.x + theRandom.nextUIntMod(400) - 200;
        int end_y = prev_cut.end.y + theRandom.nextUIntMod(400) - 200;

        line next_cut;
        next_cut.start.x = start_x;
        next_cut.start.y = start_y;
        next_cut.end.x = end_x;
        next_cut.end.y = end_y;

        line_to_cut[i] = next_cut;

        score = calc_score_delta(i);

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            output_ans();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          line_to_cut[i] = prev_cut;
          calc_score_delta(i);
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

  int final_score = calc_score();
  int max_score = 0;
  for (int i = 1; i <= 10; i++) max_score += group_count_target[i];
  cerr << "score = " << (final_score * 1000000) / max_score << endl;
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
