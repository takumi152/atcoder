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
  double base_temperature = 2.53751395632036;
  double target_temperature = 0.2786509636143085;
  double single_change_chance = 0.07121090278759733;
  double double_change_chance = 0.999715577498485;

  void load_hyper_parameter(int argc, char *argv[]) {
    if (argc > 1) base_temperature = strtod(argv[1], nullptr);
    if (argc > 2) target_temperature = strtod(argv[2], nullptr);
    if (argc > 3) single_change_chance = strtod(argv[3], nullptr);
    if (argc > 4) double_change_chance = strtod(argv[4], nullptr);
  }
}

// structs
struct Position {
  int x, y;
  Position() = default;
  Position(int x, int y): x(x), y(y) {}

  bool operator==(const Position& that) const {
    return this->x == that.x && this->y == that.y;
  }

  bool operator!=(const Position& that) const {
    return !(*this == that);
  }

  Position operator+(const Position& that) const {
    Position res = *this;
    res.x += that.x;
    res.y += that.y;
    return res;
  }
};

// enums

// constants
struct Adj4 {
  static constexpr array<int, 4> dx = {-1,  0,  1,  0};
  static constexpr array<int, 4> dy = { 0, -1,  0,  1};
  static int reverse(const int dir) {
    assert(0 <= dir && dir < 4);
    return dir ^ 2;
  }
};

// inputs
struct Input {
  static constexpr int board_size = 50;
  static constexpr int component_num = 100;

  vector<vector<int>> initial_board;

  vector<bool> connected_on_removal;
};

// outputs
struct Output {
  vector<vector<int>> final_board;
};

// internals

bool within_board(const Input &input, const Position &pos) {
  return 0 <= pos.x && pos.x < input.board_size && 0 <= pos.y && pos.y < input.board_size;
}

Input get_input() {
  Input input;

  int _board_size, _component_num;
  cin >> _board_size >> _component_num;
  input.initial_board = vector<vector<int>>(input.board_size, vector<int>(input.board_size));
  for (auto &x: input.initial_board) {
    for (auto &y: x) cin >> y;
  }

  return input;
}

void init(Input& input) {
  input.connected_on_removal = vector<bool>(1 << 9);
  for (int k = 0; k < (1 << 9); k++) {
    vector<vector<bool>> ref_board(3, vector<bool>(3));
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (i == 1 && j == 1) continue;
        if (k & (1 << (i * 3 + j))) {
          ref_board[i][j] = true;
        }
      }
    }

    // 4近傍には何か繋がっている必要がある
    bool adj4_connected = false;
    for (int d = 0; d < 4; d++) {
      if (ref_board[1 + Adj4::dx[d]][1 + Adj4::dy[d]]) {
        adj4_connected = true;
        break;
      }
    }
    if (!adj4_connected) continue;

    // 適当な点から始めて連結かどうか確認
    bool all_connected = false;
    {
      int cell_num = __builtin_popcount(k & ~(1 << 4));
      Position start_pos(-1, -1);
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if (ref_board[i][j]) {
            start_pos = Position(i, j);
            break;
          }
        }
        if (start_pos.x != -1) break;
      }
      vector<vector<bool>> visited(3, vector<bool>(3));
      int visited_count = 0;
      queue<Position> que;
      que.push(start_pos);
      while (!que.empty()) {
        auto now = que.front();
        que.pop();
        if (visited[now.x][now.y]) continue;
        visited[now.x][now.y] = true;
        visited_count++;
        for (int d = 0; d < 4; d++) {
          if (now.x + Adj4::dx[d] >= 0 && now.x + Adj4::dx[d] < 3 && now.y + Adj4::dy[d] >= 0 && now.y + Adj4::dy[d] < 3 && ref_board[now.x + Adj4::dx[d]][now.y + Adj4::dy[d]] && !visited[now.x + Adj4::dx[d]][now.y + Adj4::dy[d]]) {
            que.emplace(now.x + Adj4::dx[d], now.y + Adj4::dy[d]);
          }
        }
      }
      if (visited_count == cell_num) {
        all_connected = true;
      }
    }

    input.connected_on_removal[k] = all_connected;
  }
}

void print_output(const Output& output);

Output solve(const Input& input) {
  struct SAState {
    const Input& input;

    vector<vector<int>> board;

    vector<vector<bool>> should_be_connected; // 空白マス(0)の情報も含む
    vector<vector<int>> connection_count; // 空白マス(0)の情報も含む

    int empty_cell_score = 0;
    int wrong_connection_score = 0;

    const double empty_cell_score_weight = 1.0;
    const double wrong_connection_weight = -1e9;

    SAState(const Input& input): input(input) {
      board = input.initial_board;

      should_be_connected = vector<vector<bool>>(input.component_num + 1, vector<bool>(input.component_num + 1));
      for (int i = 0; i < input.board_size; i++) {
        for (int j = 0; j < input.board_size; j++) {
          if (within_board(input, Position(i + 1, j)) && board[i][j] != board[i + 1][j]) {
            should_be_connected[board[i][j]][board[i + 1][j]] = true;
            should_be_connected[board[i + 1][j]][board[i][j]] = true;
          }
          if (within_board(input, Position(i, j + 1)) && board[i][j] != board[i][j + 1]) {
            should_be_connected[board[i][j]][board[i][j + 1]] = true;
            should_be_connected[board[i][j + 1]][board[i][j]] = true;
          }
          for (int d = 0; d < 4; d++) {
            if (!within_board(input, Position(i + Adj4::dx[d], j + Adj4::dy[d]))) {
              should_be_connected[board[i][j]][0] = true;
              should_be_connected[0][board[i][j]] = true;
            }
          }
        }
      }

      connection_count = vector<vector<int>>(input.component_num + 1, vector<int>(input.component_num + 1));

      update_score_full();
    }

    void update_score_full() {
      empty_cell_score = 0;
      wrong_connection_score = 0;
      for (int i = 0; i <= input.component_num; i++) {
        for (int j = 0; j <= input.component_num; j++) {
          connection_count[i][j] = 0;
        }
      }

      for (int i = 0; i < input.board_size; i++) {
        for (int j = 0; j < input.board_size; j++) {
          if (board[i][j] == 0) {
            empty_cell_score++;
          }
          if (within_board(input, Position(i + 1, j)) && board[i][j] != board[i + 1][j]) {
            connection_count[board[i][j]][board[i + 1][j]]++;
            connection_count[board[i + 1][j]][board[i][j]]++;
          }
          if (within_board(input, Position(i, j + 1)) && board[i][j] != board[i][j + 1]) {
            connection_count[board[i][j]][board[i][j + 1]]++;
            connection_count[board[i][j + 1]][board[i][j]]++;
          }
          for (int d = 0; d < 4; d++) {
            if (board[i][j] != 0 && !within_board(input, Position(i + Adj4::dx[d], j + Adj4::dy[d]))) {
              connection_count[board[i][j]][0]++;
              connection_count[0][board[i][j]]++;
            }
          }
        }
      }

      for (int i = 0; i <= input.component_num; i++) {
        for (int j = 0; j <= input.component_num; j++) {
          if (should_be_connected[i][j]) {
            wrong_connection_score += max(0, 1 - connection_count[i][j]);
          }
          else {
            wrong_connection_score += connection_count[i][j];
          }
        }
      }
    }

    void update_score_partial(const Position& pos, const bool after_update) {
      if (board[pos.x][pos.y] == 0) {
        empty_cell_score += after_update ? 1 : -1;
      }
      for (int d = 0; d < 4; d++) {
        if (within_board(input, Position(pos.x + Adj4::dx[d], pos.y + Adj4::dy[d])) && board[pos.x][pos.y] != board[pos.x + Adj4::dx[d]][pos.y + Adj4::dy[d]]) {
          int c1 = board[pos.x][pos.y];
          int c2 = board[pos.x + Adj4::dx[d]][pos.y + Adj4::dy[d]];
          wrong_connection_score -= (should_be_connected[c1][c2] ? max(0, 1 - connection_count[c1][c2]) : connection_count[c1][c2]);
          connection_count[c1][c2] += after_update ? 1 : -1;
          connection_count[c2][c1] += after_update ? 1 : -1;
          wrong_connection_score += (should_be_connected[c1][c2] ? max(0, 1 - connection_count[c1][c2]) : connection_count[c1][c2]);
        }
        else if (board[pos.x][pos.y] != 0 && !within_board(input, Position(pos.x + Adj4::dx[d], pos.y + Adj4::dy[d]))) {
          int c1 = board[pos.x][pos.y];
          int c2 = 0;
          wrong_connection_score -= (should_be_connected[c1][c2] ? max(0, 1 - connection_count[c1][c2]) : connection_count[c1][c2]);
          connection_count[c1][c2] += after_update ? 1 : -1;
          connection_count[c2][c1] += after_update ? 1 : -1;
          wrong_connection_score += (should_be_connected[c1][c2] ? max(0, 1 - connection_count[c1][c2]) : connection_count[c1][c2]);
        }
      }
    }

    bool can_update_cell(const Position& pos, const int next_color) const {
      // 前の色から変更できる条件:
      // - 3x3内の隣接判定が通ること
      // - ただし、空白マス(0)の場合は盤面外も含めて判定を行う
      bool can_remove = false;
      {
        int adj_code = 0;
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            if ((within_board(input, Position(pos.x + i - 1, pos.y + j - 1)) && board[pos.x][pos.y] == board[pos.x + i - 1][pos.y + j - 1])
                || (board[pos.x][pos.y] == 0 && !within_board(input, Position(pos.x + i - 1, pos.y + j - 1))))
            {
              adj_code |= (1 << (i * 3 + j));
            }
          }
        }
        can_remove = input.connected_on_removal[adj_code];
      }
      if (!can_remove) return false;

      // 次の色に変更できる条件:
      // - 空白マス(0)の場合は同色or盤面外に隣接していること
      // - それ以外の場合は同色に隣接していること
      bool can_add = false;
      for (int d = 0; d < 4; d++) {
        if (within_board(input, Position(pos.x + Adj4::dx[d], pos.y + Adj4::dy[d])) && next_color == board[pos.x + Adj4::dx[d]][pos.y + Adj4::dy[d]]) {
          can_add = true;
          break;
        }
        else if (next_color == 0 && !within_board(input, Position(pos.x + Adj4::dx[d], pos.y + Adj4::dy[d]))) {
          can_add = true;
          break;
        }
      }
      if (!can_add) return false;

      return true;
    }

    void update_cell(const Position& pos, const int next_color) {
      board[pos.x][pos.y] = next_color;
    }

    double get_score() const {
      return empty_cell_score * empty_cell_score_weight
           + wrong_connection_score * wrong_connection_weight;
    }

    Output get_output() const {
      Output output;
      output.final_board = board;
      return output;
    }
  };

  SAState state(input);
  Output best_output = state.get_output();
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
    const double time_limit = 1.990;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < HyperParameter::single_change_chance) {
        int x = theRandom.nextUIntMod(input.board_size);
        int y = theRandom.nextUIntMod(input.board_size);
        int d = theRandom.nextUIntMod(4);

        int orig_color = state.board[x][y];
        int next_color;
        if (within_board(input, Position(x + Adj4::dx[d], y + Adj4::dy[d]))) {
          next_color = state.board[x + Adj4::dx[d]][y + Adj4::dy[d]];
        }
        else {
          next_color = 0;
        }
        if (orig_color == next_color) continue;

        bool valid = state.can_update_cell(Position(x, y), next_color);
        if (!valid) continue;

        state.update_score_partial(Position(x, y), false);
        state.update_cell(Position(x, y), next_color);
        state.update_score_partial(Position(x, y), true);

        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 50000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_output = state.get_output();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          state.update_score_partial(Position(x, y), false);
          state.update_cell(Position(x, y), orig_color);
          state.update_score_partial(Position(x, y), true);
          if (state.get_score() != last_score) {
            cerr << iter_count << " " << x << " " << y << " " << d << " " << orig_color << " " << next_color << endl;
            for (int i = 0; i < input.board_size; i++) {
              for (int j = 0; j < input.board_size; j++) {
                if (i == x && j == y) cerr << "\u001b[41m";
                cerr << setw(3) << state.board[i][j];
                if (i == x && j == y) cerr << "\u001b[0m";
                cerr << " ";
              }
              cerr << endl;
            }
            cerr << "actual:" << endl;
            cerr << "score                  = " << state.get_score() << endl;
            cerr << "empty_cell_score       = " << state.empty_cell_score << endl;
            cerr << "wrong_connection_score = " << state.wrong_connection_score << endl;
            state.update_score_full();
            cerr << "expected:" << endl;
            cerr << "score                  = " << state.get_score() << endl;
            cerr << "empty_cell_score       = " << state.empty_cell_score << endl;
            cerr << "wrong_connection_score = " << state.wrong_connection_score << endl;
            assert(false);
          }
          score = last_score;
        }
      }
      else if (roll < HyperParameter::double_change_chance) {
        int x1 = theRandom.nextUIntMod(input.board_size);
        int y1 = theRandom.nextUIntMod(input.board_size);
        int d1 = theRandom.nextUIntMod(4);
        int m = theRandom.nextUIntMod(4);
        int d2 = theRandom.nextUIntMod(4);

        int x2 = x1 + Adj4::dx[m];
        int y2 = y1 + Adj4::dy[m];
        if (!within_board(input, Position(x2, y2))) continue;

        int orig_color1 = state.board[x1][y1];
        int next_color1;
        if (within_board(input, Position(x1 + Adj4::dx[d1], y1 + Adj4::dy[d1]))) {
          next_color1 = state.board[x1 + Adj4::dx[d1]][y1 + Adj4::dy[d1]];
        }
        else {
          next_color1 = 0;
        }
        if (orig_color1 == next_color1) continue;

        bool valid1 = state.can_update_cell(Position(x1, y1), next_color1);
        if (!valid1) continue;

        state.update_score_partial(Position(x1, y1), false);
        state.update_cell(Position(x1, y1), next_color1);
        state.update_score_partial(Position(x1, y1), true);

        int orig_color2 = state.board[x2][y2];
        int next_color2;
        if (within_board(input, Position(x2 + Adj4::dx[d2], y2 + Adj4::dy[d2]))) {
          next_color2 = state.board[x2 + Adj4::dx[d2]][y2 + Adj4::dy[d2]];
        }
        else {
          next_color2 = 0;
        }
        if (orig_color2 != next_color2) {
          bool valid2 = state.can_update_cell(Position(x2, y2), next_color2);
          if (!valid2) {
            state.update_score_partial(Position(x1, y1), false);
            state.update_cell(Position(x1, y1), orig_color1);
            state.update_score_partial(Position(x1, y1), true);
            continue;
          }
          state.update_score_partial(Position(x2, y2), false);
          state.update_cell(Position(x2, y2), next_color2);
          state.update_score_partial(Position(x2, y2), true);
        }


        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 50000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_output = state.get_output();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          if (orig_color2 != next_color2) {
            state.update_score_partial(Position(x2, y2), false);
            state.update_cell(Position(x2, y2), orig_color2);
            state.update_score_partial(Position(x2, y2), true);
          }
          state.update_score_partial(Position(x1, y1), false);
          state.update_cell(Position(x1, y1), orig_color1);
          state.update_score_partial(Position(x1, y1), true);
          if (state.get_score() != last_score) {
            cerr << iter_count << " " << x1 << " " << y1 << " " << d1 << " " << orig_color1 << " " << next_color1 << endl;
            cerr << iter_count << " " << x2 << " " << y2 << " " << d2 << " " << orig_color2 << " " << next_color2 << endl;
            for (int i = 0; i < input.board_size; i++) {
              for (int j = 0; j < input.board_size; j++) {
                if (i == x1 && j == y1) cerr << "\u001b[41m";
                if (i == x2 && j == y2) cerr << "\u001b[42m";
                cerr << setw(3) << state.board[i][j];
                if (i == x1 && j == y1) cerr << "\u001b[0m";
                if (i == x2 && j == y2) cerr << "\u001b[0m";
                cerr << " ";
              }
              cerr << endl;
            }
            cerr << "actual:" << endl;
            cerr << "score                  = " << state.get_score() << endl;
            cerr << "empty_cell_score       = " << state.empty_cell_score << endl;
            cerr << "wrong_connection_score = " << state.wrong_connection_score << endl;
            state.update_score_full();
            cerr << "expected:" << endl;
            cerr << "score                  = " << state.get_score() << endl;
            cerr << "empty_cell_score       = " << state.empty_cell_score << endl;
            cerr << "wrong_connection_score = " << state.wrong_connection_score << endl;
            assert(false);
          }
          score = last_score;
        }
      }
      else if (roll < 1.000) {
        int x = theRandom.nextUIntMod(input.board_size - 2) + 1;
        int y = theRandom.nextUIntMod(input.board_size - 2) + 1;
        int d = theRandom.nextUIntMod(2);

        vector<int> orig_row(input.board_size);
        if (d == 0) {
          bool all_empty = true;
          for (int i = 0; i < input.board_size; i++) {
            if (state.board[i][y] != 0) all_empty = false;
            orig_row[i] = state.board[i][y];
          }
          if (all_empty) continue;
          for (int i = 0; i < input.board_size; i++) {
            state.board[i].erase(state.board[i].begin() + y);
            state.board[i].push_back(0);
          }
        }
        else {
          bool all_empty = true;
          orig_row = state.board[x];
          for (int i = 0; i < input.board_size; i++) {
            if (orig_row[i] != 0) all_empty = false;
          }
          if (all_empty) continue;
          state.board.erase(state.board.begin() + x);
          state.board.push_back(vector<int>(input.board_size, 0));
        }

        bool keeps_connectivity = true;
        {
          vector<bool> checked(input.component_num + 1);
          int checked_num = 0;
          vector<vector<bool>> visited(input.board_size, vector<bool>(input.board_size));
          for (int i = 0; i < input.board_size; i++) {
            for (int j = 0; j < input.board_size; j++) {
              if (!visited[i][j]) {
                int target = state.board[i][j];
                if (target != 0) {
                  checked[target] = true;
                  checked_num++;
                  queue<Position> que;
                  que.emplace(i, j);
                  while (!que.empty()) {
                    auto now = que.front();
                    que.pop();
                    if (visited[now.x][now.y]) continue;
                    visited[now.x][now.y] = true;
                    for (int d = 0; d < 4; d++) {
                      if (within_board(input, Position(now.x + Adj4::dx[d], now.y + Adj4::dy[d])) && state.board[now.x + Adj4::dx[d]][now.y + Adj4::dy[d]] == target && !visited[now.x + Adj4::dx[d]][now.y + Adj4::dy[d]]) {
                        que.emplace(now.x + Adj4::dx[d], now.y + Adj4::dy[d]);
                      }
                    }
                  }
                }
                else {
                  bool connected_to_outside = false;
                  queue<Position> que;
                  que.emplace(i, j);
                  while (!que.empty()) {
                    auto now = que.front();
                    que.pop();
                    if (visited[now.x][now.y]) continue;
                    visited[now.x][now.y] = true;
                    for (int d = 0; d < 4; d++) {
                      if (!within_board(input, Position(now.x + Adj4::dx[d], now.y + Adj4::dy[d]))) {
                        connected_to_outside = true;
                      }
                      if (within_board(input, Position(now.x + Adj4::dx[d], now.y + Adj4::dy[d])) && state.board[now.x + Adj4::dx[d]][now.y + Adj4::dy[d]] == target && !visited[now.x + Adj4::dx[d]][now.y + Adj4::dy[d]]) {
                        que.emplace(now.x + Adj4::dx[d], now.y + Adj4::dy[d]);
                      }
                    }
                  }
                  if (!connected_to_outside) keeps_connectivity = false;
                }
              }
            }
          }
          if (checked_num != input.component_num) {
            keeps_connectivity = false;
          }
          for (int i = 1; i <= input.component_num; i++) {
            if (!checked[i]) {
              keeps_connectivity = false;
              break;
            }
          }
        }
        if (!keeps_connectivity) {
          if (d == 0) {
            for (int i = 0; i < input.board_size; i++) {
              state.board[i].pop_back();
              state.board[i].insert(state.board[i].begin() + y, orig_row[i]);
            }
          }
          else {
            state.board.pop_back();
            state.board.insert(state.board.begin() + x, orig_row);
          }
          continue;
        }

        state.update_score_full();

        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 50000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_output = state.get_output();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          if (d == 0) {
            for (int i = 0; i < input.board_size; i++) {
              state.board[i].pop_back();
              state.board[i].insert(state.board[i].begin() + y, orig_row[i]);
            }
          }
          else {
            state.board.pop_back();
            state.board.insert(state.board.begin() + x, orig_row);
          }
          state.update_score_full();
          score = last_score;
        }
      }

      // 中心からよくずれるので、たまにできるだけ中心に動かす
      if (iter_count % 20000 == 0) {
        int x_min = input.board_size;
        int x_max = 0;
        int y_min = input.board_size;
        int y_max = 0;
        for (int i = 0; i < input.board_size; i++) {
          for (int j = 0; j < input.board_size; j++) {
            if (state.board[i][j] != 0) {
              x_min = min(x_min, i);
              x_max = max(x_max, i);
              y_min = min(y_min, j);
              y_max = max(y_max, j);
            }
          }
        }

        while (x_min < input.board_size - x_max && x_max < input.board_size - 1) {
          state.board.pop_back();
          state.board.insert(state.board.begin(), vector<int>(input.board_size));
          x_min++;
          x_max++;
        }
        while (y_min < input.board_size - y_max && y_max < input.board_size - 1) {
          for (int i = 0; i < input.board_size; i++) {
            state.board[i].pop_back();
            state.board[i].insert(state.board[i].begin(), 0);
          }
          y_min++;
          y_max++;
        }

        // #ifdef LOCAL
        // print_output(state.get_output());
        // #endif
      }

      // temperature *= 1.0 - decay_rate;
      // if (iter_count == 100000) break;
      temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      iter_count++;
    }

    cerr << "iter_count             = " << iter_count << endl;
    cerr << "score                  = " << score << endl;
    cerr << "empty_cell_score       = " << state.empty_cell_score << endl;
    cerr << "wrong_connection_score = " << state.wrong_connection_score << endl;
    cerr << "best_score             = " << best_score << endl;
    cerr << "temperature            = " << temperature << endl;
  }

  return best_output;
}

void print_output(const Output& output) {
  for (auto &x: output.final_board) {
    for (auto &y: x) cout << y << " ";
    cout << endl;
  }
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
