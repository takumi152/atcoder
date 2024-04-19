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

  double angle_toward(const position& target_pos) const {
    if (target_pos == (*this)) return 0.0; // cannot determine direction
    return atan2(target_pos.y - this->y, target_pos.x - this->x);
  }

  position toward_angle_abs(const double theta, const double dist) const {
    double dx = cos(theta);
    double dy = sin(theta);
    return (*this) + position(dx, dy) * dist;
  }
};

enum search_status {
  NOT_FOUND = 0,
  FOUND = 1,
  ALL_FOUND = 2
};

struct search_result {
  search_status status;
  double theta;
  position treasure_position;
};

// constants
constexpr double pi = 3.14159265358979323846264338327950;

constexpr int treasure_num = 50;
constexpr int turn_limit = 1000;
constexpr double field_radius = 1e9;
constexpr double search_radius = 1e6;

// hyper parameters
double hyper_initial_speed_factor = 4.190992401338163;
double hyper_restart_speed_factor = 2.000541466866975;
double hyper_accel_factor = 0.7434402862283297;
double hyper_brake_angle_threshold = 1.5054529112705894;
double hyper_brake_factor = 1.3706828330740548;
double hyper_speed_reset_threshold = 0.8411348338406944;
double hyper_speed_reset_speed_factor = 3.2850211208385383;

// inputs
double stddev;

// internals
vector<position> searched_point;
vector<search_result> result_history;
vector<double> confidence;

void get_hyper_parameters(int argc, char *argv[]) {
  if (argc > 1) hyper_initial_speed_factor = strtod(argv[1], nullptr);
  if (argc > 2) hyper_restart_speed_factor = strtod(argv[2], nullptr);
  if (argc > 3) hyper_accel_factor = strtod(argv[3], nullptr);
  if (argc > 4) hyper_brake_angle_threshold = strtod(argv[4], nullptr);
  if (argc > 5) hyper_brake_factor = strtod(argv[5], nullptr);
  if (argc > 6) hyper_speed_reset_threshold = strtod(argv[6], nullptr);
  if (argc > 7) hyper_speed_reset_speed_factor = strtod(argv[7], nullptr);
}

void get_first_input() {
  cin >> stddev;
}

search_result search(position pos) {
  cout << (int) pos.x << " " << (int) pos.y << endl;

  search_result res;
  int status_code;
  cin >> status_code;
  res.status = (search_status) status_code;
  if (res.status == NOT_FOUND) {
    cin >> res.theta;
  }
  else {
    cin >> res.treasure_position.x >> res.treasure_position.y;
  }
  return res;
}

void mark_position(position p, int r, int g, int b) {
  cout << "#c " << (int) p.x << " " << (int) p.y << " " << r << " " << g << " " << b << endl;
}

void init() {
  
}

double distance_between_point_and_line(position p0, position p1, position p2) {
  return abs((p2.y - p1.y) * p0.x - (p2.x - p1.x) * p0.y + p2.x * p1.y - p2.y * p1.x) / sqrt((p2.y - p1.y) * (p2.y - p1.y) + (p2.x - p1.x) * (p2.x - p1.x));
}

double relative_angle(double theta1, double theta2) {
  if (theta1 < theta2) {
    double theta_low = theta2 - theta1 - pi * 2.0;
    double theta_high = theta2 - theta1;
    if (abs(theta_low) < abs(theta_high)) return theta_low;
    else return theta_high;
  }
  else {
    double theta_low = theta2 - theta1;
    double theta_high = theta2 - theta1 + pi * 2.0;
    if (abs(theta_low) < abs(theta_high)) return theta_low;
    else return theta_high;
  }
}

position find_intersection_between_two_lines(position p1, double theta1, position p2, double theta2) {
  double dx1 = cos(theta1) * field_radius;
  double dy1 = sin(theta1) * field_radius;
  position t1 = p1 + position(dx1, dy1);
  double dx2 = cos(theta2) * field_radius;
  double dy2 = sin(theta2) * field_radius;
  position t2 = p2 + position(dx2, dy2);

  double low = -field_radius * 10.0;
  double high = field_radius * 10.0;
  for (int i = 0; i < 80; i++) {
    double mid_low = (low * 2 + high) / 3;
    double mid_high = (low + high * 2) / 3;
    if (distance_between_point_and_line(p1.toward_pos_abs(t1, mid_low), p2, t2) > distance_between_point_and_line(p1.toward_pos_abs(t1, mid_high), p2, t2)) low = mid_low;
    else high = mid_high;
  }

  double mid = (low + high) / 2;
  return p1.toward_pos_abs(t1, mid);
}

double cdf(double rel_theta) {
  return (1.0 + erf(rel_theta / sqrt(2.0 * stddev * stddev))) / 2.0;
}

void solve() {
  position current_pos(field_radius * 2 / 3, 0);
  double delta = field_radius / hyper_initial_speed_factor;
  double last_theta = 0.0;
  for (int turn = 0; turn < turn_limit; turn++) {
    if (turn == 0) {
      auto res = search(current_pos);
      searched_point.push_back(current_pos);
      result_history.push_back(res);
      if (res.status == NOT_FOUND) last_theta = res.theta;
      continue;
    }

    while (true) {
      position next_pos;
      if (result_history[result_history.size()-1].status == NOT_FOUND) next_pos = current_pos.toward_angle_abs(result_history[result_history.size()-1].theta, delta);
      else                                                             next_pos = current_pos;

      if (next_pos.distance_to(position(0, 0)) > field_radius) {
        delta *= 0.8;
        continue;
      }

      current_pos = next_pos;
      break;
    }

    auto res = search(current_pos);
    searched_point.push_back(current_pos);
    result_history.push_back(res);
    confidence.push_back(1.0);

    if (res.status == NOT_FOUND) {
      double rel_theta = relative_angle(last_theta, res.theta);
      if (abs(rel_theta) > pi / hyper_brake_angle_threshold) delta /= (abs(rel_theta) / (pi / hyper_brake_angle_threshold)) * hyper_brake_factor;
      delta *= hyper_accel_factor;
      if (delta < search_radius * hyper_speed_reset_threshold) delta = field_radius / hyper_speed_reset_speed_factor;
      last_theta = res.theta;
    }
    else if (res.status == FOUND) {
      delta = field_radius / hyper_restart_speed_factor;
      for (int i = 0; i < (int) confidence.size(); i++) {
        if (result_history[i].status != NOT_FOUND) continue;
        confidence[i] *= abs(cdf(relative_angle(result_history[i].theta, searched_point[i].angle_toward(result_history[result_history.size()-1].treasure_position))) - 0.5) * 2.0;
      }
      int best_confidence_idx = 0;
      double best_confidence_val = 0.0;
      for (int i = 0; i < (int) confidence.size(); i++) {
        if (result_history[i].status != NOT_FOUND) continue;
        if (confidence[i] > best_confidence_val) {
          best_confidence_idx = i;
          best_confidence_val = confidence[i];
        }
      }
      current_pos = searched_point[best_confidence_idx];
      while (true) {
        position next_pos = current_pos.toward_angle_abs(result_history[best_confidence_idx].theta, delta);

        if (next_pos.distance_to(position(0, 0)) > field_radius) {
          delta *= 0.8;
          continue;
        }

        current_pos = next_pos;
        break;
      }
      last_theta = result_history[best_confidence_idx].theta;
    }
    else break;
  }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  get_hyper_parameters(argc, argv);

  get_first_input();

  init();
  solve();

  return 0;
}
