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
#include <memory>
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
mt19937_64 theMersenne(1);

// libs
namespace Lib {
}

// hyper parameters
namespace HyperParameter {
  double base_temperature = 0.0016695915252579086;
  double target_temperature = 6.9252545775679765e-06;
  int initial_solution_loop_num = 1;
  int old_route_length_max = 4;
  int new_route_length_max = 9;
  double flip_route_chance = 0.024704833786751933;
  double route_double_time_factor = 0.6602168443324112;

  void load_hyper_parameter(int argc, char *argv[]) {
    if (argc > 1) base_temperature = strtod(argv[1], nullptr);
    if (argc > 2) target_temperature = strtod(argv[2], nullptr);
    if (argc > 3) initial_solution_loop_num = atoi(argv[3]);
    if (argc > 4) old_route_length_max = atoi(argv[4]);
    if (argc > 5) new_route_length_max = atoi(argv[5]);
    if (argc > 6) flip_route_chance = strtod(argv[6], nullptr);
    if (argc > 7) route_double_time_factor = strtod(argv[7], nullptr);
  }
}

// structs
template<typename T> struct fenwick_tree {
  int n;
  vector<T> data;

  fenwick_tree() {
    init(0);
  }

  fenwick_tree(int s) {
    init(s);
  }

  fenwick_tree(vector<T> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) add(i, v[i]);
  }

  void init(int s) {
    n = s;
    data = vector<T>(s);
  }

  void add(int p, T v) {
    p++;
    while (p <= n) {
      data[p-1] += v;
      p += p & -p;
    }
  }

  T sum(int l, int r) {
    return sum(r) - sum(l);
  }

  T sum(int r) {
    T s = 0;
    while (r > 0) {
      s += data[r-1];
      r -= r & -r;
    }
    return s;
  }
};

// enums

// constants
struct Adj4 {
  static constexpr array<int, 4>  dx = { -1,   0,   1,   0};
  static constexpr array<int, 4>  dy = {  0,  -1,   0,   1};
  static constexpr array<char, 4> ch = {'U', 'L', 'D', 'R'};
  static int reverse(const int dir) {
    assert(0 <= dir && dir < 4);
    return dir ^ 2;
  }
};

struct Consts {
  static constexpr int turn_limit = 100000;
};

// inputs
struct Input {
  int board_size;
  vector<vector<vector<bool>>> has_wall; // x, y, dir
  vector<vector<ll>> dirtiness_factor;

  ll dirtiness_factor_sum;
};

// outputs
struct Output {
  string movement;
};

// internals

inline bool within_board(const Input &input, int x, int y) {
  return 0 <= x && x < input.board_size && 0 <= y && y < input.board_size;
}

Input get_input() {
  Input input;

  cin >> input.board_size;

  input.has_wall = vector<vector<vector<bool>>>(input.board_size, vector<vector<bool>>(input.board_size, vector<bool>(4)));
  for (int j = 0; j < input.board_size; j++) {
    // 上下端の壁
    input.has_wall[0][j][0] = true;
    input.has_wall[input.board_size - 1][j][2] = true;
  }
  for (int i = 0; i < input.board_size - 1; i++) {
    for (int j = 0; j < input.board_size; j++) {
      char w;
      cin >> w;
      if (w == '1') {
        // 横方向の壁
        input.has_wall[i][j][2] = true;
        input.has_wall[i + 1][j][0] = true;
      }
    }
  }
  for (int i = 0; i < input.board_size; i++) {
    // 左右端の壁
    input.has_wall[i][0][1] = true;
    input.has_wall[i][input.board_size - 1][3] = true;
  }
  for (int i = 0; i < input.board_size; i++) {
    for (int j = 0; j < input.board_size - 1; j++) {
      char w;
      cin >> w;
      if (w == '1') {
        // 縦方向の壁
        input.has_wall[i][j][3] = true;
        input.has_wall[i][j + 1][1] = true;
      }
    }
  }

  input.dirtiness_factor = vector<vector<ll>>(input.board_size, vector<ll>(input.board_size));
  for (int i = 0; i < input.board_size; i++) {
    for (int j = 0; j < input.board_size; j++) {
      cin >> input.dirtiness_factor[i][j];
    }
  }

  return input;
}

void init(Input& input) {
  input.dirtiness_factor_sum = 0;
  for (int i = 0; i < input.board_size; i++) {
    for (int j = 0; j < input.board_size; j++) {
      input.dirtiness_factor_sum += input.dirtiness_factor[i][j];
    }
  }
}

Output solve(const Input& input) {
  struct SAState {
    const Input& input;

    int total_turn;
    vector<vector<vector<int>>> turn_from_prev_visit;
    vector<vector<vector<int>>> next_direction; 

    vector<vector<ll>> cell_dirtiness;
    ll total_dirtiness;

    SAState(const Input& input): input(input) {
      total_turn = 0;
      turn_from_prev_visit = vector<vector<vector<int>>>(input.board_size, vector<vector<int>>(input.board_size));
      next_direction = vector<vector<vector<int>>>(input.board_size, vector<vector<int>>(input.board_size));

      cell_dirtiness = vector<vector<ll>>(input.board_size, vector<ll>(input.board_size));
      total_dirtiness = 0;

      for (int i = 0; i < HyperParameter::initial_solution_loop_num; i++) create_initial_solution();

      update_score_full();
    }

    void create_initial_solution() {
      // dfsで初期解を作成
      int turn = total_turn;
      vector<vector<bool>> visited(input.board_size, vector<bool>(input.board_size));
      auto dfs = [&](auto self, Pii pos_now, int prev_dir) {
        if (false) return;
        auto &[x, y] = pos_now;
        visited[x][y] = true;
        vector<int> dir_perm = {0, 1, 2, 3};
        shuffle(dir_perm.begin(), dir_perm.end(), theMersenne);
        for (int d_idx = 0; d_idx < 4; d_idx++) {
          int d = dir_perm[d_idx];
          if (input.has_wall[x][y][d] || !within_board(input, x + Adj4::dx[d], y + Adj4::dy[d])) continue;
          if (visited[x + Adj4::dx[d]][y + Adj4::dy[d]]) continue;
          int last_visited_turn = 0;
          for (auto &turn_delta: turn_from_prev_visit[x][y]) {
            last_visited_turn += turn_delta;
          }
          turn_from_prev_visit[x][y].push_back(turn - last_visited_turn);
          next_direction[x][y].push_back(d);
          turn++;
          self(self, Pii(x + Adj4::dx[d], y + Adj4::dy[d]), Adj4::reverse(d));
        }
        if (prev_dir != -1) {
          int last_visited_turn = 0;
          for (auto &turn_delta: turn_from_prev_visit[x][y]) {
            last_visited_turn += turn_delta;
          }
          turn_from_prev_visit[x][y].push_back(turn - last_visited_turn);
          next_direction[x][y].push_back(prev_dir);
          turn++;
        }
      };
      dfs(dfs, Pii(0, 0), -1);
      total_turn = turn;
    }

    void update_cell(int x, int y, int begin_turn, int turn_to_add) {
      int turn = 0;
      for (int i = 0; i < (int) turn_from_prev_visit[x][y].size(); i++) {
        turn += turn_from_prev_visit[x][y][i];
        if (turn >= begin_turn) {
          turn_from_prev_visit[x][y][i] += turn_to_add;
          break;
        }
      }
    }

    void insert_cell(int x, int y, int dir, int inserting_turn) {
      int prev_turn = 0;
      int turn = 0;
      int idx = 0;
      for (; idx <= (int) turn_from_prev_visit[x][y].size(); idx++) {
        if (idx == (int) turn_from_prev_visit[x][y].size()) break;
        turn += turn_from_prev_visit[x][y][idx];
        if (turn >= inserting_turn) {
          turn_from_prev_visit[x][y][idx] = turn - inserting_turn;
          break;
        }
        prev_turn = turn;
      }
      turn_from_prev_visit[x][y].insert(turn_from_prev_visit[x][y].begin() + idx, inserting_turn - prev_turn);
      next_direction[x][y].insert(next_direction[x][y].begin() + idx, dir);
    }

    void remove_cell(int x, int y, int remove_begin_turn) {
      int turn = 0;
      for (int i = 0; i < (int) turn_from_prev_visit[x][y].size(); i++) {
        turn += turn_from_prev_visit[x][y][i];
        if (turn >= remove_begin_turn) {
          if (i + 1 < (int) turn_from_prev_visit[x][y].size()) {
            turn_from_prev_visit[x][y][i + 1] += turn_from_prev_visit[x][y][i];
          }
          turn_from_prev_visit[x][y].erase(turn_from_prev_visit[x][y].begin() + i);
          next_direction[x][y].erase(next_direction[x][y].begin() + i);
          return;
        }
      }
      assert(false);
    }

    void update_score_full() {
      total_dirtiness = 0;
      for (int i = 0; i < input.board_size; i++) {
        for (int j = 0; j < input.board_size; j++) {
          cell_dirtiness[i][j] = 0;
          assert(!turn_from_prev_visit[i][j].empty());
          int turn = turn_from_prev_visit[i][j][0];
          for (int k = 1; k < (int) turn_from_prev_visit[i][j].size(); k++) {
            cell_dirtiness[i][j] += (ll) turn_from_prev_visit[i][j][k] * ((ll) turn_from_prev_visit[i][j][k] - 1LL) / 2LL * input.dirtiness_factor[i][j];
            turn += turn_from_prev_visit[i][j][k];
          }
          ll turn_after_last = (total_turn - turn) + turn_from_prev_visit[i][j][0];
          cell_dirtiness[i][j] += turn_after_last * (turn_after_last - 1LL) / 2LL * input.dirtiness_factor[i][j];
          total_dirtiness += cell_dirtiness[i][j];
        }
      }
    }

    double get_score() {
      return (double) total_dirtiness / (double) (total_turn * input.board_size * input.board_size * input.dirtiness_factor_sum);
    }

    Output create_output() {
      Output output;
      int x = 0;
      int y = 0;
      vector<vector<int>> visited_num(input.board_size, vector<int>(input.board_size));
      while (true) {
        assert(within_board(input, x, y));
        if (x == 0 && y == 0 && visited_num[x][y] == (int) turn_from_prev_visit[x][y].size()) break;
        assert(visited_num[x][y] < (int) turn_from_prev_visit[x][y].size());
        int d = next_direction[x][y][visited_num[x][y]];
        output.movement.push_back(Adj4::ch[d]);
        visited_num[x][y]++;
        x += Adj4::dx[d];
        y += Adj4::dy[d];
      }
      return output;
    }
  };
  SAState state(input);
  {
    double score = state.get_score();
    double last_score = score;
    double best_score = score;

    const double base_temperature = HyperParameter::base_temperature;
    const double target_temperature = HyperParameter::target_temperature;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.980;

    bool route_doubled = false;

    while (theTimer.time() < time_limit) {
      if (!route_doubled && theTimer.time() > time_limit * HyperParameter::route_double_time_factor) {
        for (int i = 0; i < input.board_size; i++) {
          for (int j = 0; j < input.board_size; j++) {
            int turn = 0;
            int visit_num = state.turn_from_prev_visit[i][j].size();
            for (int k = 0; k < visit_num; k++) {
              turn += state.turn_from_prev_visit[i][j][k];
            }
            for (int k = 0; k < visit_num; k++) {
              if (k == 0) state.turn_from_prev_visit[i][j].push_back((state.total_turn - turn) + state.turn_from_prev_visit[i][j][k]);
              else state.turn_from_prev_visit[i][j].push_back(state.turn_from_prev_visit[i][j][k]);
              state.next_direction[i][j].push_back(state.next_direction[i][j][k]);
            }
          }
        }
        state.total_turn *= 2;
        route_doubled = true;
      }
      double roll = theRandom.nextDouble();
      if (roll < HyperParameter::flip_route_chance) {
        const int x0 = theRandom.nextUIntMod(input.board_size);
        const int y0 = theRandom.nextUIntMod(input.board_size);
        if (state.turn_from_prev_visit[x0][y0].size() < 2) continue;
        const int idx = theRandom.nextUIntMod(state.turn_from_prev_visit[x0][y0].size() - 1);

        int begin_turn = 0;
        for (int i = 0; i <= idx; i++) begin_turn += state.turn_from_prev_visit[x0][y0][i];

        vector<tuple<int, int, char>> old_route_order;
        old_route_order.emplace_back(x0, y0, state.next_direction[x0][y0][idx]);
        {
          int now_x = x0 + Adj4::dx[state.next_direction[x0][y0][idx]];
          int now_y = y0 + Adj4::dy[state.next_direction[x0][y0][idx]];
          int turn = begin_turn + 1;
          while (!(now_x == x0 && now_y == y0)) {
            int turn_sum = 0;
            int idx_now = 0;
            for (; idx_now < (int) state.turn_from_prev_visit[now_x][now_y].size(); idx_now++) {
              turn_sum += state.turn_from_prev_visit[now_x][now_y][idx_now];
              if (turn_sum >= turn) break;
            }
            int d = state.next_direction[now_x][now_y][idx_now];
            old_route_order.emplace_back(now_x, now_y, d);
            now_x += Adj4::dx[d];
            now_y += Adj4::dy[d];
            turn++;
          }
        }

        vector<tuple<int, int, char>> new_route_order;
        {
          int now_x = x0;
          int now_y = y0;
          for (int i = (int) old_route_order.size() - 1; i >= 0; i--) {
            const auto &[next_x, next_y, d] = old_route_order[i];
            new_route_order.emplace_back(now_x, now_y, Adj4::reverse(d));
            now_x = next_x;
            now_y = next_y;
          }
        }

        // 以前のルートを消す
        for (int i = 1; i < (int) old_route_order.size(); i++) {
          const auto &[x, y, d] = old_route_order[i];
          state.remove_cell(x, y, begin_turn + i);
        }

        // 新しいルートを挿入
        state.next_direction[x0][y0][idx] = get<2>(new_route_order[0]);
        for (int i = 1; i < (int) new_route_order.size(); i++) {
          const auto &[x, y, d] = new_route_order[i];
          state.insert_cell(x, y, d, begin_turn + i);
        }

        state.update_score_full();
        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          for (int i = 1; i < (int) new_route_order.size(); i++) {
            const auto &[x, y, d] = new_route_order[i];
            state.remove_cell(x, y, begin_turn + i);
          }

          state.next_direction[x0][y0][idx] = get<2>(old_route_order[0]);
          for (int i = 1; i < (int) old_route_order.size(); i++) {
            const auto &[x, y, d] = old_route_order[i];
            state.insert_cell(x, y, d, begin_turn + i);
          }

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) < 1e0);
          score = last_score;
        }
      }
      else if (roll < 1.00) {
        const int x0 = theRandom.nextUIntMod(input.board_size);
        const int y0 = theRandom.nextUIntMod(input.board_size);
        const int idx = theRandom.nextUIntMod(state.turn_from_prev_visit[x0][y0].size());
        const int t = theRandom.nextUIntMod(HyperParameter::old_route_length_max) + 1;

        int begin_turn = 0;
        for (int i = 0; i <= idx; i++) {
          begin_turn += state.turn_from_prev_visit[x0][y0][i];
        }
        if (begin_turn + t >= state.total_turn) continue;
        
        int xe, ye;
        vector<tuple<int, int, char>> old_route_order;
        old_route_order.emplace_back(x0, y0, state.next_direction[x0][y0][idx]);
        {
          unordered_map<int, int> visit_num_reduction;
          int now_x = x0 + Adj4::dx[state.next_direction[x0][y0][idx]];
          int now_y = y0 + Adj4::dy[state.next_direction[x0][y0][idx]];
          for (int i = 1; i <= t; i++) {
            visit_num_reduction[(now_x << 8) | now_y]++;
            int turn_sum = 0;
            int idx_now = 0;
            for (; idx_now < (int) state.turn_from_prev_visit[now_x][now_y].size(); idx_now++) {
              turn_sum += state.turn_from_prev_visit[now_x][now_y][idx_now];
              if (turn_sum >= begin_turn + i) break;
            }
            int d = state.next_direction[now_x][now_y][idx_now];
            old_route_order.emplace_back(now_x, now_y, d);
            now_x += Adj4::dx[d];
            now_y += Adj4::dy[d];
          }
          xe = now_x;
          ye = now_y;
          bool good = true;
          for (auto &[p, n]: visit_num_reduction) {
            int x = p >> 8;
            int y = p & 0xff;
            if (n == (int) state.turn_from_prev_visit[x][y].size()) {
              good = false;
              break;
            }
          }
          if (!good) continue; // 少なくとも1箇所が掃除されなくなる
        }

        vector<tuple<int, int, char>> new_route_order;
        {
          const int t_limit = HyperParameter::new_route_length_max;
          int now_x = x0;
          int now_y = y0;
          for (int i = 0; i < t_limit; i++) {
            int d;
            do {
              d = theRandom.nextUIntMod(4);
            } while (input.has_wall[now_x][now_y][d]);
            new_route_order.emplace_back(now_x, now_y, d);
            now_x += Adj4::dx[d];
            now_y += Adj4::dy[d];
            if (now_x == xe && now_y == ye) break;
          }
          if (!(now_x == xe && now_y == ye)) continue;
        }

        int length_delta = (int) new_route_order.size() - (int) old_route_order.size();
        if (state.total_turn + length_delta > Consts::turn_limit && length_delta > 0) continue;

        // 以前のルートを消す
        for (int i = 1; i < (int) old_route_order.size(); i++) {
          const auto &[x, y, d] = old_route_order[i];
          state.remove_cell(x, y, begin_turn + i);
        }

        for (int i = 0; i < input.board_size; i++) {
          for (int j = 0; j < input.board_size; j++) {
            state.update_cell(i, j, begin_turn + 1, length_delta);
          }
        }

        // 新しいルートを挿入
        state.next_direction[x0][y0][idx] = get<2>(new_route_order[0]);
        for (int i = 1; i < (int) new_route_order.size(); i++) {
          const auto &[x, y, d] = new_route_order[i];
          state.insert_cell(x, y, d, begin_turn + i);
        }

        state.total_turn += length_delta;

        state.update_score_full();
        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state.total_turn -= length_delta;

          for (int i = 1; i < (int) new_route_order.size(); i++) {
            const auto &[x, y, d] = new_route_order[i];
            state.remove_cell(x, y, begin_turn + i);
          }

          for (int i = 0; i < input.board_size; i++) {
            for (int j = 0; j < input.board_size; j++) {
              state.update_cell(i, j, begin_turn + 1, -length_delta);
            }
          }

          state.next_direction[x0][y0][idx] = get<2>(old_route_order[0]);
          for (int i = 1; i < (int) old_route_order.size(); i++) {
            const auto &[x, y, d] = old_route_order[i];
            state.insert_cell(x, y, d, begin_turn + i);
          }

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) < 1e0);
          score = last_score;
        }
      }

      // temperature *= 1.0 - decay_rate;
      temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      iter_count++;
    }

    cerr << "iter_count  = " << iter_count << endl;
    cerr << "last_score  = " << last_score << endl;
    cerr << "best_score  = " << best_score << endl;
    cerr << "temperature = " << temperature << endl;
  }

  return state.create_output();
}

void print_output(const Output& output) {
  cout << output.movement << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  HyperParameter::load_hyper_parameter(argc, argv);

  auto input = get_input();

  init(input);

  auto output = solve(input);

  print_output(output);

  return 0;
}
