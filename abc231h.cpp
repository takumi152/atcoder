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

// constants

// inputs
int h, w, n;
vector<ll> a, b, c;

// outputs
ll ans;

// internals
vector<vector<int> > row_id;
vector<vector<int> > col_id;

vector<vector<int> > board;

vector<bool> used_id;
index_set used_set;

void solve() {
  row_id = vector<vector<int> >(h);
  col_id = vector<vector<int> >(w);
  for (int i = 0; i < n; i++) {
    row_id[a[i]-1].push_back(i);
    col_id[b[i]-1].push_back(i);
  }

  board = vector<vector<int> >(h, vector<int>(w, -1));
  for (int i = 0; i < n; i++) board[a[i]-1][b[i]-1] = i;

  used_id = vector<bool>(n, true);
  used_set = index_set(n);
  for (int i = 0; i < n; i++) used_set.include_element(i);

  auto calc_score = [&]() {
    ll score = 0;
    for (int i = 0; i < n; i++) {
      if (used_id[i]) score += c[i];
    }
    return score;
  };

  auto calc_score_delta_1 = [&](int rem_id, int add_id_1, int add_id_2) {
    return -c[rem_id] + (!used_id[add_id_1] ? c[add_id_1] : 0) + (!used_id[add_id_2] ? c[add_id_2] : 0);
  };

  auto calc_score_delta_2 = [&](int rem_id_1, int rem_id_2, int add_id_1, int add_id_2) {
    return -c[rem_id_1] + (used_id[rem_id_2] ? -c[rem_id_2] : 0) + (!used_id[add_id_1] ? c[add_id_1] : 0) + (!used_id[add_id_2] ? c[add_id_2] : 0);
  };

  ll score = calc_score();
  ll last_score = score;
  ll best_score = score;

  double base_temperature = 1e9;
  double target_temperature = 1e6;
  double temperature = base_temperature;
  // double decay_rate = 3.0e-6;
  double time_start = theTimer.time();
  double time_limit = 1.950;
  int iter_count = 0;
  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.50) {
      int rem_id = used_set.included[theRandom.nextUIntMod(used_set.included_size())];
      int add_id_1 = row_id[a[rem_id]-1][theRandom.nextUIntMod(row_id[a[rem_id]-1].size())];
      int add_id_2 = col_id[b[rem_id]-1][theRandom.nextUIntMod(col_id[b[rem_id]-1].size())];
      if (rem_id == add_id_1 || rem_id == add_id_2) continue;

      score += calc_score_delta_1(rem_id, add_id_1, add_id_2);

      #ifdef DEBUG
      if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= last_score) {
        used_id[rem_id] = false;
        used_id[add_id_1] = true;
        used_id[add_id_2] = true;
        used_set.exclude_element(rem_id);
        used_set.include_element(add_id_1);
        used_set.include_element(add_id_2);
        last_score = score;
        if (score < best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
        used_id[rem_id] = false;
        used_id[add_id_1] = true;
        used_id[add_id_2] = true;
        used_set.exclude_element(rem_id);
        used_set.include_element(add_id_1);
        used_set.include_element(add_id_2);
        last_score = score;
      }
      else { // rollback
        score = last_score;
      }
    }
    else if (roll < 1.00) {
      int rem_id_1 = used_set.included[theRandom.nextUIntMod(used_set.included_size())];
      int add_id_1 = row_id[a[rem_id_1]-1][theRandom.nextUIntMod(row_id[a[rem_id_1]-1].size())];
      int add_id_2 = col_id[b[rem_id_1]-1][theRandom.nextUIntMod(col_id[b[rem_id_1]-1].size())];
      int rem_id_2 = board[a[add_id_2]-1][b[add_id_1]-1];
      if (rem_id_1 == add_id_1 || rem_id_1 == add_id_2 || rem_id_2 == -1) continue;

      score += calc_score_delta_2(rem_id_1, rem_id_2, add_id_1, add_id_2);

      #ifdef DEBUG
      if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= last_score) {
        used_id[rem_id_1] = false;
        used_id[add_id_1] = true;
        used_id[add_id_2] = true;
        used_id[rem_id_2] = false;
        used_set.exclude_element(rem_id_1);
        used_set.include_element(add_id_1);
        used_set.include_element(add_id_2);
        used_set.exclude_element(rem_id_2);
        last_score = score;
        if (score < best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
        used_id[rem_id_1] = false;
        used_id[add_id_1] = true;
        used_id[add_id_2] = true;
        used_id[rem_id_2] = false;
        used_set.exclude_element(rem_id_1);
        used_set.include_element(add_id_1);
        used_set.include_element(add_id_2);
        used_set.exclude_element(rem_id_2);
        last_score = score;
      }
      else { // rollback
        score = last_score;
      }
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

  ans = best_score;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  cin >> h >> w >> n;
  a = vector<ll>(n);
  b = vector<ll>(n);
  c = vector<ll>(n);
  for (int i = 0; i < n; i++) cin >> a[i] >> b[i] >> c[i];

  solve();

  cout << ans << endl;

  return 0;
}
