// ヘッダーは変えない
// namespace Takumi内を変える
// seedを入力から受け取らず、INPUTSEEDを使用する
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
#include <cstring>
#include <chrono>
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
#include <map>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
namespace atcoder {}


int INPUTSEED = 0;

namespace Takumi{
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
  input.case_type = INPUTSEED;
  cin >> input.board_size;
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
  const bool vis_output = false;

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

int Main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  HyperParameter::load_hyper_parameter(argc, argv);

  auto input = get_input();

  auto env = init_env(input);

  auto output = solve(env);

  print_output(output);

  return 0;
}


}

namespace Jiro{

using namespace std;
using namespace atcoder;
#define fast_io                     \
  ios_base::sync_with_stdio(false); \
  cin.tie(0);                       \
  cout.tie(0);
#define ll long long int
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, n) for (int i = 1; i <= (int)(n); i++)
#define REP(i, n) for (int i = n - 1; i >= 0; i--)
#define REPS(i, n) for (int i = n; i > 0; i--)
#define MOD (long long int)(1e9 + 7)
#define INF (int)(1e9)
#define LINF (long long int)(1e18)
#define chmax(a, b) a = (((a) < (b)) ? (b) : (a))
#define chmin(a, b) a = (((a) > (b)) ? (b) : (a))
#define ALL(v) v.begin(), v.end()
typedef pair<int, int> Pii;
typedef pair<ll, ll> Pll;
constexpr double PI = acos(-1);

#ifdef NDEBUG
#define CHECK(v1, op, v2)
#else
#define CHECK(v1, op, v2)                            \
  if (!((v1)op(v2))) {                               \
    cerr << "ERROR:" << (v1) << " " << (v2) << endl; \
    assert((v1)op(v2));                              \
  }
#endif

long double nCr(const int n, const int r) {
  long double ret = 1;
  rep(t, r) {
    ret *= (n - t);
    ret /= (r - t);
  }
  return ret;
}

template <typename T>
string to_string(const vector<T>& vec) {
  string ret = "";
  rep(i, vec.size()) {
    ret += vec[i].to_string();
    if (i + 1 != vec.size()) {
      ret += ",";
    }
  }
  return ret;
}

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& vec) {
  os << to_string(vec);
  return os;
}

uint32_t xorshift() {
  static uint32_t x = 12345789;
  static uint32_t y = 362436069;
  static uint32_t z = 521288629;
  static uint32_t w = 88675123;
  uint32_t t;
  t = x ^ (x << 11);
  x = y;
  y = z;
  z = w;
  w ^= t ^ (t >> 8) ^ (w >> 19);

  return w;
}

int rand(const uint32_t l, const uint32_t r) {
  return xorshift() % (r - l) + l;
}

uint32_t rand_other_than(const uint32_t l, const uint32_t r,
                         const uint32_t other) {
  const uint32_t num = rand(l, r - 1);
  return num + (num >= other);
}

template <typename T>
const T& rand_vec(const vector<T>& vec) {
  assert(vec.size() > 0);
  return vec[rand(0, vec.size())];
}

template <typename T>
void shuffle(vector<T>& vec) {
  rep(l, (int)vec.size() - 1) {
    const int idx = rand(l, vec.size());
    swap(vec[idx], vec[l]);
  }
}


class Timer {
  chrono::system_clock::time_point _start, _end;
  ll _sum = 0, _count = 0;

 public:
  void start() { _start = chrono::system_clock::now(); }

  void stop() { _end = chrono::system_clock::now(); }

  void add() {
    const chrono::system_clock::time_point now = chrono::system_clock::now();
    _sum += static_cast<double>(
        chrono::duration_cast<chrono::nanoseconds>(now - _start).count());
    _count++;
  }

  ll sum() const { return _sum / 1000; }

  int count() const { return _count; }

  string average() const {
    if (_count == 0) {
      return "NaN";
    }
    return std::to_string(_sum / 1000 / _count);
  }

  void reset() {
    _start = chrono::system_clock::now();
    _sum = 0;
    _count = 0;
  }

  inline int ms() const {
    const chrono::system_clock::time_point now = chrono::system_clock::now();
    return static_cast<double>(
        chrono::duration_cast<chrono::microseconds>(now - _start).count() /
        1000);
  }

  inline int ns() const {
    const chrono::system_clock::time_point now = chrono::system_clock::now();
    return static_cast<double>(
        chrono::duration_cast<chrono::microseconds>(now - _start).count());
  }
};

#ifdef LOCAL
struct Timers : unordered_map<string, Timer> {
  friend ostream& operator<<(ostream& os, const Timers& timers) {
    for (const auto& pa : timers) {
      os << pa.first << " time: " << pa.second.sum() / 1000
         << " count: " << pa.second.count() << endl;
    }
    return os;
  }
};
#else
struct Timers {
  struct Dummy {
    void start() const {}
    void add() const {}
  };
  Dummy dummy;
  const Dummy& operator[](const std::string& str) { return dummy; }
  friend ostream& operator<<(ostream& os, const Timers& timers) { return os; }
};
#endif

Timers global_timers;






enum Dir {
  // y-1, y+1, x-1, x+1
  kU,
  kD,
  kL,
  kR,
  kNone,
};

vector<Dir> kDirs5 = {kU,kD,kL,kR,kNone};

string Dir2Str(const Dir dir) {
  if (dir == kU) {
    return "U";
  } else if (dir == kD) {
    return "D";
  } else if (dir == kL) {
    return "L";
  } else if (dir == kR) {
    return "R";
  } else {
    return ".";
  }
}

Dir RightDir(const Dir dir) {
  constexpr Dir kRightDirs[4] = {kR, kL, kU, kD};
  return kRightDirs[dir];
}

Dir InvDir(const Dir dir) {
  constexpr Dir kInvDirs[4] = {kD, kU, kR, kL};
  return kInvDirs[dir];
}

struct Pos {
  int idx_;
  Pos() {}
  explicit Pos(const int _idx) : idx_(_idx) {}
  Pos(int _x, int _y) : idx_(_y * N + _x) { assert(N > 0); }
  int X() const { return pos_2_x[*this]; }
  int Y() const { return pos_2_y[*this]; }
  int Idx() const { return idx_; }
  operator int() const { return Idx(); }
  operator size_t() const { return Idx(); }
  const vector<Pos>& Adj() const { return adj_poses[*this]; }
  const vector<Dir>& AdjDirs() const { return adj_dirs[*this]; }
  int Manhattan(const Pos& other) const {
    return abs(X() - other.X()) + abs(Y() - other.Y());
  }
  int Euclid2(const Pos& other) const {
    const int dx = X() - other.X();
    const int dy = Y() - other.Y();
    return dx * dx + dy * dy;
  }
  bool Move(const Dir dir) {
    if (move_to[dir][*this].IsDummy()) {
      return false;
    } else {
      *this = move_to[dir][*this];
      return true;
    }
  }
  int Dist(const Pos other) const { return dists[Idx() * N2 + other.Idx()]; }
  template <typename ID>
  bool IsConnectedAfterRemoving(const vector<ID>& ids) const {
    // 自身を削除してもids[p]の連結性が維持されるか3x3を見て判定
    bitset<8> bs = 0;
    int adj_count = 0;
    int i = -1;
    const auto& p = *this;
    const int base_id = ids[p];
    for (const auto adj : p.Adj()) {
      i++;
      if (ids[adj] == base_id) {
        adj_count++;
        bs[i] = true;
      }
    }
    if (adj_count <= 1) return true;

    i = 3;
    for (const auto adjadj : adjadjs_check_remove[p]) {
      i++;
      bs[i] = ids[adjadj] == base_id;
    }
    return can_remove_bses[p][bs.to_ulong()];
  }
  bool operator<(const Pos& other) const { return this->Idx() < other.Idx(); }
  bool operator==(const Pos& other) const { return this->Idx() == other.Idx(); }
  bool operator!=(const Pos& other) const { return this->Idx() != other.Idx(); }
  friend ostream& operator<<(ostream& os, const Pos& pos) {
    os << pos.X() << " " << pos.Y();
    return os;
  }
  bool IsDummy() const { return this->idx_ < 0; }
  static Pos Dummy() {
    Pos p;
    p.idx_ = -1;
    return p;
  }

  static Pos TryCreate(const int x, const int y) {
    if (y < 0 || y >= N || x < 0 || x >= N) {
      return Pos::Dummy();
    } else {
      return Pos(x, y);
    }
  }

  static void StaticInit(const int n, const vector<string>& V,
                         const vector<string>& H) {
    N = n;
    N2 = N * N;
    N4 = N2 * N2;
    adj_poses = vector<vector<Pos>>(N2);
    adj_dirs = vector<vector<Dir>>(N2);
    move_to = vector<vector<Pos>>(4, vector<Pos>(N2, Pos::Dummy()));
    pos_2_y.resize(N2);
    pos_2_x.resize(N2);

    rep(y, N) {
      rep(x, N) {
        const Pos p{x, y};
        all_poses.push_back(p);
        pos_2_y[p] = y;
        pos_2_x[p] = x;
        for (int dy = -1; dy <= 1; ++dy) {
          for (int dx = -1; dx <= 1; ++dx) {
            if (abs(dy) + abs(dx) != 1) continue;
            const int adj_y = y + dy;
            const int adj_x = x + dx;
            if (adj_y < 0 || adj_y >= N) continue;
            if (adj_x < 0 || adj_x >= N) continue;
            if (dx == 1) {
              if (V[y][x] == '1') continue;
            } else if (dx == -1) {
              if (V[y][x - 1] == '1') continue;
            } else if (dy == 1) {
              if (H[y][x] == '1') continue;
            } else if (dy == -1) {
              if (H[y - 1][x] == '1') continue;
            } else {
              assert(false);
            }
            adj_poses[p].emplace_back(adj_x, adj_y);

            Dir dir;
            if (dy == -1) {
              dir = kU;
            } else if (dy == 1) {
              dir = kD;
            } else if (dx == -1) {
              dir = kL;
            } else if (dx == 1) {
              dir = kR;
            } else {
              assert(false);
            }

            move_to[dir][p] = {adj_x, adj_y};
            adj_dirs[p].emplace_back(dir);
          }
        }
      }
    }
    all_poses.shrink_to_fit();

    if (N * N * 256 <= 1e7) {
      can_remove_bses.resize(N2);
      adjadjs_check_remove.resize(N2);
      rep(idx, N * N) {
        const Pos p(idx);
        set<Pos> S;
        for (const auto adj : p.Adj()) {
          for (const auto adjadj : adj.Adj()) {
            if (adjadj == p) continue;
            if (S.count(adjadj) > 0) {
              adjadjs_check_remove[p].push_back(adjadj);
            } else {
              S.insert(adjadj);
            }
          }
        }
        assert(adjadjs_check_remove[p].size() <= 4);
        rep(bit, 256) {
          // adj0, adj1, ..., adjadj0, adjdadj1, ... を詰めずに並べる
          bitset<8> bs(bit);
          set<Pos> adjs, adjadjs;
          rep(j, p.Adj().size()) {
            if (bs[j]) {
              adjs.insert(p.Adj()[j]);
            }
          }
          rep(j, adjadjs_check_remove[p].size()) {
            const auto adjadj = adjadjs_check_remove[p][j];
            if (bs[j + 4]) {
              int count = 0;
              for (const auto adjadjadj : adjadj.Adj()) {
                count += adjs.count(adjadjadj);
              }
              if (count >= 2) {
                adjadjs.insert(adjadj);
              }
            }
          }

          can_remove_bses[p][bs.to_ulong()] =
              ((int)adjadjs.size() >= (int)adjs.size() - 1);
        }
      }
    }

    // dist計算
    dists.resize(N4, -1);
    for (const auto base : all_poses) {
      queue<Pos> q;
      vector<bool> checked(N2);
      checked[base] = true;
      q.push(base);
      int nextDist = 0;
      while(q.size() > 0){
        queue<Pos> nextQ;
        while(q.size() > 0){
          const auto p = q.front();
          q.pop();
          dists[base.Idx() * N2 + p.Idx()] = nextDist;
          for(const auto adj : p.Adj()){
            if(checked[adj]) continue;
            checked[adj] = true;
            nextQ.push(adj);
          }
        }
        q = std::move(nextQ);
        nextDist++;
      }
    }
  }
  static int N, N2, N4;
  static vector<Pos> all_poses;
  static vector<vector<Pos>> adj_poses;
  static vector<vector<Dir>> adj_dirs;
  static vector<vector<Pos>> move_to;
  static vector<int> pos_2_x, pos_2_y;
  // 2^8のbit列を受け取って削除可能かを返す
  // adj_poses[0], ..., adj_poses[3], adjadjs_check_remove[0], ...,
  // adjadjs_check_remove[3]
  // の順に詰めずに並べること 左が上位桁。
  static vector<bitset<256>> can_remove_bses;
  static vector<vector<Pos>> adjadjs_check_remove;
  static vector<int> dists;
};

vector<Pos> Pos::all_poses;
vector<int> Pos::dists;
vector<vector<Pos>> Pos::adj_poses;
vector<vector<Dir>> Pos::adj_dirs;
vector<vector<Pos>> Pos::move_to;
vector<int> Pos::pos_2_x, Pos::pos_2_y;
int Pos::N, Pos::N2, Pos::N4;
vector<bitset<256>> Pos::can_remove_bses;
vector<vector<Pos>> Pos::adjadjs_check_remove;


/* start */

Timer global_timer;
int param_ttt, N;
vector<string> V, H;
vector<int> A;
int Q;
ll initialSum = 0;

struct Command {
  bool doSwap;
  Dir d0;
  Dir d1;
};

Dir FromTo2Dir(const Pos from, const Pos to){
  if(from.X() != to.X()){
    if(from.X() > to.X()){
      return kL;
    }else{
      return kR;
    }

  }else if(from.Y() != to.Y()){
    if(from.Y() > to.Y()){
      return kU;
    }else{
      return kD;
    }
  }else{
    return kNone;
  }
}


ll BoardSquareSum(const vector<int>& board) {
  ll sum = 0;
  for (const auto p : Pos::all_poses) {
    for (const auto dir : {kR, kD}) {
      auto ppp = p;
      if (!ppp.Move(dir)) continue;
      const ll diff = (board[p] - board[ppp]);
      sum += diff * diff;
    }
  }
  return sum;
}

ll BoardScore(const vector<int>& board) {
  return max(1ll, (ll)std::round(
                      1e6 * log2((double)initialSum / BoardSquareSum(board))));
}

ll SquareDiff(const ll a, const ll b){
  return (a-b) * (a-b);
}

struct Board {
  vector<int> board;
  Pos p0, p1;
  // 外側から操作
  ll cost = LINF;
  Board(){}
  Board(const Pos p0_, const Pos p1_) : board(A), p0(p0_), p1(p1_) {}
  bool IsDummy()const{
    return cost == LINF;
  }
  void Swap() { swap(board[p0], board[p1]); }
  void Move(const Dir dir, const int t) {
    if (dir == kNone) return;
    if (t == 0) {
      const bool success = p0.Move(dir);
      assert(success);
    } else {
      const bool success = p1.Move(dir);
      assert(success);
    }
  }
  double EvalIfSwap(const vector<ll>& finalA, Pos ppp0, Pos ppp1) const{
    // 大きいほうが嬉しい
    double currentCost = SquareDiff(finalA[ppp0], Get(ppp0)) +
                          SquareDiff(finalA[ppp1], Get(ppp1));
    double afterCost = SquareDiff(finalA[ppp0], Get(ppp1)) +
                        SquareDiff(finalA[ppp1], Get(ppp0));
    double maxDist = max(ppp0.Dist(p0), ppp1.Dist(p1));
    const double eval = (currentCost - afterCost) / (maxDist + 1e-3);
    return eval;
  }
  ll EvalIfSwapNoDist(const vector<ll>& finalA, Pos ppp0, Pos ppp1) const{
    // 大きいほうが嬉しい
    double currentCost = SquareDiff(finalA[ppp0], Get(ppp0)) +
                          SquareDiff(finalA[ppp1], Get(ppp1));
    double afterCost = SquareDiff(finalA[ppp0], Get(ppp1)) +
                        SquareDiff(finalA[ppp1], Get(ppp0));
    const ll eval = (currentCost - afterCost);
    return eval;
  }
  ll Score() const { return BoardScore(board); }
  ll Get(const Pos p) const { return board[p]; }
};

void Print(const Pos p0, const Pos p1, const vector<Command>& commands) {
  cout << p0.Y() << " " << p0.X() << " " << p1.Y() << " " << p1.X() << endl;
  for (const auto [doSwap, d0, d1] : commands) {
    cout << (doSwap ? 1 : 0) << " " << Dir2Str(d0) << " " << Dir2Str(d1)
         << endl;
  }
}


struct Output {
  static void StaticInit(istream& is) {
    global_timer.start();
    cin >> N;
    rep(i, N) {
      string s;
      cin >> s;
      V.push_back(s);
    }
    rep(i, N - 1) {
      string s;
      cin >> s;
      H.push_back(s);
    }
    rep(i, N) {
      rep(j, N) {
        int a;
        cin >> a;
        a--;
        A.push_back(a);
      }
    }

    Pos::StaticInit(N, V, H);
    Q = 4 * N * N;
    initialSum = BoardSquareSum(A);
  }
  friend ostream& operator<<(ostream& os, const Output& output) { return os; }
};



/* start */

vector<double> PARAMS = {1.0, 0.1};




/* start */

class Solver {
 public:
  Solver() {}
  void Solve() {
    vector<ll> finalA(N * N);
    // 最終盤面をBFSで決め打つ
    {
      vector<bool> checked(N * N);
      queue<Pos> q;
      q.push(Pos(0, 0));
      checked[Pos(0, 0)] = true;
      int nextNumber = 0;
      while (q.size() > 0) {
        const Pos p = q.front();
        finalA[p] = nextNumber++;
        q.pop();
        for (const auto ppp : p.Adj()) {
          if (checked[ppp]) continue;
          checked[ppp] = true;
          q.push(ppp);
        }
      }
      assert(nextNumber == N * N);
    }

    // swap後の完成盤面との二乗差の減少量 / 移動コスト
    // を最大化する点の組を貪欲に選ぶ
    Pos startP0 = Pos(3, 3);
    Pos startP1 = Pos(N - 3, N - 3);
    Board board(startP0, startP1);
    vector<Command> commands;
    int q = 0;
    while(q < Q && global_timer.ms() <= 1950){
      // 目的地を選ぶ
      Pos bestP0, bestP1;
      double maxEval = -LINF;
      // 完成盤面との差でソート
      // 大きい順
      auto sortedPoses = Pos::all_poses;
      sort(sortedPoses.begin(), sortedPoses.end(), [&](const Pos l, const Pos r){
        return SquareDiff(finalA[l], board.Get(l)) > SquareDiff(finalA[r], board.Get(r));
      });
      constexpr size_t kMaxSize = 70;
      if(sortedPoses.size() > kMaxSize){
        sortedPoses.resize(kMaxSize);
      }
      for (const auto p0 : sortedPoses) {
        for (const auto p1 : sortedPoses) {
          if(p0 == board.p0 && p1 == board.p1) continue;
          const auto eval = board.EvalIfSwap(finalA, p0, p1);
          if(eval> maxEval){
            maxEval= eval;
            bestP0 = p0;
            bestP1 = p1;
          }
        }
      }
      // cerr <<board.p0<<" "<<board.p1<<" "<< bestP0<<" "<<bestP1<<endl;

      vector<Pos> id2poses[2];
      rep(t,2){
        map<Pos, ll> dp;
        map<Pos, Pos> prevs;
        const auto boardStartP = (t == 0 ? board.p0 : board.p1);
        const auto bestGoalP = (t==0 ? bestP0 : bestP1);
        dp[boardStartP] = 1;
        queue<Pos> qqq;
        qqq.push(boardStartP);
        while(qqq.size() > 0){
          queue<Pos> nextQQQ;
          while(qqq.size() > 0){
            const auto p = qqq.front();
            qqq.pop();
            for(const auto adj : p.Adj()){
              if(bestGoalP.Dist(adj) > bestGoalP.Dist(p)) continue;
              if(dp[adj] == 0){
                nextQQQ.push(adj);
              }
              ll diff = SquareDiff(finalA[adj], board.Get(adj));
              ll newScore = diff + dp[p];
              if(newScore > dp[adj]){
                dp[adj] = newScore;
                prevs[adj] = p;
              }
            }
          }
          qqq = std::move(nextQQQ);
        }
        // 復元
        auto& poses = id2poses[t];
        poses.push_back(bestGoalP);
        int debug = 0;
        while(true){
          if(poses.back() == boardStartP) break;
          poses.push_back(prevs[poses.back()]);
        }
      }

      reverse(ALL(id2poses[0]));
      reverse(ALL(id2poses[1]));


      // アラインをDP
      // dp[idx0][idx1] = 大きいほうの次がidx0で、小さいほうの次がidx1
      // 最短手数であることは守る
      bool swapped = false;
      if(id2poses[0].size() < id2poses[1].size()){
        swap(id2poses[0], id2poses[1]);
        swapped = true;
      }
      vector<vector<ll>> dp(id2poses[0].size() + 1, vector<ll>(id2poses[1].size() + 1, -LINF));
      vector<vector<pair<int,int>>> prevs(id2poses[0].size() + 1, vector<pair<int,int>>(id2poses[1].size() + 1));
      dp[0][0] = 0;
      rep(i, id2poses[0].size()){
        rep(j,id2poses[1].size()){
          const auto baseValue = dp[i][j];
          // jを進めない
          {
            if(dp[i+1][j] < dp[i][j]){
              dp[i+1][j] = dp[i][j];
              prevs[i+1][j] = {i,j};
            }
          }
          // jを進める
          {
            const auto nextValue = baseValue + board.EvalIfSwapNoDist(finalA, id2poses[0][i], id2poses[1][j]);
            if(dp[i+1][j+1] < nextValue){
              dp[i+1][j+1] = nextValue;
              prevs[i+1][j+1] = {i,j};
            }
          }
        }
      }
      // 復元
      vector<pair<int,int>> indexes;
      pair<int,int> currentIdx = {id2poses[0].size(), id2poses[1].size()};
      while(currentIdx != pair<int,int>{0,0}){
        indexes.push_back(currentIdx);
        currentIdx = prevs[currentIdx.first][currentIdx.second];
      }
      indexes.push_back(currentIdx);
      reverse(ALL(indexes));

      if(swapped){
        swap(id2poses[0], id2poses[1]);
        for(auto& idx : indexes){
          swap(idx.first, idx.second);
        }
      }



      // 雑に移動
      reps(i, indexes.size()-1){
        if(q >= Q) break;
        const auto index = indexes[i];
        Dir dir0 = kNone, dir1 = kNone;
        if(board.p0 != bestP0){
          assert((int)id2poses[0].size() > 0);
          dir0 = FromTo2Dir(board.p0, id2poses[0][index.first]);
        }
        if(board.p1 != bestP1){
          assert((int)id2poses[1].size() > 0);
          dir1 = FromTo2Dir(board.p1, id2poses[1][index.second]);
        }

        q++;
        //swapすべきか
        bool shouldSwap = false;
        if(board.EvalIfSwap(finalA, board.p0, board.p1) > 0){
          shouldSwap = true;
          board.Swap();
        }
        board.Move(dir0, 0);
        board.Move(dir1, 1);
        commands.push_back(Command{shouldSwap, dir0, dir1});
      }
    }
    cerr << "score: " << BoardScore(board.board)<<endl;
    Print(startP0, startP1, commands);
    return;
  }

 private:
};

int Main(int argc, char* argv[]) {
  fast_io;

  if (argc >= 2) {
    int idx = 0;
    for (int i = 1; i < argc; ++i) {
      PARAMS[idx++] = std::stod(argv[i]);
    }
  }

  Timer timer;
  timer.start();
  Output::StaticInit(cin);
  Solver solver;
  solver.Solve();
  return 0;
}
}

int main(int argc, char *argv[]) {
  std::cin >> INPUTSEED;
  std::vector<int> jiroWin = {
    1,2,3,4,5,6,8,9,10,11,12,16
  };
  const auto itr = std::find(ALL(jiroWin), INPUTSEED);
  if(itr == jiroWin.end()){
    return Takumi::Main(argc, argv);
  }else{
    return Jiro::Main(argc, argv);
  }
}