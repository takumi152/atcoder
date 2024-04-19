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

// constant
constexpr int n = 20;

// inputs
int m;
vector<string> s;

// states
vector<vector<char> > board(n, vector<char>(n, '.'));
vector<vector<int> > board_cardinality(n, vector<int>(n));
vector<Pii> origin;
vector<bool> is_vertical;
vector<int> s_weight;

// hyper parameters
double base_temperature = 1.30e0;
double decay_rate = 6.00e-7;
int dlim = 4;
double single_include_factor = 1.00;
double single_exclude_factor = 1.00;
double single_move_factor = 1.00;

struct index_set {
  vector<int> included;
  vector<int> excluded;

  index_set() {};

  index_set(int n) {
    included.reserve(n);
    excluded.reserve(n);

    for (int i = 0; i < n; i++) excluded.push_back(i);
  }

  void include(int index) {
    assert(0 <= index && index < (int) excluded.size());
    included.push_back(excluded[index]);
    swap(excluded[index], excluded[excluded.size()-1]);
    excluded.pop_back();
  }

  void exclude(int index) {
    assert(0 <= index && index < (int) included.size());
    excluded.push_back(included[index]);
    swap(included[index], included[included.size()-1]);
    included.pop_back();
  }
};
index_set used_set;

void reduce_s() {
  double time_limit = 0.020;
  while (theTimer.time() < time_limit) {
    int i = theRandom.nextUIntMod(m);
    int j = theRandom.nextUIntMod(m);
    if (i == j) continue;
    if (s[i].size() < s[j].size()) swap(i, j);
    if (s[i].find(s[j]) != string::npos) {
      s_weight[i] += s_weight[j];
      s.erase(s.begin() + j);
      s_weight.erase(s_weight.begin() + j);
      m--;
    }
  }

  for (int dmin = 10; dmin >= dlim; dmin--) {
    time_limit += 0.020;
    while (theTimer.time() < time_limit) {
      int i = theRandom.nextUIntMod(m);

      for (int d = s[i].size(); d >= dmin; d--) {
        vector<int> found_index;
        for (int j = 0; j < m; j++) {
          if (i == j) continue;
          if ((int) s[j].size() < d) continue;
          if (s[i].substr(0, d) == s[j].substr(s[j].size() - d, d) && s[j].size() - d + s[i].size() <= 20) found_index.push_back(j);
        }
        if (found_index.size() >= 2) break;
        if (found_index.size() == 1) {
          int j = found_index[0];
          s[i] = s[j].substr(0, s[j].size() - d) + s[i];
          s_weight[i] += s_weight[j];
          s.erase(s.begin() + j);
          s_weight.erase(s_weight.begin() + j);
          m--;
        }
      }
    }
  }
}

bool check_addable(int d, int x, int y, int v) {
  int dx, dy;
  if (v) {
    dx = 1;
    dy = 0;
  }
  else {
    dx = 0;
    dy = 1;
  }

  for (int i = 0; i < (int) s[d].size(); i++) {
    if (board[x][y] != '.' && board[x][y] != s[d][i]) return false;
    x = (x + dx) % n;
    y = (y + dy) % n;
  }

  return true;
}

double calc_score_delta_add(int d, int x, int y, int v) {
  int dx, dy;
  if (v) {
    dx = 1;
    dy = 0;
  }
  else {
    dx = 0;
    dy = 1;
  }

  double score_delta = 0.0;
  for (int i = 0; i < (int) s[d].size(); i++) {
    if (board_cardinality[x][y] == 0) board[x][y] = s[d][i];
    board_cardinality[x][y] += s_weight[d];
    x = (x + dx) % n;
    y = (y + dy) % n;
  }

  score_delta += s_weight[d];

  return score_delta;
}

double calc_score_delta_remove(int d, int x, int y, int v) {
  int dx, dy;
  if (v) {
    dx = 1;
    dy = 0;
  }
  else {
    dx = 0;
    dy = 1;
  }

  double score_delta = 0.0;
  for (int i = 0; i < (int) s[d].size(); i++) {
    board_cardinality[x][y] -= s_weight[d];
    assert(board_cardinality[x][y] >= 0);
    if (board_cardinality[x][y] == 0) board[x][y] = '.';
    x = (x + dx) % n;
    y = (y + dy) % n;
  }

  score_delta -= s_weight[d];

  return score_delta;
}

void solve() {
  double score = 0;
  double last_score = score;
  double best_score = score;

  // double base_temperature = 5.0e-1;
  double temperature = base_temperature;
  // double decay_rate = 1.5e-7;
  double time_limit = 2.950;
  int iter_count = 0;

  double single_include_chance, single_exclude_chance, single_move_chance;
  {
    double factor_sum = single_include_factor + single_exclude_factor + single_move_factor;
    single_include_chance = single_include_factor / factor_sum;
    single_exclude_chance = single_include_chance + single_exclude_factor / factor_sum;
    single_move_chance = single_exclude_chance + single_move_factor / factor_sum;
  }

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < single_include_chance) {
      if (used_set.excluded.empty()) continue;
      int i = theRandom.nextUIntMod(used_set.excluded.size());
      int d = used_set.excluded[i];
      int x = theRandom.nextUIntMod(n);
      int y = theRandom.nextUIntMod(n);
      int v = theRandom.nextUIntMod(2);

      if (!check_addable(d, x, y, v)) continue;

      score += calc_score_delta_add(d, x, y, v);

      #ifdef DEBUG
      if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        used_set.include(i);
        origin[d] = Pii(x, y);
        is_vertical[d] = v;
        last_score = score;
        if (score > best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        used_set.include(i);
        origin[d] = Pii(x, y);
        is_vertical[d] = v;
        last_score = score;
      }
      else { // rollback
        calc_score_delta_remove(d, x, y, v);
        score = last_score;
      }
    }
    else if (roll < single_exclude_chance) {
      if (used_set.included.empty()) continue;
      int i = theRandom.nextUIntMod(used_set.included.size());
      int d = used_set.included[i];

      score += calc_score_delta_remove(d, origin[d].first, origin[d].second, is_vertical[d]);

      #ifdef DEBUG
      if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        used_set.exclude(i);
        origin[d] = Pii(-1, -1);
        last_score = score;
        if (score > best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        used_set.exclude(i);
        origin[d] = Pii(-1, -1);
        last_score = score;
      }
      else { // rollback
        calc_score_delta_add(d, origin[d].first, origin[d].second, is_vertical[d]);
        score = last_score;
      }
    }
    else if (roll < single_move_chance) {
      if (used_set.included.empty()) continue;
      int i = theRandom.nextUIntMod(used_set.included.size());
      int d = used_set.included[i];
      int x = theRandom.nextUIntMod(n);
      int y = theRandom.nextUIntMod(n);
      int v = theRandom.nextUIntMod(2);

      score += calc_score_delta_remove(d, origin[d].first, origin[d].second, is_vertical[d]);

      if (!check_addable(d, x, y, v)) {
        calc_score_delta_add(d, origin[d].first, origin[d].second, is_vertical[d]);
        score = last_score;
        continue;
      }

      score += calc_score_delta_add(d, x, y, v);

      #ifdef DEBUG
      if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        origin[d] = Pii(x, y);
        is_vertical[d] = v;
        last_score = score;
        if (score > best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        origin[d] = Pii(x, y);
        is_vertical[d] = v;
        last_score = score;
      }
      else { // rollback
        calc_score_delta_remove(d, x, y, v);
        calc_score_delta_add(d, origin[d].first, origin[d].second, is_vertical[d]);
        score = last_score;
      }
    }

    iter_count++;
    temperature *= 1.0 - decay_rate;
    // temperature = base_temperature * ((time_limit - theTimer.time()) / time_limit);
  }

  cerr << "iter_count  = " << iter_count << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "score       = " << score << endl;
  cerr << "best_score  = " << best_score << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  if (argc > 1) base_temperature = strtod(argv[1], nullptr);
  if (argc > 2) decay_rate = strtod(argv[2], nullptr);
  if (argc > 3) dlim = strtol(argv[3], nullptr, 10);
  if (argc > 6) {
    single_include_factor = strtod(argv[4], nullptr);
    single_exclude_factor = strtod(argv[5], nullptr);
    single_move_factor = strtod(argv[6], nullptr);
  }

  int _n;
  cin >> _n >> m;
  s = vector<string>(m);
  for (auto &x: s) cin >> x;

  origin = vector<Pii>(m, Pii(-1, -1));
  is_vertical = vector<bool>(m);
  s_weight = vector<int>(m, 1);

  reduce_s();

  used_set = index_set(m);

  solve();

  for (auto &x: board) {
    for (auto &y: x) cout << y;
    cout << endl;
  }

  return 0;
}
