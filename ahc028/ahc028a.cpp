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
  void load_hyper_parameter(int argc, char *argv[]) {
    // do nothing
  }
}

// enums

// structs
struct Position {
  int x;
  int y;
  Position(): x(0), y(0) {};
  Position(int x, int y): x(x), y(y) {};

  bool operator==(const Position& that) const {
    return this->x == that.x && this->y == that.y;
  }

  bool operator!=(const Position& that) const {
    return !(this->x == that.x && this->y == that.y);
  }

  Position operator+(const Position& that) const {
    Position res;
    res.x = this->x + that.x;
    res.y = this->y + that.y;
    return res;
  }

  Position operator-(const Position& that) const {
    Position res;
    res.x = this->x - that.x;
    res.y = this->y - that.y;
    return res;
  }

  Position operator*(const int& that) const {
    Position res;
    res.x = this->x * that;
    res.y = this->y * that;
    return res;
  }

  Position operator*(const double& that) const {
    Position res;
    res.x = (int) ((double) (this->x) * that);
    res.y = (int) ((double) (this->y) * that);
    return res;
  }

  Position& operator+=(const Position& that) {
    this->x += that.x;
    this->y += that.y;
    return *this;
  }

  Position& operator-=(const Position& that) {
    this->x -= that.x;
    this->y -= that.y;
    return *this;
  }

  Position& operator*=(const Position& that) {
    this->x *= that.x;
    this->y *= that.y;
    return *this;
  }

  int length() const
  {
    return abs(this->x) + abs(this->y);
  }

  static int distance_between(const Position& p1, const Position& p2) {
    const Position delta = p2 - p1;
    return delta.length();
  }

  int distance_to(const Position& target) const {
    return distance_between((*this), target);
  }

  size_t hash() const {
    return (size_t) (this->x << 16) + (size_t) (this->y);
  }
};

bool operator<(const Position& lhs, const Position& rhs) {
  return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

namespace std {
  template <>
  struct hash<Position> {
    size_t operator()(const Position& pos) const {
      return pos.hash();
    }
  };

  template <typename T>
  void hash_combine(size_t& seed, const T& v) {
    seed ^= hash<T>{}(v) + 0x9e3779b9U + (seed << 6) + (seed >> 2);
  }

  template <>
  struct hash<pair<int, Position>> {
    size_t operator()(const pair<int, Position>& t) const {
      size_t hash = 0;
      hash_combine(hash, t.first);
      hash_combine(hash, t.second);
      return hash;
    }
  };

  template <>
  struct hash<tuple<int, int, Position, Position>> {
    size_t operator()(const tuple<int, int, Position, Position>& t) const {
      size_t hash = 0;
      hash_combine(hash, get<0>(t));
      hash_combine(hash, get<1>(t));
      hash_combine(hash, get<2>(t));
      hash_combine(hash, get<3>(t));
      return hash;
    }
  };
}

struct OptimalPathInfo {
  int score;
  vector<Position> path;
};

// constants
namespace Consts {
  constexpr int board_size = 15;
  constexpr int word_num = 200;
  constexpr int word_length = 5;
};

// inputs
struct Input {
  Position initial_pos;
  vector<vector<char>> board;
  vector<vector<char>> words;

  vector<vector<Position>> char_appear_pos;
  vector<vector<int>> word_begin_pos_of_next;
  unordered_map<tuple<int, int, Position, Position>, OptimalPathInfo> optimal_path_info_dict; // word_id, begin_word_index, begin_pos, end_pos
  vector<int> optimal_path_score;

  Input(): initial_pos(), board(), words(), char_appear_pos(), word_begin_pos_of_next(), optimal_path_info_dict() {};
};

// outputs
struct Output {
  vector<Position> path;
};

// internals
int hash_idx(const tuple<int, int, Position, Position>& t) {
  return  get<0>(t) * Consts::word_length * Consts::board_size * Consts::board_size * Consts::board_size * Consts::board_size
        + get<1>(t) * Consts::board_size * Consts::board_size * Consts::board_size * Consts::board_size
        + get<2>(t).x * Consts::board_size * Consts::board_size * Consts::board_size
        + get<2>(t).y * Consts::board_size * Consts::board_size
        + get<3>(t).x * Consts::board_size
        + get<3>(t).y;
}

struct SAState {
  const Input& input;
  vector<int> word_order;
  vector<Position> word_begin_pos;
  vector<Position> word_end_pos;

  int score;

  SAState(const Input& input): input(input) {
    word_order = vector<int>(Consts::word_num);
    iota(word_order.begin(), word_order.end(), 0);
    word_begin_pos = vector<Position>(Consts::word_num);
    word_end_pos = vector<Position>(Consts::word_num);
    for (int i = 0; i < Consts::word_num; i++) {
      word_begin_pos[i] = input.char_appear_pos[input.words[i][0]][0];
      word_end_pos[i] = input.char_appear_pos[input.words[i][Consts::word_length - 1]][0];
    }

    update_score_full();
  }

  void update_score_full() {
    score = 0;
    Position prev_end_position = input.initial_pos;
    for (int i = 0; i < Consts::word_num; i++) {
      if (i == 0 || input.word_begin_pos_of_next[word_order[i - 1]][word_order[i]] == 0) {
        score += prev_end_position.distance_to(word_begin_pos[word_order[i]]) + 1;
        prev_end_position = word_begin_pos[word_order[i]];
      }
      if (i == 0) {
        score += input.optimal_path_score[hash_idx(make_tuple(word_order[i], 0, prev_end_position, word_end_pos[word_order[i]]))];
      }
      else {
        score += input.optimal_path_score[hash_idx(make_tuple(word_order[i], max(0, input.word_begin_pos_of_next[word_order[i - 1]][word_order[i]] - 1), prev_end_position, word_end_pos[word_order[i]]))];
      }
      prev_end_position = word_end_pos[word_order[i]];
    }
  }

  int get_score() {
    return score;
  }

  Output get_output() {
    Output output;
    Position prev_end_position = input.initial_pos;
    for (int i = 0; i < Consts::word_num; i++) {
      if (i == 0 || input.word_begin_pos_of_next[word_order[i - 1]][word_order[i]] == 0) {
        output.path.push_back(word_begin_pos[word_order[i]]);
        prev_end_position = word_begin_pos[word_order[i]];
      }
      if (i == 0) {
        const auto& optimal_path = input.optimal_path_info_dict.at(make_tuple(word_order[i], 0, prev_end_position, word_end_pos[word_order[i]])).path;
        output.path.insert(output.path.end(), optimal_path.begin(), optimal_path.end());
      }
      else {
        const auto& optimal_path = input.optimal_path_info_dict.at(make_tuple(word_order[i], max(0, input.word_begin_pos_of_next[word_order[i - 1]][word_order[i]] - 1), prev_end_position, word_end_pos[word_order[i]])).path;
        output.path.insert(output.path.end(), optimal_path.begin(), optimal_path.end());
      }
      prev_end_position = word_end_pos[word_order[i]];
    }
    return output;
  }
};

Input get_input() {
  Input input;
  int _n, _m;
  cin >> _n >> _m;
  cin >> input.initial_pos.x >> input.initial_pos.y;
  input.board = vector<vector<char>>(Consts::board_size, vector<char>(Consts::board_size));
  for (int i = 0; i < Consts::board_size; i++) {
    for (int j = 0; j < Consts::board_size; j++) {
      cin >> input.board[i][j];
      input.board[i][j] -= 'A';
    }
  }
  input.words = vector<vector<char>>(Consts::word_num, vector<char>(Consts::word_length));
  for (int i = 0; i < Consts::word_num; i++) {
    for (int j = 0; j < Consts::word_length; j++) {
      cin >> input.words[i][j];
      input.words[i][j] -= 'A';
    }
  }

  return input;
}

void init(Input& input) {
  input.char_appear_pos = vector<vector<Position>>(26);
  for (int i = 0; i < Consts::board_size; i++) {
    for (int j = 0; j < Consts::board_size; j++) {
      input.char_appear_pos[input.board[i][j]].push_back(Position(i, j));
    }
  }

  input.word_begin_pos_of_next = vector<vector<int>>(Consts::word_num, vector<int>(Consts::word_num));
  for (int i = 0; i < Consts::word_num; i++) {
    for (int j = 0; j < Consts::word_num; j++) {
      if (i == j) {
        input.word_begin_pos_of_next[i][j] = -1;
        continue;
      }
      for (int k = 0; k <= Consts::word_length; k++) {
        bool match = true;
        for (int l = 0; k + l < Consts::word_length; l++) {
          if (input.words[i][k + l] != input.words[j][l]) {
            match = false;
            break;
          }
        }
        if (match) {
          input.word_begin_pos_of_next[i][j] = Consts::word_length - k;
          break;
        }
      }
    }
  }

  input.optimal_path_info_dict = unordered_map<tuple<int, int, Position, Position>, OptimalPathInfo>();
  for (int i = 0; i < Consts::word_num; i++) {
    for (int j = 0; j < Consts::word_length - 1; j++) {
      for (const auto& begin_pos: input.char_appear_pos[input.words[i][j]]) {
        unordered_map<pair<int, Position>, OptimalPathInfo> best_path_dict;
        best_path_dict[make_pair(j, begin_pos)] = {0, vector<Position>({})};
        priority_queue<tuple<int, int, Position, Position>, vector<tuple<int, int, Position, Position>>, greater<tuple<int, int, Position, Position>>> que; // score, word_index, prev_pos, next_pos
        for (const auto &next_pos : input.char_appear_pos[input.words[i][j + 1]]) {
          int score = begin_pos.distance_to(next_pos) + 1;
          que.emplace(score, j + 1, begin_pos, next_pos);
        }
        while (!que.empty()) {
          const auto [score, word_index, prev_pos, next_pos] = que.top();
          que.pop();
          const auto prev_key = make_pair(word_index - 1, prev_pos);
          const auto next_key = make_pair(word_index, next_pos);
          if (best_path_dict.find(next_key) != best_path_dict.end()) continue;
          auto best_path = best_path_dict[prev_key].path;
          best_path.push_back(next_pos);
          best_path_dict[next_key] = {score, best_path};
          if (word_index == Consts::word_length - 1) continue;
          for (const auto &next_next_pos : input.char_appear_pos[input.words[i][word_index + 1]]) {
            int next_score = score + next_pos.distance_to(next_next_pos) + 1;
            que.emplace(next_score, word_index + 1, next_pos, next_next_pos);
          }
        }
        for (const auto& [key, value] : best_path_dict) {
          const auto& [word_index, end_pos] = key;
          if (word_index != Consts::word_length - 1) continue;
          const auto& optimal_path = value;
          input.optimal_path_info_dict[make_tuple(i, j, begin_pos, end_pos)] = optimal_path;
        }
      }
    }
  }

  input.optimal_path_score = vector<int>(Consts::word_num * Consts::word_length * Consts::board_size * Consts::board_size * Consts::board_size * Consts::board_size, -1);
  for (int i = 0; i < Consts::word_num; i++) {
    for (int j = 0; j < Consts::word_length - 1; j++) {
      for (const auto& begin_pos: input.char_appear_pos[input.words[i][j]]) {
        for (const auto& end_pos: input.char_appear_pos[input.words[i][Consts::word_length - 1]]) {
          const auto& optimal_path = input.optimal_path_info_dict.at(make_tuple(i, j, begin_pos, end_pos));
          input.optimal_path_score[hash_idx(make_tuple(i, j, begin_pos, end_pos))] = optimal_path.score;
        }
      }
    }
  }
}

Output solve(const Input& input) {
  SAState state(input);
  {
    int score = state.get_score();
    int last_score = score;
    int best_score = score;

    const double base_temperature = 3e0;
    const double target_temperature = 2e-1;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.970;

    while (true) {
      if (iter_count % 1024 == 0 && theTimer.time() > time_limit) break;
      const double roll = theRandom.nextDouble();
      if (roll < 0.2) {
        const int w = theRandom.nextUIntMod(Consts::word_num);
        const int i = theRandom.nextUIntMod(input.char_appear_pos[input.words[w][0]].size());

        const auto orig_pos = state.word_begin_pos[w];
        const auto new_pos = input.char_appear_pos[input.words[w][0]][i];

        state.word_begin_pos[w] = new_pos;

        state.update_score_full();
        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state.word_begin_pos[w] = orig_pos;

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 0.4) {
        const int w = theRandom.nextUIntMod(Consts::word_num);
        const int i = theRandom.nextUIntMod(input.char_appear_pos[input.words[w][Consts::word_length - 1]].size());

        const auto orig_pos = state.word_end_pos[w];
        const auto new_pos = input.char_appear_pos[input.words[w][Consts::word_length - 1]][i];

        state.word_end_pos[w] = new_pos;

        state.update_score_full();
        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state.word_end_pos[w] = orig_pos;

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 0.55) {
        const int w1 = theRandom.nextUIntMod(Consts::word_num);
        const int w2 = theRandom.nextUIntMod(Consts::word_num);
        if (w1 == w2) continue;

        swap(state.word_order[w1], state.word_order[w2]);

        state.update_score_full();
        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          swap(state.word_order[w1], state.word_order[w2]);

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 0.6) {
        const int w1 = theRandom.nextUIntMod(Consts::word_num);
        const int w2 = theRandom.nextUIntMod(Consts::word_num);
        if (w1 == w2) continue;

        const auto orig_begin_pos_w1 = state.word_begin_pos[state.word_order[w1]];
        const auto orig_begin_pos_w2 = state.word_begin_pos[state.word_order[w2]];
        const auto orig_end_pos_w1 = state.word_end_pos[state.word_order[w1]];
        const auto orig_end_pos_w2 = state.word_end_pos[state.word_order[w2]];

        swap(state.word_order[w1], state.word_order[w2]);

        if (w1 > 0 && input.word_begin_pos_of_next[state.word_order[w1 - 1]][state.word_order[w1]] == 0) {
          Position nearest_pos;
          int nearest_dist = 1e9;
          for (const auto& pos : input.char_appear_pos[input.words[state.word_order[w1]][0]]) {
            const int dist = pos.distance_to(state.word_end_pos[state.word_order[w1 - 1]]) + input.optimal_path_score[hash_idx(make_tuple(state.word_order[w1], 0, pos, state.word_end_pos[state.word_order[w1]]))];
            if (dist < nearest_dist) {
              nearest_dist = dist;
              nearest_pos = pos;
            }
          }
          state.word_begin_pos[state.word_order[w1]] = nearest_pos;
        }
        if (w1 < Consts::word_num) {
          Position nearest_pos;
          int nearest_dist = 1e9;
          int word_begin_pos_of_next = input.word_begin_pos_of_next[state.word_order[w1]][state.word_order[w1 + 1]];
          for (const auto& pos : input.char_appear_pos[input.words[state.word_order[w1]][Consts::word_length - 1]]) {
            const int dist = (word_begin_pos_of_next == 0 ? pos.distance_to(state.word_begin_pos[state.word_order[w1 + 1]]) + 1 : 0)
              + input.optimal_path_score[hash_idx(make_tuple(state.word_order[w1 + 1], max(0, word_begin_pos_of_next - 1), pos, state.word_end_pos[state.word_order[w1 + 1]]))];
            if (dist < nearest_dist) {
              nearest_dist = dist;
              nearest_pos = pos;
            }
          }
          state.word_end_pos[state.word_order[w1]] = nearest_pos;
        }
        if (w2 > 0 && input.word_begin_pos_of_next[state.word_order[w2 - 1]][state.word_order[w2]] == 0) {
          Position nearest_pos;
          int nearest_dist = 1e9;
          for (const auto& pos : input.char_appear_pos[input.words[state.word_order[w2]][0]]) {
            const int dist = pos.distance_to(state.word_end_pos[state.word_order[w2 - 1]]) + input.optimal_path_score[hash_idx(make_tuple(state.word_order[w2], 0, pos, state.word_end_pos[state.word_order[w2]]))];
            if (dist < nearest_dist) {
              nearest_dist = dist;
              nearest_pos = pos;
            }
          }
          state.word_begin_pos[state.word_order[w2]] = nearest_pos;
        }
        if (w2 < Consts::word_num) {
          Position nearest_pos;
          int nearest_dist = 1e9;
          int word_begin_pos_of_next = input.word_begin_pos_of_next[state.word_order[w2]][state.word_order[w2 + 1]];
          for (const auto& pos : input.char_appear_pos[input.words[state.word_order[w2]][Consts::word_length - 1]]) {
            const int dist = (word_begin_pos_of_next == 0 ? pos.distance_to(state.word_begin_pos[state.word_order[w2 + 1]]) + 1 : 0)
              + input.optimal_path_score[hash_idx(make_tuple(state.word_order[w2 + 1], max(0, word_begin_pos_of_next - 1), pos, state.word_end_pos[state.word_order[w2 + 1]]))];
            if (dist < nearest_dist) {
              nearest_dist = dist;
              nearest_pos = pos;
            }
          }
          state.word_end_pos[state.word_order[w2]] = nearest_pos;
        }

        state.update_score_full();
        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          swap(state.word_order[w1], state.word_order[w2]);
          state.word_begin_pos[state.word_order[w1]] = orig_begin_pos_w1;
          state.word_begin_pos[state.word_order[w2]] = orig_begin_pos_w2;
          state.word_end_pos[state.word_order[w1]] = orig_end_pos_w1;
          state.word_end_pos[state.word_order[w2]] = orig_end_pos_w2;

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 0.75) {
        const int w1 = theRandom.nextUIntMod(Consts::word_num);
        const int w2 = theRandom.nextUIntMod(Consts::word_num);
        if (w1 == w2) continue;

        const int orig_word_id = state.word_order[w1];
        state.word_order.erase(state.word_order.begin() + w1);
        state.word_order.insert(state.word_order.begin() + w2, orig_word_id);

        state.update_score_full();
        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state.word_order.erase(state.word_order.begin() + w2);
          state.word_order.insert(state.word_order.begin() + w1, orig_word_id);

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 0.8) {
        const int w1 = theRandom.nextUIntMod(Consts::word_num);
        const int w2 = theRandom.nextUIntMod(Consts::word_num);
        if (w1 == w2) continue;

        const auto orig_begin_pos_w1 = state.word_begin_pos[state.word_order[w1]];
        const auto orig_end_pos_w1 = state.word_end_pos[state.word_order[w1]];

        const int orig_word_id = state.word_order[w1];
        state.word_order.erase(state.word_order.begin() + w1);
        state.word_order.insert(state.word_order.begin() + w2, orig_word_id);

        if (w2 > 0 && input.word_begin_pos_of_next[state.word_order[w2 - 1]][state.word_order[w2]] == 0) {
          Position nearest_pos;
          int nearest_dist = 1e9;
          for (const auto& pos : input.char_appear_pos[input.words[state.word_order[w2]][0]]) {
            const int dist = pos.distance_to(state.word_end_pos[state.word_order[w2 - 1]]) + input.optimal_path_score[hash_idx(make_tuple(state.word_order[w2], 0, pos, state.word_end_pos[state.word_order[w2]]))];
            if (dist < nearest_dist) {
              nearest_dist = dist;
              nearest_pos = pos;
            }
          }
          state.word_begin_pos[state.word_order[w2]] = nearest_pos;
        }
        if (w2 < Consts::word_num) {
          Position nearest_pos;
          int nearest_dist = 1e9;
          int word_begin_pos_of_next = input.word_begin_pos_of_next[state.word_order[w2]][state.word_order[w2 + 1]];
          for (const auto& pos : input.char_appear_pos[input.words[state.word_order[w2]][Consts::word_length - 1]]) {
            const int dist = (word_begin_pos_of_next == 0 ? pos.distance_to(state.word_begin_pos[state.word_order[w2 + 1]]) + 1 : 0)
              + input.optimal_path_score[hash_idx(make_tuple(state.word_order[w2 + 1], max(0, word_begin_pos_of_next - 1), pos, state.word_end_pos[state.word_order[w2 + 1]]))];
            if (dist < nearest_dist) {
              nearest_dist = dist;
              nearest_pos = pos;
            }
          }
          state.word_end_pos[state.word_order[w2]] = nearest_pos;
        }

        state.update_score_full();
        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state.word_order.erase(state.word_order.begin() + w2);
          state.word_order.insert(state.word_order.begin() + w1, orig_word_id);
          state.word_begin_pos[state.word_order[w1]] = orig_begin_pos_w1;
          state.word_end_pos[state.word_order[w1]] = orig_end_pos_w1;

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 1.0) {
        const int l = theRandom.nextUIntMod(Consts::word_num);
        const int w = theRandom.nextUIntMod(Consts::word_num - l + 1);
        const int i = theRandom.nextUIntMod(Consts::word_num - l + 1);

        auto orig_order = vector<int>(state.word_order.begin() + w, state.word_order.begin() + w + l);
        state.word_order.erase(state.word_order.begin() + w, state.word_order.begin() + w + l);
        state.word_order.insert(state.word_order.begin() + i, orig_order.begin(), orig_order.end());

        state.update_score_full();
        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state.word_order.erase(state.word_order.begin() + i, state.word_order.begin() + i + l);
          state.word_order.insert(state.word_order.begin() + w, orig_order.begin(), orig_order.end());

          state.update_score_full();
          assert(abs(last_score - state.get_score()) == 0);
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

  return state.get_output();
}

void print_output(const Output& output) {
  for (const auto& pos : output.path) {
    cout << pos.x << " " << pos.y << endl;
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
