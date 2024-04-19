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
#include <tuple>
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

// structs / enums
struct position {
  double x;
  double y;
  position(): x(0), y(0) {};
  position(double x, double y): x(x), y(y) {};

  bool operator==(const position& that) const {
    return this->x == that.x && this->y == that.y;
  }

  bool operator!=(const position& that) const {
    return !(this->x == that.x && this->y == that.y);
  }

  position operator+(const position& that) const {
    position res;
    res.x = this->x + that.x;
    res.y = this->y + that.y;
    return res;
  }

  position operator-(const position& that) const {
    position res;
    res.x = this->x - that.x;
    res.y = this->y - that.y;
    return res;
  }

  position operator*(const int that) const {
    position res;
    res.x = this->x * that;
    res.y = this->y * that;
    return res;
  }

  position operator*(const double that) const {
    position res;
    res.x = this->x * that;
    res.y = this->y * that;
    return res;
  }

  position& operator+=(const position& that) {
    this->x += that.x;
    this->y += that.y;
    return *this;
  }

  position& operator-=(const position& that) {
    this->x -= that.x;
    this->y -= that.y;
    return *this;
  }

  position& operator*=(const int that) {
    this->x *= that;
    this->y *= that;
    return *this;
  }
  
  position& operator*=(const double that) {
    this->x *= that;
    this->y *= that;
    return *this;
  }

  double length() const
  {
    return sqrt(this->x * this->x + this->y * this->y);
  }

  static double distance_between(const position& p1, const position& p2) {
    const position delta = p2 - p1;
    return delta.length();
  }

  double distance_to(const position& target) const {
    return distance_between((*this), target);
  }

  bool within_range_of(const position& target, double range) const {
    return this->distance_to(target) <= range;
  }

  position toward_pos_abs(const position& target_pos, const double dist) const {
    if (target_pos == (*this)) return (*this); // cannot determine direction
    const auto pos_delta = target_pos - (*this);
    const auto scale = dist / pos_delta.length();
    return (*this) + pos_delta * scale;
  }

  position toward_pos_rel(const position& target_pos, const double dist) const {
    return this->toward_pos_abs(target_pos, dist) - (*this);
  }
};

// constants
constexpr double field_radius = 1e9;

// inputs
int n;

// outputs

// internals
vector<position> pos;

void get_first_input() {
  cin >> n;
}

void init() {
  pos = vector<position>(n);
  for (int i = 0; i < n; i++) {
    do {
      pos[i].x = theRandom.nextDouble() * field_radius * 2.0 - field_radius;
      pos[i].y = theRandom.nextDouble() * field_radius * 2.0 - field_radius;
    } while (pos[i].distance_to(position(0.0, 0.0)) > field_radius);
  }
}

void solve() {
  auto calc_score = [&]() {
    double score = 0.0;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        score += (field_radius / position::distance_between(pos[i], pos[j])) * (field_radius / position::distance_between(pos[i], pos[j]));
      }
    }
    return score;
  };
  auto calc_score_delta = [&](int i) {
    double score = 0.0;
    for (int j = 0; j < n; j++) {
      if (i == j) continue;
      score += (field_radius / position::distance_between(pos[i], pos[j])) * (field_radius / position::distance_between(pos[i], pos[j]));
    }
    return score;
  };

  double score = calc_score();
  double last_score = score;
  double best_score = score;

  const double base_temperature = 1e2;
  const double target_temperature = 1e-1;
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;
  const double base_delta_factor = 1e0;
  const double target_delta_factor = 1e2;
  double delta_factor = base_delta_factor;

  const double time_start = theTimer.time();
  const double time_limit = time_start + 99.900;
  int iter_count = 0;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 1.00) {
      int i = theRandom.nextUIntMod(n);
      int dx = theRandom.nextUIntMod(field_radius / delta_factor) * 2.0 - field_radius / delta_factor;
      int dy = theRandom.nextUIntMod(field_radius / delta_factor) * 2.0 - field_radius / delta_factor;
      if ((pos[i] + position(dx, dy)).distance_to(position(0.0, 0.0)) > field_radius) continue;

      score -= calc_score_delta(i);
      pos[i] += position(dx, dy);
      score += calc_score_delta(i);

      #ifdef DEBUG
      if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() - time_start << endl;
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
        pos[i] -= position(dx, dy);
        score = last_score;
      }
    }

    // temperature *= 1.0 - decay_rate;
    temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
    delta_factor = exp(log(base_delta_factor) - ((log(base_delta_factor) - log(target_delta_factor)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
    iter_count++;
  }

  cerr << "iter_count   = " << iter_count << endl;
  cerr << "score        = " << score << endl;
  cerr << "best_score   = " << best_score << endl;
  cerr << "temperature  = " << temperature << endl;
}

void output_ans() {
  cout << "{";
  for (int i = 0; i < n; i++) {
    cout << "{" << (int) pos[i].x << "," << (int) pos[i].y << "}";
    if (i < n-1) cout << ",";
  }
  cout << "};" << endl;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  get_first_input();

  init();
  solve();

  output_ans();

  return 0;
}
