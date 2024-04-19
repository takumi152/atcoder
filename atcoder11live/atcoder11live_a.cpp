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
int board_size;
Pii start_pos;
vector<vector<bool>> initial_board;

// outputs
vector<Pii> ans;

// internals
vector<vector<bool>> board;

void get_first_input() {
  cin >> board_size;
  cin >> start_pos.first >> start_pos.second;
  initial_board = vector<vector<bool>>(board_size, vector<bool>(board_size));
  for (int i = 0; i < board_size; i++) {
    string b;
    cin >> b;
    for (int j = 0; j < board_size; j++) {
      initial_board[i][j] = (b[j] == '#') ? true : false;
    }
  }
}

void init() {
  board = initial_board;
}

bool within_board(int x, int y) {
  return 0 <= x && x < board_size && 0 <= y && y < board_size;
}

int get_visited_index(int dir, int x, int y) {
  return dir * board_size * board_size + x * board_size + y;
}

void solve() {
  vector<bool> visited(4 * board_size * board_size);
  vector<int> visited_count(4 * board_size * board_size);
  int wall_count = 0;
  auto calc_score = [&]() {
    int score = 0;
    int cd = 0;
    int cx = start_pos.first;
    int cy = start_pos.second;
    while (!visited[get_visited_index(cd, cx, cy)]) {
      visited_count[get_visited_index(cd, cx, cy)]++;
      score += visited_count[get_visited_index(cd, cx, cy)];
      visited[get_visited_index(cd, cx, cy)] = true;
      int nx = cx + delta.x[cd];
      int ny = cy + delta.y[cd];
      if (!within_board(nx, ny) || board[nx][ny]) {
        cd = (cd + 1) & 3;
      }
      else {
        cx = nx;
        cy = ny;
        score++;
      }
    }
    return score;
  };

  auto reset_visited = [&]() {
    int cd = 0;
    int cx = start_pos.first;
    int cy = start_pos.second;
    while (visited[get_visited_index(cd, cx, cy)]) {
      visited_count[get_visited_index(cd, cx, cy)]--;
      visited[get_visited_index(cd, cx, cy)] = false;
      int nx = cx + delta.x[cd];
      int ny = cy + delta.y[cd];
      if (!within_board(nx, ny) || board[nx][ny]) {
        cd = (cd + 1) & 3;
      }
      else {
        cx = nx;
        cy = ny;
      }
    }
  };

  auto generate_ans = [&]() {
    vector<Pii> ans;
    for (int i = 0; i < board_size; i++) {
      for (int j = 0; j < board_size; j++) {
        if (board[i][j] && !initial_board[i][j]) ans.emplace_back(i, j);
      }
    }
    return ans;
  };
  ans = generate_ans();

  int score = calc_score();
  int last_score = score;
  int best_score = score;

  const double base_temperature = 5e0;
  const double target_temperature = 3e-1;
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;

  int iter_count = 0;
  double time_start = theTimer.time();
  const double time_limit = 19.900;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 1.00) {
      int px = theRandom.nextUIntMod(board_size);
      int py = theRandom.nextUIntMod(board_size);
      if (px == start_pos.first && py == start_pos.second) continue;
      if (initial_board[px][py]) continue;

      wall_count += board[px][py] ? -1 : 1;
      board[px][py] = !board[px][py];

      score = calc_score();
      reset_visited();

      #ifdef DEBUG
      if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        if (score > best_score) {
          best_score = score;
          ans = generate_ans();
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        score = last_score;
        wall_count += board[px][py] ? -1 : 1;
        board[px][py] = !board[px][py];
      }
    }
    else if (roll < 1.00) {
      int px1 = theRandom.nextUIntMod(board_size);
      int py1 = theRandom.nextUIntMod(board_size);
      int px2 = theRandom.nextUIntMod(board_size);
      int py2 = theRandom.nextUIntMod(board_size);
      if (px1 == start_pos.first && py1 == start_pos.second) continue;
      if (px2 == start_pos.first && py2 == start_pos.second) continue;
      if (initial_board[px1][py1]) continue;
      if (initial_board[px2][py2]) continue;
      if (px1 == px2 && py1 == py2) continue;

      wall_count += board[px1][py1] ? -1 : 1;
      wall_count += board[px2][py2] ? -1 : 1;
      board[px1][py1] = !board[px1][py1];
      board[px2][py2] = !board[px2][py2];

      score = calc_score();
      reset_visited();

      #ifdef DEBUG
      if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        if (score > best_score) {
          best_score = score;
          ans = generate_ans();
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        score = last_score;
        wall_count += board[px1][py1] ? -1 : 1;
        wall_count += board[px2][py2] ? -1 : 1;
        board[px1][py1] = !board[px1][py1];
        board[px2][py2] = !board[px2][py2];
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

void output_ans() {
  cout << ans.size() << endl;
  for (auto &x: ans) cout << x.first << " " << x.second << endl;
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
