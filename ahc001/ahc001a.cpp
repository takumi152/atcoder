#pragma GCC optimize ("O3")
// #pragma GCC optimize ("unroll-loops")
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
#include <unordered_map>
#include <set>
#include <random>
#include <cmath>
#include <cassert>

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
constexpr int h = 10000;
constexpr int w = 10000;

// inputs
int n;
vector<int> x, y, r;

// outputs
vector<int> space_left;
vector<int> space_right;
vector<int> space_up;
vector<int> space_down;

vector<int> best_space_left;
vector<int> best_space_right;
vector<int> best_space_up;
vector<int> best_space_down;

// intermediates
vector<vector<int> > overlapped_area;

vector<Pii> space_left_sorted;
vector<Pii> space_right_sorted;
vector<Pii> space_up_sorted;
vector<Pii> space_down_sorted;
vector<vector<int> > overlapped_with;

ll total_area = 0;

inline int calc_overlapped_area(int left1, int right1, int up1, int down1, int left2, int right2, int up2, int down2, int px1, int py1, int px2, int py2) {
  if (px2 < px1) {
    swap(left1, right1);
    left1 = -left1;
    right1 = -right1;
    swap(left2, right2);
    left2 = -left2;
    right2 = -right2;
  }
  if (py2 < py1) {
    swap(up1, down1);
    up1 = -up1;
    down1 = -down1;
    swap(up2, down2);
    up2 = -up2;
    down2 = -down2;
  }
  return max(0, right1 - left2) * max(0, down1 - up2);
}

inline double calc_score(int r, int left, int right, int up, int down) {
  int area = (right - left) * (down - up);
  double factor = 1.0 - (double) min(r, area) / (double) max(r, area);
  return 1.0 - factor * abs(factor);
}

inline double calc_score_cubic(int r, int left, int right, int up, int down) {
  int area = (right - left) * (down - up);
  double factor = 1.0 - (double) min(r, area) / (double) max(r, area);
  return 1.0 - factor * factor * abs(factor);
}

double calc_score_delta(int tgt, int dir, int rep, double penalty_factor) {
  double score_delta = -calc_score(r[tgt], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt]);

       if (dir == 0) space_left[tgt] -= rep;
  else if (dir == 1) space_right[tgt] += rep;
  else if (dir == 2) space_up[tgt] -= rep;
  else if (dir == 3) space_down[tgt] += rep;

  int p1 = 0;
  while (p1 < (int) overlapped_with[tgt].size()) {
    int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[overlapped_with[tgt][p1]], space_right[overlapped_with[tgt][p1]], space_up[overlapped_with[tgt][p1]], space_down[overlapped_with[tgt][p1]], x[tgt], y[tgt], x[overlapped_with[tgt][p1]], y[overlapped_with[tgt][p1]]);
    int next_overlapped_area_2 = calc_overlapped_area(space_left[overlapped_with[tgt][p1]], space_right[overlapped_with[tgt][p1]], space_up[overlapped_with[tgt][p1]], space_down[overlapped_with[tgt][p1]], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[overlapped_with[tgt][p1]], y[overlapped_with[tgt][p1]], x[tgt], y[tgt]);
    score_delta += (double) ((overlapped_area[tgt][overlapped_with[tgt][p1]] - next_overlapped_area_1) + (overlapped_area[overlapped_with[tgt][p1]][tgt] - next_overlapped_area_2)) * penalty_factor;
    overlapped_area[tgt][overlapped_with[tgt][p1]] = next_overlapped_area_1;
    overlapped_area[overlapped_with[tgt][p1]][tgt] = next_overlapped_area_2;
    if (next_overlapped_area_1 == 0) {
    for (int p2 = 0; p2 < (int) overlapped_with[overlapped_with[tgt][p1]].size(); p2++) {
      if (overlapped_with[overlapped_with[tgt][p1]][p2] == tgt) {
        swap(overlapped_with[overlapped_with[tgt][p1]][p2], overlapped_with[overlapped_with[tgt][p1]][overlapped_with[overlapped_with[tgt][p1]].size()-1]);
        overlapped_with[overlapped_with[tgt][p1]].pop_back();
        break;
      }
    }
      swap(overlapped_with[tgt][p1], overlapped_with[tgt][overlapped_with[tgt].size()-1]);
      overlapped_with[tgt].pop_back();
    }
    else p1++;
  }

  if (rep > 0) {
    if (dir == 0) {
      auto it1 = lower_bound(space_right_sorted.begin(), space_right_sorted.end(), Pii(space_left[tgt] + 1, 0));
      while (it1 != space_right_sorted.end()) {
        if (space_left[tgt] + rep < it1->first) break;
        int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]);
        int next_overlapped_area_2 = calc_overlapped_area(space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[it1->second], y[it1->second], x[tgt], y[tgt]);
        if (next_overlapped_area_1 > 0) {
          score_delta += (double) ((overlapped_area[tgt][it1->second] - next_overlapped_area_1) + (overlapped_area[it1->second][tgt] - next_overlapped_area_2)) * penalty_factor;
          overlapped_area[tgt][it1->second] = next_overlapped_area_1;
          overlapped_area[it1->second][tgt] = next_overlapped_area_2;
          overlapped_with[tgt].push_back(it1->second);
          overlapped_with[it1->second].push_back(tgt);
        }

        it1++;
      }

      auto it2 = lower_bound(space_left_sorted.begin(), space_left_sorted.end(), Pii(space_left[tgt] + rep, tgt));
      // assert(it2 != space_left_sorted.end());
      // assert(*it2 == Pii(space_left[tgt] + rep, tgt));
      it2->first = space_left[tgt];
      auto it3 = it2 - 1;
      while (it2 != space_left_sorted.begin()) {
        if (*it3 < *it2) break;
        iter_swap(it3, it2);
        it2--;
        it3--;
      }
    }
    else if (dir == 1) {
      auto it1 = lower_bound(space_left_sorted.begin(), space_left_sorted.end(), Pii(space_right[tgt] - rep, 0));
      while (it1 != space_left_sorted.end()) {
        if (space_right[tgt] + rep <= it1->first) break;
        int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]);
        int next_overlapped_area_2 = calc_overlapped_area(space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[it1->second], y[it1->second], x[tgt], y[tgt]);
        if (next_overlapped_area_1 > 0) {
          score_delta += (double) ((overlapped_area[tgt][it1->second] - next_overlapped_area_1) + (overlapped_area[it1->second][tgt] - next_overlapped_area_2)) * penalty_factor;
          overlapped_area[tgt][it1->second] = next_overlapped_area_1;
          overlapped_area[it1->second][tgt] = next_overlapped_area_2;
          overlapped_with[tgt].push_back(it1->second);
          overlapped_with[it1->second].push_back(tgt);
        }

        it1++;
      }

      auto it2 = lower_bound(space_right_sorted.begin(), space_right_sorted.end(), Pii(space_right[tgt] - rep, tgt));
      // assert(it2 != space_right_sorted.end());
      // assert(*it2 == Pii(space_right[tgt] - rep, tgt));
      it2->first = space_right[tgt];
      auto it3 = it2 + 1;
      while (it3 != space_right_sorted.end()) {
        if (*it2 < *it3) break;
        iter_swap(it2, it3);
        it2++;
        it3++;
      }
    }

    else if (dir == 2) {
      auto it1 = lower_bound(space_down_sorted.begin(), space_down_sorted.end(), Pii(space_up[tgt] + 1, 0));
      while (it1 != space_down_sorted.end()) {
        if (space_up[tgt] + rep < it1->first) break;
        int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]);
        int next_overlapped_area_2 = calc_overlapped_area(space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[it1->second], y[it1->second], x[tgt], y[tgt]);
        if (next_overlapped_area_1 > 0) {
          score_delta += (double) ((overlapped_area[tgt][it1->second] - next_overlapped_area_1) + (overlapped_area[it1->second][tgt] - next_overlapped_area_2)) * penalty_factor;
          overlapped_area[tgt][it1->second] = next_overlapped_area_1;
          overlapped_area[it1->second][tgt] = next_overlapped_area_2;
          overlapped_with[tgt].push_back(it1->second);
          overlapped_with[it1->second].push_back(tgt);
        }

        it1++;
      }

      auto it2 = lower_bound(space_up_sorted.begin(), space_up_sorted.end(), Pii(space_up[tgt] + rep, tgt));
      // assert(it2 != space_up_sorted.end());
      // assert(*it2 == Pii(space_up[tgt] + rep, tgt));
      it2->first = space_up[tgt];
      auto it3 = it2 - 1;
      while (it2 != space_up_sorted.begin()) {
        if (*it3 < *it2) break;
        iter_swap(it3, it2);
        it2--;
        it3--;
      }
    }
    else if (dir == 3) {
      auto it1 = lower_bound(space_up_sorted.begin(), space_up_sorted.end(), Pii(space_down[tgt] - rep, 0));
      while (it1 != space_up_sorted.end()) {
        if (space_down[tgt] + rep <= it1->first) break;
        int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]);
        int next_overlapped_area_2 = calc_overlapped_area(space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[it1->second], y[it1->second], x[tgt], y[tgt]);
        if (next_overlapped_area_1 > 0) {
          score_delta += (double) ((overlapped_area[tgt][it1->second] - next_overlapped_area_1) + (overlapped_area[it1->second][tgt] - next_overlapped_area_2)) * penalty_factor;
          overlapped_area[tgt][it1->second] = next_overlapped_area_1;
          overlapped_area[it1->second][tgt] = next_overlapped_area_2;
          overlapped_with[tgt].push_back(it1->second);
          overlapped_with[it1->second].push_back(tgt);
        }

        it1++;
      }

      auto it2 = lower_bound(space_down_sorted.begin(), space_down_sorted.end(), Pii(space_down[tgt] - rep, tgt));
      // assert(it2 != space_down_sorted.end());
      // assert(*it2 == Pii(space_down[tgt] - rep, tgt));
      it2->first = space_down[tgt];
      auto it3 = it2 + 1;
      while (it3 != space_down_sorted.end()) {
        if (*it2 < *it3) break;
        iter_swap(it2, it3);
        it2++;
        it3++;
      }
    }
  }
  else {
    if (dir == 0) {
      auto it2 = lower_bound(space_left_sorted.begin(), space_left_sorted.end(), Pii(space_left[tgt] + rep, tgt));
      // assert(it2 != space_left_sorted.end());
      // assert(*it2 == Pii(space_left[tgt] + rep, tgt));
      it2->first = space_left[tgt];
      auto it3 = it2 + 1;
      while (it3 != space_left_sorted.end()) {
        if (*it2 < *it3) break;
        iter_swap(it2, it3);
        it2++;
        it3++;
      }
    }
    else if (dir == 1) {
      auto it2 = lower_bound(space_right_sorted.begin(), space_right_sorted.end(), Pii(space_right[tgt] - rep, tgt));
      // assert(it2 != space_right_sorted.end());
      // assert(*it2 == Pii(space_right[tgt] - rep, tgt));
      it2->first = space_right[tgt];
      auto it3 = it2 - 1;
      while (it2 != space_right_sorted.begin()) {
        if (*it3 < *it2) break;
        iter_swap(it3, it2);
        it2--;
        it3--;
      }
    }
    else if (dir == 2) {
      auto it2 = lower_bound(space_up_sorted.begin(), space_up_sorted.end(), Pii(space_up[tgt] + rep, tgt));
      // assert(it2 != space_up_sorted.end());
      // assert(*it2 == Pii(space_up[tgt] + rep, tgt));
      it2->first = space_up[tgt];
      auto it3 = it2 + 1;
      while (it3 != space_up_sorted.end()) {
        if (*it2 < *it3) break;
        iter_swap(it2, it3);
        it2++;
        it3++;
      }
    }
    else if (dir == 3) {
      auto it2 = lower_bound(space_down_sorted.begin(), space_down_sorted.end(), Pii(space_down[tgt] - rep, tgt));
      // assert(it2 != space_down_sorted.end());
      // assert(*it2 == Pii(space_down[tgt] - rep, tgt));
      it2->first = space_down[tgt];
      auto it3 = it2 - 1;
      while (it2 != space_down_sorted.begin()) {
        if (*it3 < *it2) break;
        iter_swap(it3, it2);
        it2--;
        it3--;
      }
    }
  }

  // assert(is_sorted(space_left_sorted.begin(), space_left_sorted.end()));
  // assert(is_sorted(space_right_sorted.begin(), space_right_sorted.end()));
  // assert(is_sorted(space_up_sorted.begin(), space_up_sorted.end()));
  // assert(is_sorted(space_down_sorted.begin(), space_down_sorted.end()));

  for (int i = 0; i < n; i++) {
    // assert(space_left_sorted[i].first == space_left[space_left_sorted[i].second]);
    // assert(space_right_sorted[i].first == space_right[space_right_sorted[i].second]);
    // assert(space_up_sorted[i].first == space_up[space_up_sorted[i].second]);
    // assert(space_down_sorted[i].first == space_down[space_down_sorted[i].second]);
  }

  score_delta += calc_score(r[tgt], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt]);

  return score_delta;
}

double calc_score_delta_cubic(int tgt, int dir, int rep, double penalty_factor) {
  double score_delta = -calc_score_cubic(r[tgt], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt]);

       if (dir == 0) space_left[tgt] -= rep;
  else if (dir == 1) space_right[tgt] += rep;
  else if (dir == 2) space_up[tgt] -= rep;
  else if (dir == 3) space_down[tgt] += rep;

  int p1 = 0;
  while (p1 < (int) overlapped_with[tgt].size()) {
    int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[overlapped_with[tgt][p1]], space_right[overlapped_with[tgt][p1]], space_up[overlapped_with[tgt][p1]], space_down[overlapped_with[tgt][p1]], x[tgt], y[tgt], x[overlapped_with[tgt][p1]], y[overlapped_with[tgt][p1]]);
    int next_overlapped_area_2 = calc_overlapped_area(space_left[overlapped_with[tgt][p1]], space_right[overlapped_with[tgt][p1]], space_up[overlapped_with[tgt][p1]], space_down[overlapped_with[tgt][p1]], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[overlapped_with[tgt][p1]], y[overlapped_with[tgt][p1]], x[tgt], y[tgt]);
    score_delta += (double) ((overlapped_area[tgt][overlapped_with[tgt][p1]] - next_overlapped_area_1) + (overlapped_area[overlapped_with[tgt][p1]][tgt] - next_overlapped_area_2)) * penalty_factor;
    overlapped_area[tgt][overlapped_with[tgt][p1]] = next_overlapped_area_1;
    overlapped_area[overlapped_with[tgt][p1]][tgt] = next_overlapped_area_2;
    if (next_overlapped_area_1 == 0) {
    for (int p2 = 0; p2 < (int) overlapped_with[overlapped_with[tgt][p1]].size(); p2++) {
      if (overlapped_with[overlapped_with[tgt][p1]][p2] == tgt) {
        swap(overlapped_with[overlapped_with[tgt][p1]][p2], overlapped_with[overlapped_with[tgt][p1]][overlapped_with[overlapped_with[tgt][p1]].size()-1]);
        overlapped_with[overlapped_with[tgt][p1]].pop_back();
        break;
      }
    }
      swap(overlapped_with[tgt][p1], overlapped_with[tgt][overlapped_with[tgt].size()-1]);
      overlapped_with[tgt].pop_back();
    }
    else p1++;
  }

  if (rep > 0) {
    if (dir == 0) {
      auto it1 = lower_bound(space_right_sorted.begin(), space_right_sorted.end(), Pii(space_left[tgt] + 1, 0));
      while (it1 != space_right_sorted.end()) {
        if (space_left[tgt] + rep < it1->first) break;
        int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]);
        int next_overlapped_area_2 = calc_overlapped_area(space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[it1->second], y[it1->second], x[tgt], y[tgt]);
        if (next_overlapped_area_1 > 0) {
          score_delta += (double) ((overlapped_area[tgt][it1->second] - next_overlapped_area_1) + (overlapped_area[it1->second][tgt] - next_overlapped_area_2)) * penalty_factor;
          overlapped_area[tgt][it1->second] = next_overlapped_area_1;
          overlapped_area[it1->second][tgt] = next_overlapped_area_2;
          overlapped_with[tgt].push_back(it1->second);
          overlapped_with[it1->second].push_back(tgt);
        }

        it1++;
      }

      auto it2 = lower_bound(space_left_sorted.begin(), space_left_sorted.end(), Pii(space_left[tgt] + rep, tgt));
      // assert(it2 != space_left_sorted.end());
      // assert(*it2 == Pii(space_left[tgt] + rep, tgt));
      it2->first = space_left[tgt];
      auto it3 = it2 - 1;
      while (it2 != space_left_sorted.begin()) {
        if (*it3 < *it2) break;
        iter_swap(it3, it2);
        it2--;
        it3--;
      }
    }
    else if (dir == 1) {
      auto it1 = lower_bound(space_left_sorted.begin(), space_left_sorted.end(), Pii(space_right[tgt] - rep, 0));
      while (it1 != space_left_sorted.end()) {
        if (space_right[tgt] + rep <= it1->first) break;
        int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]);
        int next_overlapped_area_2 = calc_overlapped_area(space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[it1->second], y[it1->second], x[tgt], y[tgt]);
        if (next_overlapped_area_1 > 0) {
          score_delta += (double) ((overlapped_area[tgt][it1->second] - next_overlapped_area_1) + (overlapped_area[it1->second][tgt] - next_overlapped_area_2)) * penalty_factor;
          overlapped_area[tgt][it1->second] = next_overlapped_area_1;
          overlapped_area[it1->second][tgt] = next_overlapped_area_2;
          overlapped_with[tgt].push_back(it1->second);
          overlapped_with[it1->second].push_back(tgt);
        }

        it1++;
      }

      auto it2 = lower_bound(space_right_sorted.begin(), space_right_sorted.end(), Pii(space_right[tgt] - rep, tgt));
      // assert(it2 != space_right_sorted.end());
      // assert(*it2 == Pii(space_right[tgt] - rep, tgt));
      it2->first = space_right[tgt];
      auto it3 = it2 + 1;
      while (it3 != space_right_sorted.end()) {
        if (*it2 < *it3) break;
        iter_swap(it2, it3);
        it2++;
        it3++;
      }
    }

    else if (dir == 2) {
      auto it1 = lower_bound(space_down_sorted.begin(), space_down_sorted.end(), Pii(space_up[tgt] + 1, 0));
      while (it1 != space_down_sorted.end()) {
        if (space_up[tgt] + rep < it1->first) break;
        int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]);
        int next_overlapped_area_2 = calc_overlapped_area(space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[it1->second], y[it1->second], x[tgt], y[tgt]);
        if (next_overlapped_area_1 > 0) {
          score_delta += (double) ((overlapped_area[tgt][it1->second] - next_overlapped_area_1) + (overlapped_area[it1->second][tgt] - next_overlapped_area_2)) * penalty_factor;
          overlapped_area[tgt][it1->second] = next_overlapped_area_1;
          overlapped_area[it1->second][tgt] = next_overlapped_area_2;
          overlapped_with[tgt].push_back(it1->second);
          overlapped_with[it1->second].push_back(tgt);
        }

        it1++;
      }

      auto it2 = lower_bound(space_up_sorted.begin(), space_up_sorted.end(), Pii(space_up[tgt] + rep, tgt));
      // assert(it2 != space_up_sorted.end());
      // assert(*it2 == Pii(space_up[tgt] + rep, tgt));
      it2->first = space_up[tgt];
      auto it3 = it2 - 1;
      while (it2 != space_up_sorted.begin()) {
        if (*it3 < *it2) break;
        iter_swap(it3, it2);
        it2--;
        it3--;
      }
    }
    else if (dir == 3) {
      auto it1 = lower_bound(space_up_sorted.begin(), space_up_sorted.end(), Pii(space_down[tgt] - rep, 0));
      while (it1 != space_up_sorted.end()) {
        if (space_down[tgt] + rep <= it1->first) break;
        int next_overlapped_area_1 = calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]);
        int next_overlapped_area_2 = calc_overlapped_area(space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt], x[it1->second], y[it1->second], x[tgt], y[tgt]);
        if (next_overlapped_area_1 > 0) {
          score_delta += (double) ((overlapped_area[tgt][it1->second] - next_overlapped_area_1) + (overlapped_area[it1->second][tgt] - next_overlapped_area_2)) * penalty_factor;
          overlapped_area[tgt][it1->second] = next_overlapped_area_1;
          overlapped_area[it1->second][tgt] = next_overlapped_area_2;
          overlapped_with[tgt].push_back(it1->second);
          overlapped_with[it1->second].push_back(tgt);
        }

        it1++;
      }

      auto it2 = lower_bound(space_down_sorted.begin(), space_down_sorted.end(), Pii(space_down[tgt] - rep, tgt));
      // assert(it2 != space_down_sorted.end());
      // assert(*it2 == Pii(space_down[tgt] - rep, tgt));
      it2->first = space_down[tgt];
      auto it3 = it2 + 1;
      while (it3 != space_down_sorted.end()) {
        if (*it2 < *it3) break;
        iter_swap(it2, it3);
        it2++;
        it3++;
      }
    }
  }
  else {
    if (dir == 0) {
      auto it2 = lower_bound(space_left_sorted.begin(), space_left_sorted.end(), Pii(space_left[tgt] + rep, tgt));
      // assert(it2 != space_left_sorted.end());
      // assert(*it2 == Pii(space_left[tgt] + rep, tgt));
      it2->first = space_left[tgt];
      auto it3 = it2 + 1;
      while (it3 != space_left_sorted.end()) {
        if (*it2 < *it3) break;
        iter_swap(it2, it3);
        it2++;
        it3++;
      }
    }
    else if (dir == 1) {
      auto it2 = lower_bound(space_right_sorted.begin(), space_right_sorted.end(), Pii(space_right[tgt] - rep, tgt));
      // assert(it2 != space_right_sorted.end());
      // assert(*it2 == Pii(space_right[tgt] - rep, tgt));
      it2->first = space_right[tgt];
      auto it3 = it2 - 1;
      while (it2 != space_right_sorted.begin()) {
        if (*it3 < *it2) break;
        iter_swap(it3, it2);
        it2--;
        it3--;
      }
    }
    else if (dir == 2) {
      auto it2 = lower_bound(space_up_sorted.begin(), space_up_sorted.end(), Pii(space_up[tgt] + rep, tgt));
      // assert(it2 != space_up_sorted.end());
      // assert(*it2 == Pii(space_up[tgt] + rep, tgt));
      it2->first = space_up[tgt];
      auto it3 = it2 + 1;
      while (it3 != space_up_sorted.end()) {
        if (*it2 < *it3) break;
        iter_swap(it2, it3);
        it2++;
        it3++;
      }
    }
    else if (dir == 3) {
      auto it2 = lower_bound(space_down_sorted.begin(), space_down_sorted.end(), Pii(space_down[tgt] - rep, tgt));
      // assert(it2 != space_down_sorted.end());
      // assert(*it2 == Pii(space_down[tgt] - rep, tgt));
      it2->first = space_down[tgt];
      auto it3 = it2 - 1;
      while (it2 != space_down_sorted.begin()) {
        if (*it3 < *it2) break;
        iter_swap(it3, it2);
        it2--;
        it3--;
      }
    }
  }

  // assert(is_sorted(space_left_sorted.begin(), space_left_sorted.end()));
  // assert(is_sorted(space_right_sorted.begin(), space_right_sorted.end()));
  // assert(is_sorted(space_up_sorted.begin(), space_up_sorted.end()));
  // assert(is_sorted(space_down_sorted.begin(), space_down_sorted.end()));

  for (int i = 0; i < n; i++) {
    // assert(space_left_sorted[i].first == space_left[space_left_sorted[i].second]);
    // assert(space_right_sorted[i].first == space_right[space_right_sorted[i].second]);
    // assert(space_up_sorted[i].first == space_up[space_up_sorted[i].second]);
    // assert(space_down_sorted[i].first == space_down[space_down_sorted[i].second]);
  }

  score_delta += calc_score_cubic(r[tgt], space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt]);

  return score_delta;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  cin >> n;
  x = vector<int>(n);
  y = vector<int>(n);
  r = vector<int>(n);
  for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> r[i];

  // preprocessing
  theRandom.x = ((ll) random_device()() << 32) | (ll) random_device()();

  space_left = vector<int>(n);
  space_right = vector<int>(n);
  space_up = vector<int>(n);
  space_down = vector<int>(n);
  for (int i = 0; i < n; i++) {
    space_left[i] = 0;
    space_right[i] = 10000;
    space_up[i] = 0;
    space_down[i] = 10000;
  }

  best_space_left = space_left;
  best_space_right = space_right;
  best_space_up = space_up;
  best_space_down = space_down;

  overlapped_area = vector<vector<int> >(n, vector<int>(n, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) continue;
      overlapped_area[i][j] = calc_overlapped_area(space_left[i], space_right[i], space_up[i], space_down[i], space_left[j], space_right[j], space_up[j], space_down[j], x[i], y[i], x[j], y[j]);
      overlapped_area[j][i] = calc_overlapped_area(space_left[j], space_right[j], space_up[j], space_down[j], space_left[i], space_right[i], space_up[i], space_down[i], x[j], y[j], x[i], y[i]);
    }
  }

  space_left_sorted = vector<Pii>(n);
  space_right_sorted = vector<Pii>(n);
  space_up_sorted = vector<Pii>(n);
  space_down_sorted = vector<Pii>(n);
  for (int i = 0; i < n; i++) {
    space_left_sorted[i] = Pii(space_left[i], i);
    space_right_sorted[i] = Pii(space_right[i], i);
    space_up_sorted[i] = Pii(space_up[i], i);
    space_down_sorted[i] = Pii(space_down[i], i);
  }

  overlapped_with = vector<vector<int> >(n);
  for (int i = 0; i < n; i++) {
    overlapped_with[i].reserve(n);
    for (int j = 0; j < n; j++) {
      if (i == j) continue;
      if (overlapped_area[i][j] > 0) overlapped_with[i].push_back(j);
    }
  }

  double total_area_score_multiplier = 0.0;
  total_area = 0;
  for (int i = 0; i < n; i++) total_area += (space_right[i] - space_left[i]) * (space_down[i] - space_up[i]);

  // optimization
  bool score_func_changed = false;
  auto score_func = calc_score;
  auto score_func_delta = calc_score_delta;

  double penalty_factor = 1e-8;
  int rep_max = 1000;
  vector<double> time_schedule;
  vector<double> penalty_schedule;
  vector<int> rep_max_schedule;
  time_schedule.push_back(0.000);
  penalty_schedule.push_back(1e-8);
  rep_max_schedule.push_back(2000);
  for (int i = 1; i <= 50; i++) {
    time_schedule.push_back(time_schedule[time_schedule.size()-1] + 0.070);
    if (i <= 45) penalty_schedule.push_back(penalty_schedule[penalty_schedule.size()-1] * 1.15);
    else penalty_schedule.push_back(1.0);
    rep_max_schedule.push_back(max(1, rep_max_schedule[rep_max_schedule.size()-1] * 9 / 10 - 1));
  }
  int next_time_schedule = 0;

  bool chance_changed = false;
  double single_stretch_chance = 0.9;
  double double_stretch_chance = 1.0;

  double score = 0.0;
  for (int i = 0; i < n; i++) {
    score += score_func(r[i], space_left[i], space_right[i], space_up[i], space_down[i]);
    for (int j = 0; j < n; j++) {
      if (i == j) continue;
      score -= penalty_factor * overlapped_area[i][j];
    }
  }
  score += total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * total_area_score_multiplier;
  double lastScore = score;
  double bestScore = score;

  double baseTemperature = 5e-2;
  double temperature = baseTemperature;
  double decayRate = 5e-7;
  double timeLimit = 3.430;
  int iterCount = 0;

  bool kicked = false;

  while (theTimer.time() < timeLimit) {
    if (!score_func_changed && theTimer.time() >= 2.500) {
      score_func = calc_score;
      score_func_delta = calc_score_delta;
      score_func_changed = true;
    }

    if (!chance_changed && theTimer.time() >= 3.400) {
      single_stretch_chance = 0.1;
      double_stretch_chance = 0.2;
      chance_changed = true;
    }

    if (theTimer.time() >= time_schedule[next_time_schedule]) {
      penalty_factor = penalty_schedule[next_time_schedule];
      rep_max = rep_max_schedule[next_time_schedule];
      next_time_schedule++;

      if (theTimer.time() > 3.000) total_area_score_multiplier = 0.0;

      score = 0.0;
      // double score_base = 0.0;
      // double score_pen = 0.0;
      for (int i = 0; i < n; i++) {
        score += score_func(r[i], space_left[i], space_right[i], space_up[i], space_down[i]);
        // score_base += score_func(r[i], space_left[i], space_right[i], space_up[i], space_down[i]);
        for (int j = 0; j < n; j++) {
          if (i == j) continue;
          score -= penalty_factor * overlapped_area[i][j];
          // score_pen -= penalty_factor * overlapped_area[i][j];
        }
      }
      score += total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * total_area_score_multiplier;
      // cerr << score_base << " " << score_pen << endl;
      lastScore = score;
      bestScore = 0.0;
      ll best_total_area = 0;
      for (int i = 0; i < n; i++) {
        bestScore += score_func(r[i], best_space_left[i], best_space_right[i], best_space_up[i], best_space_down[i]);
        best_total_area += (best_space_right[i] - best_space_left[i]) * (best_space_down[i] - best_space_up[i]);
        for (int j = 0; j < n; j++) {
          if (i == j) continue;
          bestScore -= penalty_factor * calc_overlapped_area(best_space_left[i], best_space_right[i], best_space_up[i], best_space_down[i], best_space_left[j], best_space_right[j], best_space_up[j], best_space_down[j], x[i], y[i], x[j], y[j]);
        }
      }
      bestScore += total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, best_total_area) / (double) max((ll) 1e8, best_total_area)) * (1.0 - (double) min((ll) 1e8, best_total_area) / (double) max((ll) 1e8, best_total_area)) * total_area_score_multiplier;
    }

    double roll = theRandom.nextDouble();
    if (roll < single_stretch_chance) {
      int tgt = theRandom.nextUIntMod(n);
      int dir = theRandom.nextUIntMod(4);
      int rep = theRandom.nextUIntMod(rep_max * 2 + 1) - rep_max;
      if (rep == 0) continue;

           if (dir == 0 && (space_left[tgt]  - rep <  0 || space_left[tgt]  - rep >  x[tgt])) continue;
      else if (dir == 1 && (space_right[tgt] + rep >= w || space_right[tgt] + rep <= x[tgt])) continue;
      else if (dir == 2 && (space_up[tgt]    - rep <  0 || space_up[tgt]    - rep >  y[tgt])) continue;
      else if (dir == 3 && (space_down[tgt]  + rep >= h || space_down[tgt]  + rep <= y[tgt])) continue;

      score -= total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * total_area_score_multiplier;

      total_area -= (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);
      score += score_func_delta(tgt, dir, rep, penalty_factor);
      total_area += (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);

      score += total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * total_area_score_multiplier;

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
          best_space_left = space_left;
          best_space_right = space_right;
          best_space_up = space_up;
          best_space_down = space_down;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        total_area -= (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);
        score_func_delta(tgt, dir, -rep, penalty_factor);
        total_area += (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);
        score = lastScore;
      }
    }
    else if (roll < double_stretch_chance) {
      int tgt = theRandom.nextUIntMod(n);
      int dir1 = theRandom.nextUIntMod(4);
      int dir2 = theRandom.nextUIntMod(4);
      int rep1 = theRandom.nextUIntMod(rep_max * 2 + 1) - rep_max;
      int rep2 = theRandom.nextUIntMod(rep_max * 2 + 1) - rep_max;
      if (dir1 == dir2) continue;
      if (rep1 == 0 || rep2 == 0) continue;

      if      (dir1 == 0 && (space_left[tgt]  - rep1 <  0 || space_left[tgt]  - rep1 >  x[tgt])) continue;
      else if (dir1 == 1 && (space_right[tgt] + rep1 >= w || space_right[tgt] + rep1 <= x[tgt])) continue;
      else if (dir1 == 2 && (space_up[tgt]    - rep1 <  0 || space_up[tgt]    - rep1 >  y[tgt])) continue;
      else if (dir1 == 3 && (space_down[tgt]  + rep1 >= h || space_down[tgt]  + rep1 <= y[tgt])) continue;

      if      (dir2 == 0 && (space_left[tgt]  - rep2 <  0 || space_left[tgt]  - rep2 >  x[tgt])) continue;
      else if (dir2 == 1 && (space_right[tgt] + rep2 >= w || space_right[tgt] + rep2 <= x[tgt])) continue;
      else if (dir2 == 2 && (space_up[tgt]    - rep2 <  0 || space_up[tgt]    - rep2 >  y[tgt])) continue;
      else if (dir2 == 3 && (space_down[tgt]  + rep2 >= h || space_down[tgt]  + rep2 <= y[tgt])) continue;

      score -= total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * total_area_score_multiplier;

      total_area -= (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);
      score += score_func_delta(tgt, dir1, rep1, penalty_factor);
      score += score_func_delta(tgt, dir2, rep2, penalty_factor);
      total_area += (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);

      score += total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * total_area_score_multiplier;

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
          best_space_left = space_left;
          best_space_right = space_right;
          best_space_up = space_up;
          best_space_down = space_down;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        total_area -= (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);
        score_func_delta(tgt, dir1, -rep1, penalty_factor);
        score_func_delta(tgt, dir2, -rep2, penalty_factor);
        total_area += (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);
        score = lastScore;
      }
    }
    else {
      int tgt = theRandom.nextUIntMod(n);
      int dir = theRandom.nextUIntMod(4);
      int rep = theRandom.nextUIntMod(rep_max) + 1;

           if (dir == 0 && (space_left[tgt]  - rep <  0)) continue;
      else if (dir == 1 && (space_right[tgt] + rep >= w)) continue;
      else if (dir == 2 && (space_up[tgt]    - rep <  0)) continue;
      else if (dir == 3 && (space_down[tgt]  + rep >= h)) continue;

      static vector<Pii> extra_moves;
      extra_moves.clear();
      if (dir == 0) {
        auto it1 = lower_bound(space_right_sorted.begin(), space_right_sorted.end(), Pii(space_left[tgt] - rep + 1, 0));
        while (it1 != space_right_sorted.end()) {
          if (space_left[tgt] < it1->first) break;
          if (tgt == it1->second) continue;
          if (!((space_up[tgt] <= space_down[it1->second] && space_down[tgt] <= space_up[it1->second]) || (space_up[it1->second] <= space_down[tgt] && space_down[it1->second] <= space_up[tgt]))) {
            // assert(calc_overlapped_area(space_left[tgt] - rep, space_right[tgt], space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]) > 0);
            extra_moves.emplace_back(it1->second, -min(rep, min(it1->first - (space_left[tgt] - rep), it1->first - (x[it1->second] + 1))));
          }

          it1++;
        }
      }
      else if (dir == 1) {
        auto it1 = lower_bound(space_left_sorted.begin(), space_left_sorted.end(), Pii(space_right[tgt], 0));
        while (it1 != space_left_sorted.end()) {
          if (space_right[tgt] + rep <= it1->first) break;
          if (tgt == it1->second) continue;
          if (!((space_up[tgt] <= space_down[it1->second] && space_down[tgt] <= space_up[it1->second]) || (space_up[it1->second] <= space_down[tgt] && space_down[it1->second] <= space_up[tgt]))) {
            // assert(calc_overlapped_area(space_left[tgt], space_right[tgt] + rep, space_up[tgt], space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]) > 0);
            extra_moves.emplace_back(it1->second, -min(rep, min((space_right[tgt] + rep) - it1->first, x[it1->second] - it1->first)));
          }

          it1++;
        }
      }
      else if (dir == 2) {
        auto it1 = lower_bound(space_down_sorted.begin(), space_down_sorted.end(), Pii(space_up[tgt] - rep + 1, 0));
        while (it1 != space_down_sorted.end()) {
          if (space_up[tgt] < it1->first) break;
          if (tgt == it1->second) continue;
          if (!((space_left[tgt] <= space_right[it1->second] && space_right[tgt] <= space_left[it1->second]) || (space_left[it1->second] <= space_right[tgt] && space_right[it1->second] <= space_left[tgt]))) {
            // assert(calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt] - rep, space_down[tgt], space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]) > 0);
            extra_moves.emplace_back(it1->second, -min(rep, min(it1->first - (space_up[tgt] - rep), it1->first - (y[it1->second] + 1))));
          }

          it1++;
        }
      }
      else if (dir == 3) {
        auto it1 = lower_bound(space_up_sorted.begin(), space_up_sorted.end(), Pii(space_down[tgt], 0));
        while (it1 != space_up_sorted.end()) {
          if (space_down[tgt] + rep <= it1->first) break;
          if (tgt == it1->second) continue;
          if (!((space_left[tgt] <= space_right[it1->second] && space_right[tgt] <= space_left[it1->second]) || (space_left[it1->second] <= space_right[tgt] && space_right[it1->second] <= space_left[tgt]))) {
            // assert(calc_overlapped_area(space_left[tgt], space_right[tgt], space_up[tgt], space_down[tgt] + rep, space_left[it1->second], space_right[it1->second], space_up[it1->second], space_down[it1->second], x[tgt], y[tgt], x[it1->second], y[it1->second]) > 0);
            extra_moves.emplace_back(it1->second, -min(rep, min((space_down[tgt] + rep) - it1->first, y[it1->second] - it1->first)));
          }

          it1++;
        }
      }

      score -= total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * total_area_score_multiplier;

      for (auto &[tgt_ex, rep_ex]: extra_moves) {
        total_area -= (space_right[tgt_ex] - space_left[tgt_ex]) * (space_down[tgt_ex] - space_up[tgt_ex]);
        score += score_func_delta(tgt_ex, dir ^ 1, rep_ex, penalty_factor);
        total_area += (space_right[tgt_ex] - space_left[tgt_ex]) * (space_down[tgt_ex] - space_up[tgt_ex]);
      }
      total_area -= (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);
      score += score_func_delta(tgt, dir, rep, penalty_factor);
      total_area += (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);

      score += total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * total_area_score_multiplier;

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
          best_space_left = space_left;
          best_space_right = space_right;
          best_space_up = space_up;
          best_space_down = space_down;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / (temperature * 1e-10))) { // accept
        lastScore = score;
      }
      else { // rollback
        total_area -= (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);
        score_func_delta(tgt, dir, -rep, penalty_factor);
        total_area += (space_right[tgt] - space_left[tgt]) * (space_down[tgt] - space_up[tgt]);
        for (auto &[tgt_ex, rep_ex]: extra_moves) {
          total_area -= (space_right[tgt_ex] - space_left[tgt_ex]) * (space_down[tgt_ex] - space_up[tgt_ex]);
          score_func_delta(tgt_ex, dir ^ 1, -rep_ex, penalty_factor);
          total_area += (space_right[tgt_ex] - space_left[tgt_ex]) * (space_down[tgt_ex] - space_up[tgt_ex]);
        }
        score = lastScore;
      }
    }

    // restore best
    if (iterCount % 200000 == 0) {
      space_left = best_space_left;
      space_right = best_space_right;
      space_up = best_space_up;
      space_down = best_space_down;

      for (int i = 0; i < n; i++) {
        space_left_sorted[i] = Pii(space_left[i], i);
        space_right_sorted[i] = Pii(space_right[i], i);
        space_up_sorted[i] = Pii(space_up[i], i);
        space_down_sorted[i] = Pii(space_down[i], i);
      }
      sort(space_left_sorted.begin(), space_left_sorted.end());
      sort(space_right_sorted.begin(), space_right_sorted.end());
      sort(space_up_sorted.begin(), space_up_sorted.end());
      sort(space_down_sorted.begin(), space_down_sorted.end());

      score = 0.0;
      total_area = 0;
      for (int i = 0; i < n; i++) {
        overlapped_with[i].clear();
        score += score_func(r[i], space_left[i], space_right[i], space_up[i], space_down[i]);
        total_area += (space_right[i] - space_left[i]) * (space_down[i] - space_up[i]);
        for (int j = 0; j < n; j++) {
          if (i == j) continue;
          overlapped_area[i][j] = calc_overlapped_area(space_left[i], space_right[i], space_up[i], space_down[i], space_left[j], space_right[j], space_up[j], space_down[j], x[i], y[i], x[j], y[j]);
          if (overlapped_area[i][j] > 0) overlapped_with[i].push_back(j);
          score -= penalty_factor * overlapped_area[i][j];
        }
      }
      score += total_area_score_multiplier - (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * (1.0 - (double) min((ll) 1e8, total_area) / (double) max((ll) 1e8, total_area)) * total_area_score_multiplier;
      lastScore = score;
    }

    if (!kicked && theTimer.time > 2.500) {
      for (int i = 0; i < n; i++) partial_score.emplace_back(calc_score(r[i], space_left[i], space_right[i], space_up[i], space_down[i]), i);
      kicked = true;
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "bestScore   = " << bestScore << endl;

  // postprocess & output
  for (int i = 0; i < n; i++) {
    cout << space_left[i] << " " << space_up[i] << " " << space_right[i] << " " << space_down[i] << endl;
  }

  return 0;
}
