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
struct position {
  int x;
  int y;

  position() : x(0), y(0) {}
  position(int x, int y) : x(x), y(y) {}

  bool operator==(const position& that) const {
    return this->x == that.x && this->y == that.y;
  }

  bool operator!=(const position& that) const {
    return !(*this == that);
  }

  bool operator<(const position& that) const {
    if (this->x != that.x) return this->x < that.x;
    else return this->y < that.y;
  }
};

// enums

// constants
constexpr int board_size = 30;
constexpr int ball_num = board_size * (board_size + 1) / 2;

struct {
  const vector<int> x = {-1, -1,  0,  1,  1,  0};
  const vector<int> y = {-1,  0,  1,  1,  0, -1};
} delta;

int pos_index[board_size][board_size];
position index_pos[ball_num];

// inputs
int initial_board[board_size][board_size];

// outputs
vector<pair<position, position>> ans;

// internals
position initial_ball_pos[ball_num];

bool allow_sideways[ball_num];

bool within_board(int x, int y) {
  return 0 <= x && x <= board_size && 0 <= y && y <= x;
}

void get_first_input() {
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j <= i; j++) {
      cin >> initial_board[i][j];
    }
  }
}

void init() {
  {
    int p = 0;
    for (int i = 0; i < board_size; i++) {
      for (int j = 0; j <= i; j++) {
        pos_index[i][j] = p;
        index_pos[p] = position(i, j);
        p++;
      }
    }
  }

  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j <= i; j++) {
      initial_ball_pos[initial_board[i][j]] = position(i, j);
    }
  }

  for (int i = 0; i < ball_num; i++) {
    allow_sideways[i] = false;
  }
}

int calc_score() {
  int score = 0;
  vector<pair<position, position>> res;
  vector<pair<int, int>> swapped_nums;

  static int current_board[board_size][board_size];
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j <= i; j++) {
      current_board[i][j] = initial_board[i][j];
    }
  }
  static position current_ball_pos[ball_num];
  for (int i = 0; i < ball_num; i++) {
    current_ball_pos[i] = initial_ball_pos[i];
  }

  for (int i = 0; i < ball_num; i++) {
    static position prev[board_size][board_size];
    for (int j = 0; j < board_size; j++) {
      for (int k = 0; k <= j; k++) {
        prev[j][k] = position(-1, -1);
      }
    }
    position p0 = current_ball_pos[i];
    position p_end;
    priority_queue<tuple<int, int, position, position>> que;
    que.emplace(0, 0, p0, p0);
    while (!que.empty()) {
      auto [dist, num_total, p_now, p_prev] = que.top();
      que.pop();
      if (prev[p_now.x][p_now.y].x != -1) continue;
      prev[p_now.x][p_now.y] = p_prev;
      bool is_end = true;
      if (within_board(p_now.x - 1, p_now.y) && current_board[p_now.x - 1][p_now.y] >= i) {
        if (prev[p_now.x - 1][p_now.y].x == -1) {
          que.emplace(dist - 1, num_total + current_board[p_now.x - 1][p_now.y], position(p_now.x - 1, p_now.y), p_now);
        }
        is_end = false;
      }
      if (within_board(p_now.x - 1, p_now.y - 1) && current_board[p_now.x - 1][p_now.y - 1] >= i) {
        if (prev[p_now.x - 1][p_now.y - 1].x == -1) {
          que.emplace(dist - 1, num_total + current_board[p_now.x - 1][p_now.y - 1], position(p_now.x - 1, p_now.y - 1), p_now);
        }
        is_end = false;
      }
      if (allow_sideways[i] && within_board(p_now.x, p_now.y - 1) && current_board[p_now.x][p_now.y - 1] > i && prev[p_now.x][p_now.y - 1].x == -1) {
        que.emplace(dist - 1, num_total + current_board[p_now.x][p_now.y - 1], position(p_now.x, p_now.y - 1), p_now);
      }
      if (allow_sideways[i] && within_board(p_now.x, p_now.y + 1) && current_board[p_now.x][p_now.y + 1] > i && prev[p_now.x][p_now.y + 1].x == -1) {
        que.emplace(dist - 1, num_total + current_board[p_now.x][p_now.y + 1], position(p_now.x, p_now.y + 1), p_now);
      }
      if (is_end) {
        p_end = p_now;
        break;
      }
    }
    vector<position> moves;
    {
      position p_now = p_end;
      moves.push_back(p_now);
      while (p_now != p0) {
        p_now = prev[p_now.x][p_now.y];
        moves.push_back(p_now);
      }
      reverse(moves.begin(), moves.end());
    }
    for (int j = 0; j < (int) moves.size() - 1; j++) {
      swap(current_ball_pos[current_board[moves[j].x][moves[j].y]], current_ball_pos[current_board[moves[j+1].x][moves[j+1].y]]);
      swap(current_board[moves[j].x][moves[j].y], current_board[moves[j+1].x][moves[j+1].y]);
      score++;
      res.emplace_back(moves[j], moves[j+1]);
      swapped_nums.emplace_back(current_board[moves[j].x][moves[j].y], current_board[moves[j+1].x][moves[j+1].y]);
    }
  }

  auto check_consistent = [&](int num, position pos) {
    return (
      (!within_board(pos.x - 1, pos.y - 1) || num > current_board[pos.x - 1][pos.y - 1]) &&
      (!within_board(pos.x - 1, pos.y) || num > current_board[pos.x - 1][pos.y]) &&
      (!within_board(pos.x + 1, pos.y) || num < current_board[pos.x + 1][pos.y]) &&
      (!within_board(pos.x + 1, pos.y + 1) || num < current_board[pos.x + 1][pos.y - 1])
    );
  };

  for (int i = 0; i < (int) res.size() - 1; i++) {
    if (check_consistent(swapped_nums[i].second, res[i].first) && check_consistent(swapped_nums[i].first, res[i].second)) {
      swap(current_ball_pos[swapped_nums[i].first], current_ball_pos[swapped_nums[i].second]);
      swap(current_board[current_ball_pos[swapped_nums[i].first].x][current_ball_pos[swapped_nums[i].first].y], current_board[current_ball_pos[swapped_nums[i].second].x][current_ball_pos[swapped_nums[i].second].y]);
      score--;
    }
  }

  return score;
}

vector<pair<position, position>> generate_ans() {
  int score = 0;
  vector<pair<position, position>> res;
  vector<pair<int, int>> swapped_nums;

  static int current_board[board_size][board_size];
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j <= i; j++) {
      current_board[i][j] = initial_board[i][j];
    }
  }
  static position current_ball_pos[ball_num];
  for (int i = 0; i < ball_num; i++) {
    current_ball_pos[i] = initial_ball_pos[i];
  }

  for (int i = 0; i < ball_num; i++) {
    static position prev[board_size][board_size];
    for (int j = 0; j < board_size; j++) {
      for (int k = 0; k <= j; k++) {
        prev[j][k] = position(-1, -1);
      }
    }
    position p0 = current_ball_pos[i];
    position p_end;
    priority_queue<tuple<int, int, position, position>> que;
    que.emplace(0, 0, p0, p0);
    while (!que.empty()) {
      auto [dist, num_total, p_now, p_prev] = que.top();
      que.pop();
      if (prev[p_now.x][p_now.y].x != -1) continue;
      prev[p_now.x][p_now.y] = p_prev;
      bool is_end = true;
      if (within_board(p_now.x - 1, p_now.y) && current_board[p_now.x - 1][p_now.y] >= i) {
        if (prev[p_now.x - 1][p_now.y].x == -1) {
          que.emplace(dist - 1, num_total + current_board[p_now.x - 1][p_now.y], position(p_now.x - 1, p_now.y), p_now);
        }
        is_end = false;
      }
      if (within_board(p_now.x - 1, p_now.y - 1) && current_board[p_now.x - 1][p_now.y - 1] >= i) {
        if (prev[p_now.x - 1][p_now.y - 1].x == -1) {
          que.emplace(dist - 1, num_total + current_board[p_now.x - 1][p_now.y - 1], position(p_now.x - 1, p_now.y - 1), p_now);
        }
        is_end = false;
      }
      if (allow_sideways[i] && within_board(p_now.x, p_now.y - 1) && current_board[p_now.x][p_now.y - 1] > i && prev[p_now.x][p_now.y - 1].x == -1) {
        que.emplace(dist - 1, num_total + current_board[p_now.x][p_now.y - 1], position(p_now.x, p_now.y - 1), p_now);
      }
      if (allow_sideways[i] && within_board(p_now.x, p_now.y + 1) && current_board[p_now.x][p_now.y + 1] > i && prev[p_now.x][p_now.y + 1].x == -1) {
        que.emplace(dist - 1, num_total + current_board[p_now.x][p_now.y + 1], position(p_now.x, p_now.y + 1), p_now);
      }
      if (is_end) {
        p_end = p_now;
        break;
      }
    }
    vector<position> moves;
    {
      position p_now = p_end;
      moves.push_back(p_now);
      while (p_now != p0) {
        p_now = prev[p_now.x][p_now.y];
        moves.push_back(p_now);
      }
      reverse(moves.begin(), moves.end());
    }
    for (int j = 0; j < (int) moves.size() - 1; j++) {
      swap(current_ball_pos[current_board[moves[j].x][moves[j].y]], current_ball_pos[current_board[moves[j+1].x][moves[j+1].y]]);
      swap(current_board[moves[j].x][moves[j].y], current_board[moves[j+1].x][moves[j+1].y]);
      score++;
      res.emplace_back(moves[j], moves[j+1]);
      swapped_nums.emplace_back(current_board[moves[j].x][moves[j].y], current_board[moves[j+1].x][moves[j+1].y]);
    }
  }

  auto check_consistent = [&](int num, position pos) {
    return (
      (!within_board(pos.x - 1, pos.y - 1) || num > current_board[pos.x - 1][pos.y - 1]) &&
      (!within_board(pos.x - 1, pos.y) || num > current_board[pos.x - 1][pos.y]) &&
      (!within_board(pos.x + 1, pos.y) || num < current_board[pos.x + 1][pos.y]) &&
      (!within_board(pos.x + 1, pos.y + 1) || num < current_board[pos.x + 1][pos.y - 1])
    );
  };

  for (int i = 0; i < (int) res.size() - 1; i++) {
    if (check_consistent(swapped_nums[i].second, res[i].first) && check_consistent(swapped_nums[i].first, res[i].second)) {
      swap(current_ball_pos[swapped_nums[i].first], current_ball_pos[swapped_nums[i].second]);
      swap(current_board[current_ball_pos[swapped_nums[i].first].x][current_ball_pos[swapped_nums[i].first].y], current_board[current_ball_pos[swapped_nums[i].second].x][current_ball_pos[swapped_nums[i].second].y]);
      score--;
      res.erase(res.begin() + i);
      swapped_nums.erase(swapped_nums.begin() + i);
      i--;
    }
  }

  return res;
}

void solve() {
  int score = calc_score();
  int last_score = score;
  int best_score = score;

  const double base_temperature = 1e-100;
  const double target_temperature = 1e-100;
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;

  int iter_count = 0;
  double time_start = theTimer.time();
  const double time_limit = 1.900;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 1.00) {
      int b = iter_count % ball_num;

      allow_sideways[b] = !allow_sideways[b];

      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 10 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
        allow_sideways[b] = !allow_sideways[b];
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

  ans = generate_ans();
}

void output_ans() {
  cout << ans.size() << endl;
  for (auto &x: ans) cout << x.first.x << " " << x.first.y << " " << x.second.x << " " << x.second.y << endl;
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
