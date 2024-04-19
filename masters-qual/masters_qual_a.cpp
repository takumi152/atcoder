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

// libraries
namespace Lib {
}

// hyper parameters
namespace HyperParameter {
  static vector<double> base_temperature1 = {
    5e2,
    5e2,
    2e3,
    5e3,
    5e3,
    5e3,
    1e4,
    1e4,
    2e4,
    5e4,
    5e4,
    1e5,
    1e5,
    1e5,
    2e5,
    4e5,
    2e5,
    5e5,
    5e5,
    2e7,
  };
  static vector<double> target_temperature1 = {
    1e1,
    2e1,
    5e1,
    5e1,
    5e1,
    5e1,
    1e2,
    1e2,
    3e2,
    3e2,
    3e2,
    5e2,
    5e2,
    1e3,
    1e3,
    1e4,
    1e4,
    1e4,
    1e4,
    5e4,
  };
  static vector<double> base_temperature2 = {
    5e2,
    5e2,
    2e3,
    5e3,
    5e3,
    5e3,
    5e3,
    5e3,
    5e3,
    1e4,
    2e4,
    3e4,
    3e4,
    3e4,
    5e4,
    5e4,
    5e4,
    1e5,
    1e5,
    1e7,
  };
  static vector<double> target_temperature2 = {
    1e1,
    2e1,
    5e1,
    5e1,
    5e1,
    5e1,
    1e2,
    1e2,
    3e2,
    3e2,
    3e2,
    5e2,
    5e2,
    1e3,
    1e3,
    1e4,
    1e4,
    1e4,
    1e4,
    5e4,
  };
  void load_hyper_parameter(int argc, char *argv[]) {
    // do nothing
  }
}

// enums

// constants
namespace Consts {
  static const vector<int> dx = { 1,  0, -1,  0};
  static const vector<int> dy = { 0,  1,  0, -1};
  static const vector<char> dc = {'D', 'R', 'U', 'L'};
};

// structs
struct Position {
  int x, y;
  Position(): x(0), y(0) {}
  Position(int x, int y): x(x), y(y) {}
};

struct TurnAction {
  bool do_swap;
  char agent0_dir;
  char agent1_dir;
  TurnAction(): do_swap(false), agent0_dir('.'), agent1_dir('.') {}
  TurnAction(bool do_swap, char agent0_dir, char agent1_dir): do_swap(do_swap), agent0_dir(agent0_dir), agent1_dir(agent1_dir) {}
};

struct SwapInfo {
  int turn;
  Position other_pos;
  int other_idx;
  SwapInfo(): turn(0), other_pos(), other_idx(0) {}
  SwapInfo(int turn, Position other_pos, int other_idx): turn(turn), other_pos(other_pos), other_idx(other_idx) {}
};

// inputs
struct Input {
  int case_type;
  int board_size;
  vector<vector<bool>> wall_vertical;
  vector<vector<bool>> wall_horizontal;
  vector<vector<int>> initial_number;
};

struct Environment {
  int case_type;
  int board_size;
  vector<vector<bool>> wall_vertical;
  vector<vector<bool>> wall_horizontal;
  vector<vector<int>> initial_number;

  int turn_limit;

  vector<vector<Position>> positions_at_turn; // agent_id, turn
  vector<vector<vector<SwapInfo>>> swap_infos; // x, y, swap_info
  vector<vector<int>> swap_info_idxs; // agent_id, swap_info_idx_of_turn

  inline bool within_board(const int x, const int y) const {
    return 0 <= x && x < board_size && 0 <= y && y < board_size;
  }

  inline bool can_move_to(const int x, const int y, const int d) const {
    if (!(within_board(x, y) && within_board(x + Consts::dx[d], y + Consts::dy[d]))) return false;
    if (d == 0) return !wall_horizontal[x][y];
    if (d == 1) return !wall_vertical[x][y];
    if (d == 2) return !wall_horizontal[x - 1][y];
    if (d == 3) return !wall_vertical[x][y - 1];
    return false;
  }
};

// outputs
struct Output {
  vector<Position> initial_positions;
  vector<TurnAction> actions;
};

// internals

void print_output(const Output& output);

Input get_input() {
  Input input;
  cin >> input.case_type >> input.board_size;
  input.wall_vertical = vector<vector<bool>>(input.board_size, vector<bool>(input.board_size - 1));
  input.wall_horizontal = vector<vector<bool>>(input.board_size - 1, vector<bool>(input.board_size));
  for (int i = 0; i < input.board_size; i++) {
    for (int j = 0; j < input.board_size - 1; j++) {
      char c;
      cin >> c;
      input.wall_vertical[i][j] = (c == '1');
    }
  }
  for (int i = 0; i < input.board_size - 1; i++) {
    for (int j = 0; j < input.board_size; j++) {
      char c;
      cin >> c;
      input.wall_horizontal[i][j] = (c == '1');
    }
  }
  input.initial_number = vector<vector<int>>(input.board_size, vector<int>(input.board_size));
  for (int i = 0; i < input.board_size; i++) {
    for (int j = 0; j < input.board_size; j++) {
      cin >> input.initial_number[i][j];
    }
  }
  return input;
}

Environment init_env(const Input& input) {
  Environment env;
  env.case_type = input.case_type;
  env.board_size = input.board_size;
  env.wall_vertical = input.wall_vertical;
  env.wall_horizontal = input.wall_horizontal;
  env.initial_number = input.initial_number;

  const int turn_limit = 4 * input.board_size * input.board_size;
  env.turn_limit = turn_limit;

  // 移動経路を適当に決める
  vector<vector<Position>> positions_at_turn = vector<vector<Position>>(2, vector<Position>());
  for (int i = 0; i < 2; i++) {
    int x = theRandom.nextUIntMod(input.board_size);
    int y = theRandom.nextUIntMod(input.board_size);
    positions_at_turn[i].emplace_back(x, y);
    while ((int) positions_at_turn[i].size() <= turn_limit) {
      // DFSで適当に全マスを1度以上通るような経路を作る
      vector<vector<bool>> visited(input.board_size, vector<bool>(input.board_size));
      auto dfs = [&](auto self, int x, int y) -> bool {
        if (visited[x][y]) return false;
        visited[x][y] = true;
        positions_at_turn[i].emplace_back(x, y);
        if (theRandom.nextDouble() < 0.01) positions_at_turn[i].emplace_back(x, y); // たまにその場にとどまるようにする

        vector<int> perm = {0, 1, 2, 3};
        shuffle(perm.begin(), perm.end(), theMersenne);
        for (int d = 0; d < 4; d++) {
          int nx = x + Consts::dx[perm[d]];
          int ny = y + Consts::dy[perm[d]];
          if (env.can_move_to(x, y, perm[d]) && !visited[nx][ny]) {
            bool did_visit = self(self, nx, ny);
            if (did_visit) positions_at_turn[i].emplace_back(x, y);
          }
        }

        return true;
      };
      dfs(dfs, x, y);
    }
    if ((int) positions_at_turn[i].size() > turn_limit + 1) {
      positions_at_turn[i].erase(positions_at_turn[i].begin() + turn_limit + 1, positions_at_turn[i].end());
    }
  }
  env.positions_at_turn = positions_at_turn;

  vector<vector<vector<SwapInfo>>> swap_infos = vector<vector<vector<SwapInfo>>>(input.board_size, vector<vector<SwapInfo>>(input.board_size));
  vector<vector<int>> swap_info_idxs = vector<vector<int>>(2, vector<int>(turn_limit));
  for (int j = 0; j < turn_limit; j++) {
    int agent0_idx = swap_infos[positions_at_turn[0][j].x][positions_at_turn[0][j].y].size();
    int agent1_idx = swap_infos[positions_at_turn[1][j].x][positions_at_turn[1][j].y].size();
    swap_infos[positions_at_turn[0][j].x][positions_at_turn[0][j].y].emplace_back(j, positions_at_turn[1][j], agent1_idx);
    swap_infos[positions_at_turn[1][j].x][positions_at_turn[1][j].y].emplace_back(j, positions_at_turn[0][j], agent0_idx);
    swap_info_idxs[0][j] = agent0_idx;
    swap_info_idxs[1][j] = agent1_idx;
  }
  env.swap_infos = swap_infos;
  env.swap_info_idxs = swap_info_idxs;

  return env;
}

void print_sa_progress(int iter_count, double score, double last_score, double best_score, double temperature) {
  cerr << "iter_count  = " << setw(8) << iter_count
       << ", score = " << fixed << setprecision(3) << setw(12) << score
       << ", last_score = " << fixed << setprecision(3) << setw(12) << last_score
       << ", best_score = " << fixed << setprecision(3) << setw(12) << best_score
       << ", temperature = " << fixed << setprecision(3) << setw(12) << temperature
       << ", time = " << fixed << setprecision(3) << setw(7) << theTimer.time() << endl;
}

Output solve(const Environment& env) {
  const bool vis_output = true;

  struct SAState1 {
    const Environment& env;

    vector<bool> do_swap;

    vector<vector<vector<int>>> number_before_swap; // x, y, number_before_nth_swap

    vector<vector<int>> target_number;
    vector<vector<int>> number_after_swap;

    ll score;

    SAState1(const Environment& env): env(env) {
      do_swap = vector<bool>(env.turn_limit, false);

      number_before_swap = vector<vector<vector<int>>>(env.board_size, vector<vector<int>>(env.board_size));
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          number_before_swap[i][j].resize(env.swap_infos[i][j].size());
        }
      }

      number_after_swap = vector<vector<int>>(env.board_size, vector<int>(env.board_size));

      create_initial_solution();
      update_score_full();
    }

    void create_initial_solution() {
      target_number = vector<vector<int>>(env.board_size, vector<int>(env.board_size));
      {
        // BFSで「理想的な値」を求める
        vector<vector<bool>> visited(env.board_size, vector<bool>(env.board_size));
        queue<Position> que;
        que.emplace(0, 0);
        int next_number = 1;
        while (!que.empty()) {
          auto pos = que.front();
          que.pop();
          if (visited[pos.x][pos.y]) continue;
          visited[pos.x][pos.y] = true;
          target_number[pos.x][pos.y] = next_number;
          next_number++;
          for (int d = 0; d < 4; d++) {
            int nx = pos.x + Consts::dx[d];
            int ny = pos.y + Consts::dy[d];
            if (env.can_move_to(pos.x, pos.y, d) && !visited[nx][ny]) {
              que.emplace(nx, ny);
            }
          }
        }
      }

      auto board_status = env.initial_number;
      for (int i = 0; i < env.turn_limit; i++) {
        const Position agent0_pos = env.positions_at_turn[0][i];
        const Position agent1_pos = env.positions_at_turn[1][i];
        // 二乗誤差が小さくなるならswapする
        const int diff0_before = board_status[agent0_pos.x][agent0_pos.y] - target_number[agent0_pos.x][agent0_pos.y];
        const int diff1_before = board_status[agent1_pos.x][agent1_pos.y] - target_number[agent1_pos.x][agent1_pos.y];
        const int sq_diff_before = diff0_before * diff0_before + diff1_before * diff1_before;
        const int diff0_after = board_status[agent1_pos.x][agent1_pos.y] - target_number[agent0_pos.x][agent0_pos.y];
        const int diff1_after = board_status[agent0_pos.x][agent0_pos.y] - target_number[agent1_pos.x][agent1_pos.y];
        const int sq_diff_after = diff0_after * diff0_after + diff1_after * diff1_after;
        if (sq_diff_after < sq_diff_before) {
          do_swap[i] = true;
          swap(board_status[agent0_pos.x][agent0_pos.y], board_status[agent1_pos.x][agent1_pos.y]);
        }
      }
    }

    void update_score_full() {
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          number_after_swap[i][j] = env.initial_number[i][j];
        }
      }

      for (int i = 0; i < env.turn_limit; i++) {
        number_before_swap[env.positions_at_turn[0][i].x][env.positions_at_turn[0][i].y][env.swap_info_idxs[0][i]] = number_after_swap[env.positions_at_turn[0][i].x][env.positions_at_turn[0][i].y];
        number_before_swap[env.positions_at_turn[1][i].x][env.positions_at_turn[1][i].y][env.swap_info_idxs[1][i]] = number_after_swap[env.positions_at_turn[1][i].x][env.positions_at_turn[1][i].y];
        // cerr << i << " " << env.positions_at_turn[0][i].x << " " << env.positions_at_turn[0][i].y << " " << env.swap_info_idxs[0][i] << " " << number_before_swap[env.positions_at_turn[0][i].x][env.positions_at_turn[0][i].y][env.swap_info_idxs[0][i]] << endl;
        // cerr << i << " " << env.positions_at_turn[1][i].x << " " << env.positions_at_turn[1][i].y << " " << env.swap_info_idxs[1][i] << " " << number_before_swap[env.positions_at_turn[1][i].x][env.positions_at_turn[1][i].y][env.swap_info_idxs[1][i]] << endl;
        if (do_swap[i]) {
          swap(number_after_swap[env.positions_at_turn[0][i].x][env.positions_at_turn[0][i].y], number_after_swap[env.positions_at_turn[1][i].x][env.positions_at_turn[1][i].y]);
        }
      }

      score = 0;
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          score += (number_after_swap[i][j] - target_number[i][j]) * (number_after_swap[i][j] - target_number[i][j]);
        }
      }
    }

    void update_score_partial(const int turn) {
      for (int i = 0; i < 2; i++) {
        const auto initial_pos = env.positions_at_turn[i][turn];
        const auto initial_idx = env.swap_info_idxs[i][turn];
        const auto number = number_before_swap[initial_pos.x][initial_pos.y][initial_idx];
        
        auto pos = initial_pos;
        auto idx = initial_idx;
        while (idx < (int) env.swap_infos[pos.x][pos.y].size()) {
          const auto swap_info = env.swap_infos[pos.x][pos.y][idx];
          // cerr << "B " << i << " " << swap_info.turn << " " << pos.x << " " << pos.y << " " << idx << " " << number_before_swap[pos.x][pos.y][idx] << endl;
          number_before_swap[pos.x][pos.y][idx] = number;
          if (!(pos.x == swap_info.other_pos.x && pos.y == swap_info.other_pos.y) && do_swap[swap_info.turn]) {
            pos = swap_info.other_pos;
            idx = swap_info.other_idx;
          }
          // cerr << "A " << i << " " << swap_info.turn << " " << pos.x << " " << pos.y << " " << idx << endl;
          idx++;
        }

        score -= (number_after_swap[pos.x][pos.y] - target_number[pos.x][pos.y]) * (number_after_swap[pos.x][pos.y] - target_number[pos.x][pos.y]);
        number_after_swap[pos.x][pos.y] = number;
        score += (number_after_swap[pos.x][pos.y] - target_number[pos.x][pos.y]) * (number_after_swap[pos.x][pos.y] - target_number[pos.x][pos.y]);
      }
    }

    double get_score() {
      return score;
    }

    Output get_output() {
      Output output;
      output.initial_positions = {env.positions_at_turn[0][0], env.positions_at_turn[1][0]};
      for (int i = 0; i < env.turn_limit; i++) {
        char agent0_dir = '.';
        for (int d = 0; d < 4; d++) {
          if (env.positions_at_turn[0][i].x + Consts::dx[d] == env.positions_at_turn[0][i + 1].x && env.positions_at_turn[0][i].y + Consts::dy[d] == env.positions_at_turn[0][i + 1].y) {
            agent0_dir = Consts::dc[d];
            break;
          }
        }
        char agent1_dir = '.';
        for (int d = 0; d < 4; d++) {
          if (env.positions_at_turn[1][i].x + Consts::dx[d] == env.positions_at_turn[1][i + 1].x && env.positions_at_turn[1][i].y + Consts::dy[d] == env.positions_at_turn[1][i + 1].y) {
            agent1_dir = Consts::dc[d];
            break;
          }
        }
        output.actions.emplace_back(do_swap[i], agent0_dir, agent1_dir);
      }
      return output;
    }
  };

  SAState1 state1(env);
  {
    double score = state1.get_score();
    double last_score = score;
    double best_score = score;

    const double base_temperature = HyperParameter::base_temperature1[env.case_type];
    const double target_temperature = HyperParameter::target_temperature1[env.case_type];
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.000;

    while (true) {
      if (iter_count % 16 == 0 && theTimer.time() > time_limit) break;
      const double roll = theRandom.nextDouble();
      if (roll < 0.5) {
        int i = theRandom.nextUIntMod(env.turn_limit);

        state1.do_swap[i] = !state1.do_swap[i];

        state1.update_score_partial(i);
        score = state1.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) print_sa_progress(iter_count, score, last_score, best_score, temperature);
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
          state1.do_swap[i] = !state1.do_swap[i];
          state1.update_score_partial(i);
          score = last_score;
        }
        // state1.update_score_full();
        // if (abs(last_score - state1.get_score()) != 0) {
        //   cerr << "iter_count  = " << iter_count << endl;
        //   cerr << "i           = " << i << endl;
        //   cerr << "x0          = " << env.positions_at_turn[0][i].x << endl;
        //   cerr << "y0          = " << env.positions_at_turn[0][i].y << endl;
        //   cerr << "x1          = " << env.positions_at_turn[1][i].x << endl;
        //   cerr << "y1          = " << env.positions_at_turn[1][i].y << endl;
        //   cerr << "last_score  = " << last_score << endl;
        //   cerr << "score       = " << state1.get_score() << endl;
        //   assert(false);
        // }
      }
      else if (roll < 0.6) {
        int i1 = theRandom.nextUIntMod(env.turn_limit);
        int i2 = theRandom.nextUIntMod(env.turn_limit);
        if (i1 == i2) continue;

        state1.do_swap[i1] = !state1.do_swap[i1];
        state1.update_score_partial(i1);
        state1.do_swap[i2] = !state1.do_swap[i2];
        state1.update_score_partial(i2);
        score = state1.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) print_sa_progress(iter_count, score, last_score, best_score, temperature);
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
          state1.do_swap[i2] = !state1.do_swap[i2];
          state1.update_score_partial(i2);
          state1.do_swap[i1] = !state1.do_swap[i1];
          state1.update_score_partial(i1);
          score = last_score;
        }
      }
      else if (roll < 1.0) {
        int x = theRandom.nextUIntMod(env.board_size);
        int y = theRandom.nextUIntMod(env.board_size);
        if (env.swap_infos[x][y].size() < 2) continue;
        int i1 = theRandom.nextUIntMod(env.swap_infos[x][y].size());
        int i2 = theRandom.nextUIntMod(env.swap_infos[x][y].size());
        if (i1 == i2) continue;

        int t1 = env.swap_infos[x][y][i1].turn;
        int t2 = env.swap_infos[x][y][i2].turn;

        state1.do_swap[t1] = !state1.do_swap[t1];
        state1.update_score_partial(t1);
        state1.do_swap[t2] = !state1.do_swap[t2];
        state1.update_score_partial(t2);
        score = state1.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) print_sa_progress(iter_count, score, last_score, best_score, temperature);
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
          state1.do_swap[t2] = !state1.do_swap[t2];
          state1.update_score_partial(t2);
          state1.do_swap[t1] = !state1.do_swap[t1];
          state1.update_score_partial(t1);
          score = last_score;
        }
      }

      if (vis_output && iter_count % 100000 == 0) {
        theTimer.stop();
        print_output(state1.get_output());
        theTimer.start();
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

  struct SAState2 {
    const Environment& env;

    vector<bool> do_swap;

    vector<vector<vector<int>>> number_before_swap; // x, y, number_before_nth_swap

    vector<vector<int>> number_after_swap;

    ll score;

    SAState2(const Environment& env, const SAState1& state1): env(env) {
      do_swap = state1.do_swap;

      number_before_swap = vector<vector<vector<int>>>(env.board_size, vector<vector<int>>(env.board_size));
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          number_before_swap[i][j].resize(env.swap_infos[i][j].size());
        }
      }

      number_after_swap = vector<vector<int>>(env.board_size, vector<int>(env.board_size));

      update_score_full();
    }

    void create_initial_solution() {
      vector<vector<int>> target_number = vector<vector<int>>(env.board_size, vector<int>(env.board_size));
      {
        // BFSで「理想的な値」を求める
        vector<vector<bool>> visited(env.board_size, vector<bool>(env.board_size));
        queue<Position> que;
        que.emplace(0, 0);
        int next_number = 1;
        while (!que.empty()) {
          auto pos = que.front();
          que.pop();
          if (visited[pos.x][pos.y]) continue;
          visited[pos.x][pos.y] = true;
          target_number[pos.x][pos.y] = next_number;
          next_number++;
          for (int d = 0; d < 4; d++) {
            int nx = pos.x + Consts::dx[d];
            int ny = pos.y + Consts::dy[d];
            if (env.can_move_to(pos.x, pos.y, d) && !visited[nx][ny]) {
              que.emplace(nx, ny);
            }
          }
        }
      }

      auto board_status = env.initial_number;
      for (int i = 0; i < env.turn_limit; i++) {
        const Position agent0_pos = env.positions_at_turn[0][i];
        const Position agent1_pos = env.positions_at_turn[1][i];
        // 二乗誤差が小さくなるならswapする
        const int diff0_before = board_status[agent0_pos.x][agent0_pos.y] - target_number[agent0_pos.x][agent0_pos.y];
        const int diff1_before = board_status[agent1_pos.x][agent1_pos.y] - target_number[agent1_pos.x][agent1_pos.y];
        const int sq_diff_before = diff0_before * diff0_before + diff1_before * diff1_before;
        const int diff0_after = board_status[agent1_pos.x][agent1_pos.y] - target_number[agent0_pos.x][agent0_pos.y];
        const int diff1_after = board_status[agent0_pos.x][agent0_pos.y] - target_number[agent1_pos.x][agent1_pos.y];
        const int sq_diff_after = diff0_after * diff0_after + diff1_after * diff1_after;
        if (sq_diff_after < sq_diff_before) {
          do_swap[i] = true;
          swap(board_status[agent0_pos.x][agent0_pos.y], board_status[agent1_pos.x][agent1_pos.y]);
        }
      }
    }

    void update_score_full() {
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          number_after_swap[i][j] = env.initial_number[i][j];
        }
      }

      for (int i = 0; i < env.turn_limit; i++) {
        number_before_swap[env.positions_at_turn[0][i].x][env.positions_at_turn[0][i].y][env.swap_info_idxs[0][i]] = number_after_swap[env.positions_at_turn[0][i].x][env.positions_at_turn[0][i].y];
        number_before_swap[env.positions_at_turn[1][i].x][env.positions_at_turn[1][i].y][env.swap_info_idxs[1][i]] = number_after_swap[env.positions_at_turn[1][i].x][env.positions_at_turn[1][i].y];
        // cerr << i << " " << env.positions_at_turn[0][i].x << " " << env.positions_at_turn[0][i].y << " " << env.swap_info_idxs[0][i] << " " << number_before_swap[env.positions_at_turn[0][i].x][env.positions_at_turn[0][i].y][env.swap_info_idxs[0][i]] << endl;
        // cerr << i << " " << env.positions_at_turn[1][i].x << " " << env.positions_at_turn[1][i].y << " " << env.swap_info_idxs[1][i] << " " << number_before_swap[env.positions_at_turn[1][i].x][env.positions_at_turn[1][i].y][env.swap_info_idxs[1][i]] << endl;
        if (do_swap[i]) {
          swap(number_after_swap[env.positions_at_turn[0][i].x][env.positions_at_turn[0][i].y], number_after_swap[env.positions_at_turn[1][i].x][env.positions_at_turn[1][i].y]);
        }
      }

      score = 0;
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          if (i < env.board_size - 1 && !env.wall_horizontal[i][j]) {
            const int diff = number_after_swap[i][j] - number_after_swap[i + 1][j];
            score += diff * diff;
          }
          if (j < env.board_size - 1 && !env.wall_vertical[i][j]) {
            const int diff = number_after_swap[i][j] - number_after_swap[i][j + 1];
            score += diff * diff;
          }
        }
      }
    }

    void update_score_partial(const int turn) {
      for (int i = 0; i < 2; i++) {
        const auto initial_pos = env.positions_at_turn[i][turn];
        const auto initial_idx = env.swap_info_idxs[i][turn];
        const auto number = number_before_swap[initial_pos.x][initial_pos.y][initial_idx];
        
        auto pos = initial_pos;
        auto idx = initial_idx;
        while (idx < (int) env.swap_infos[pos.x][pos.y].size()) {
          const auto swap_info = env.swap_infos[pos.x][pos.y][idx];
          // cerr << "B " << i << " " << swap_info.turn << " " << pos.x << " " << pos.y << " " << idx << " " << number_before_swap[pos.x][pos.y][idx] << endl;
          number_before_swap[pos.x][pos.y][idx] = number;
          if (!(pos.x == swap_info.other_pos.x && pos.y == swap_info.other_pos.y) && do_swap[swap_info.turn]) {
            pos = swap_info.other_pos;
            idx = swap_info.other_idx;
          }
          // cerr << "A " << i << " " << swap_info.turn << " " << pos.x << " " << pos.y << " " << idx << endl;
          idx++;
        }

        if (pos.x > 0 && !env.wall_horizontal[pos.x - 1][pos.y]) {
          const int diff = number_after_swap[pos.x][pos.y] - number_after_swap[pos.x - 1][pos.y];
          score -= diff * diff;
        }
        if (pos.x < env.board_size - 1 && !env.wall_horizontal[pos.x][pos.y]) {
          const int diff = number_after_swap[pos.x][pos.y] - number_after_swap[pos.x + 1][pos.y];
          score -= diff * diff;
        }
        if (pos.y > 0 && !env.wall_vertical[pos.x][pos.y - 1]) {
          const int diff = number_after_swap[pos.x][pos.y] - number_after_swap[pos.x][pos.y - 1];
          score -= diff * diff;
        }
        if (pos.y < env.board_size - 1 && !env.wall_vertical[pos.x][pos.y]) {
          const int diff = number_after_swap[pos.x][pos.y] - number_after_swap[pos.x][pos.y + 1];
          score -= diff * diff;
        }
        number_after_swap[pos.x][pos.y] = number;
        if (pos.x > 0 && !env.wall_horizontal[pos.x - 1][pos.y]) {
          const int diff = number_after_swap[pos.x][pos.y] - number_after_swap[pos.x - 1][pos.y];
          score += diff * diff;
        }
        if (pos.x < env.board_size - 1 && !env.wall_horizontal[pos.x][pos.y]) {
          const int diff = number_after_swap[pos.x][pos.y] - number_after_swap[pos.x + 1][pos.y];
          score += diff * diff;
        }
        if (pos.y > 0 && !env.wall_vertical[pos.x][pos.y - 1]) {
          const int diff = number_after_swap[pos.x][pos.y] - number_after_swap[pos.x][pos.y - 1];
          score += diff * diff;
        }
        if (pos.y < env.board_size - 1 && !env.wall_vertical[pos.x][pos.y]) {
          const int diff = number_after_swap[pos.x][pos.y] - number_after_swap[pos.x][pos.y + 1];
          score += diff * diff;
        }
      }
    }

    double get_score() {
      return score;
    }

    Output get_output() {
      Output output;
      output.initial_positions = {env.positions_at_turn[0][0], env.positions_at_turn[1][0]};
      for (int i = 0; i < env.turn_limit; i++) {
        char agent0_dir = '.';
        for (int d = 0; d < 4; d++) {
          if (env.positions_at_turn[0][i].x + Consts::dx[d] == env.positions_at_turn[0][i + 1].x && env.positions_at_turn[0][i].y + Consts::dy[d] == env.positions_at_turn[0][i + 1].y) {
            agent0_dir = Consts::dc[d];
            break;
          }
        }
        char agent1_dir = '.';
        for (int d = 0; d < 4; d++) {
          if (env.positions_at_turn[1][i].x + Consts::dx[d] == env.positions_at_turn[1][i + 1].x && env.positions_at_turn[1][i].y + Consts::dy[d] == env.positions_at_turn[1][i + 1].y) {
            agent1_dir = Consts::dc[d];
            break;
          }
        }
        output.actions.emplace_back(do_swap[i], agent0_dir, agent1_dir);
      }
      return output;
    }
  };
  SAState2 state2(env, state1);
  {
    double score = state2.get_score();
    double last_score = score;
    double best_score = score;

    const double base_temperature = HyperParameter::base_temperature2[env.case_type];
    const double target_temperature = HyperParameter::target_temperature2[env.case_type];
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.900;

    while (true) {
      if (iter_count % 16 == 0 && theTimer.time() > time_limit) break;
      const double roll = theRandom.nextDouble();
      if (roll < 0.5) {
        int i = theRandom.nextUIntMod(env.turn_limit);

        state2.do_swap[i] = !state2.do_swap[i];

        state2.update_score_partial(i);
        score = state2.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) print_sa_progress(iter_count, score, last_score, best_score, temperature);
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
          state2.do_swap[i] = !state2.do_swap[i];
          state2.update_score_partial(i);
          score = last_score;
        }
        // state2.update_score_full();
        // if (abs(last_score - state2.get_score()) != 0) {
        //   cerr << "iter_count  = " << iter_count << endl;
        //   cerr << "i           = " << i << endl;
        //   cerr << "x0          = " << env.positions_at_turn[0][i].x << endl;
        //   cerr << "y0          = " << env.positions_at_turn[0][i].y << endl;
        //   cerr << "x1          = " << env.positions_at_turn[1][i].x << endl;
        //   cerr << "y1          = " << env.positions_at_turn[1][i].y << endl;
        //   cerr << "last_score  = " << last_score << endl;
        //   cerr << "score       = " << state2.get_score() << endl;
        //   assert(false);
        // }
      }
      else if (roll < 0.6) {
        int i1 = theRandom.nextUIntMod(env.turn_limit);
        int i2 = theRandom.nextUIntMod(env.turn_limit);
        if (i1 == i2) continue;

        state2.do_swap[i1] = !state2.do_swap[i1];
        state2.update_score_partial(i1);
        state2.do_swap[i2] = !state2.do_swap[i2];
        state2.update_score_partial(i2);
        score = state2.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) print_sa_progress(iter_count, score, last_score, best_score, temperature);
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
          state2.do_swap[i2] = !state2.do_swap[i2];
          state2.update_score_partial(i2);
          state2.do_swap[i1] = !state2.do_swap[i1];
          state2.update_score_partial(i1);
          score = last_score;
        }
      }
      else if (roll < 1.0) {
        int x = theRandom.nextUIntMod(env.board_size);
        int y = theRandom.nextUIntMod(env.board_size);
        if (env.swap_infos[x][y].size() < 2) continue;
        int i1 = theRandom.nextUIntMod(env.swap_infos[x][y].size());
        int i2 = theRandom.nextUIntMod(env.swap_infos[x][y].size());
        if (i1 == i2) continue;

        int t1 = env.swap_infos[x][y][i1].turn;
        int t2 = env.swap_infos[x][y][i2].turn;

        state2.do_swap[t1] = !state2.do_swap[t1];
        state2.update_score_partial(t1);
        state2.do_swap[t2] = !state2.do_swap[t2];
        state2.update_score_partial(t2);
        score = state2.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) print_sa_progress(iter_count, score, last_score, best_score, temperature);
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
          state2.do_swap[t2] = !state2.do_swap[t2];
          state2.update_score_partial(t2);
          state2.do_swap[t1] = !state2.do_swap[t1];
          state2.update_score_partial(t1);
          score = last_score;
        }
      }

      if (vis_output && iter_count % 100000 == 0) {
        theTimer.stop();
        print_output(state2.get_output());
        theTimer.start();
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

  return state2.get_output();
}

void print_output(const Output& output) {
  for (int i = 0; i < 2; i++) {
    cout << output.initial_positions[i].x << " " << output.initial_positions[i].y << " ";
  }
  cout << endl;
  for (int i = 0; i < (int) output.actions.size(); i++) {
    cout << (output.actions[i].do_swap ? 1 : 0) << " " << output.actions[i].agent0_dir << " " << output.actions[i].agent1_dir << endl;
  }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  HyperParameter::load_hyper_parameter(argc, argv);

  auto input = get_input();

  auto env = init_env(input);

  auto output = solve(env);

  print_output(output);

  return 0;
}
