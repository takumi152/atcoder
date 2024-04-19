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
};

struct CropInfo {
  int id;
  int plant_time;
  int harvest_time;

  CropInfo() = default;
  CropInfo(const int id, const int plant_time, const int harvest_time): id(id), plant_time(plant_time), harvest_time(harvest_time) {}
};

struct PlantInfo {
  int id;
  Position plant_pos;
  int plant_time;

  PlantInfo() = default;
  PlantInfo(const int id, const Position &plant_pos, const int plant_time): id(id), plant_pos(plant_pos), plant_time(plant_time) {}
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
  constexpr static int turn_max = 100;
  constexpr static int board_size = 20;

  Position entrance_pos;
  
  vector<vector<bool>> wall_horizontal;
  vector<vector<bool>> wall_vertical;

  vector<CropInfo> crop_infos;

  vector<vector<int>> distance_from_entrance;
};

// outputs
struct Output {
  vector<PlantInfo> plant_infos;
};

// internals

bool within_board(const Input &input, const Position &pos) {
  return 0 <= pos.x && pos.x < input.board_size && 0 <= pos.y && pos.y < input.board_size;
}

bool is_adjacent(const Input input, const Position &pos, const int dir) {
  return (
    within_board(input, Position(pos.x + Adj4::dx[dir], pos.y + Adj4::dy[dir])) &&
    (
      (Adj4::dx[dir] == -1 && !input.wall_horizontal[pos.x - 1][pos.y]) ||
      (Adj4::dx[dir] ==  1 && !input.wall_horizontal[pos.x][pos.y]) ||
      (Adj4::dy[dir] == -1 && !input.wall_vertical[pos.x][pos.y - 1]) ||
      (Adj4::dy[dir] ==  1 && !input.wall_vertical[pos.x][pos.y])
    )
  );
}

Input get_input() {
  Input input;

  int _t, _h, _w, i0;
  cin >> _t >> _h >> _w >> i0;
  input.entrance_pos = Position(i0, 0);

  input.wall_horizontal = vector<vector<bool>>(input.board_size - 1, vector<bool>(input.board_size));
  for (int i = 0; i < input.board_size - 1; i++) {
    for (int j = 0; j < input.board_size; j++) {
      char c;
      cin >> c;
      input.wall_horizontal[i][j] = (c == '1');
    }
  }
  input.wall_vertical = vector<vector<bool>>(input.board_size, vector<bool>(input.board_size - 1));
  for (int i = 0; i < input.board_size; i++) {
    for (int j = 0; j < input.board_size - 1; j++) {
      char c;
      cin >> c;
      input.wall_vertical[i][j] = (c == '1');
    }
  }

  int k;
  cin >> k;
  input.crop_infos = vector<CropInfo>(k);
  for (int i = 0; i < k; i++) {
    int s, d;
    cin >> s >> d;
    input.crop_infos[i] = CropInfo(i, s - 1, d - 1);
  }

  return input;
}

void init(Input& input) {
  input.distance_from_entrance = vector<vector<int>>(input.board_size, vector<int>(input.board_size, 999));
  {
    queue<pair<int, Position>> que;
    que.emplace(0, input.entrance_pos);
    while (!que.empty()) {
      auto [dist, pos] = que.front();
      que.pop();
      if (dist >= input.distance_from_entrance[pos.x][pos.y]) continue;
      input.distance_from_entrance[pos.x][pos.y] = dist;
      for (int d = 0; d < 4; d++) {
        if (is_adjacent(input, pos, d) && dist + 1 < input.distance_from_entrance[pos.x + Adj4::dx[d]][pos.y + Adj4::dy[d]]) {
          que.emplace(dist + 1, Position(pos.x + Adj4::dx[d], pos.y + Adj4::dy[d]));
        }
      }
    }
  }
}

Output solve(const Input& input) {
  struct SAState {
    const Input& input;

    vector<vector<int>> plant_harvest_pair_num;

    vector<vector<vector<bool>>> harvest_on_time;
    vector<vector<vector<int>>> harvest_times;

    SAState(const Input& input): input(input) {
      plant_harvest_pair_num = vector<vector<int>>(input.turn_max, vector<int>(input.turn_max));
      for (auto &c: input.crop_infos) {
        plant_harvest_pair_num[c.plant_time][c.harvest_time]++;
      }

      harvest_on_time = vector<vector<vector<bool>>>(input.board_size, vector<vector<bool>>(input.board_size));
      harvest_times = vector<vector<vector<int>>>(input.board_size, vector<vector<int>>(input.board_size));

      update_score_full();
    }

    bool flip(const Position& position, const int time) {
      
    }

    void update_score_full() {

    }

    double get_score() {
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
    const double time_limit = 1.600;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 1.00) {
        int x = theRandom.nextUIntMod(input.board_size);
        int y = theRandom.nextUIntMod(input.board_size);
        int d = theRandom.nextUIntMod(4);

        int orig_dir = state.parent_dir[x][y];

        bool valid = state.change_parent_dir(Position(x, y), d);
        if (!valid) continue;

        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          state.change_parent_dir(Position(x, y), orig_dir);
          if (state.get_score() != last_score) {
            cerr << "expected = " << last_score << ", actual = " << state.get_score() << endl;
            cerr << x << " " << y << " " << d << endl;
            state.print_parent_dir();
            assert(false);
          }
          score = last_score;
        }
      }

      // temperature *= 1.0 - decay_rate;
      // if (iter_count == 100000) break;
      temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      iter_count++;
    }

    cerr << "iter_count               = " << iter_count << endl;
    cerr << "score                    = " << score << endl;
    cerr << "total_leaf_score         = " << state.total_leaf_score << endl;
    cerr << "excess_leaf_length_score = " << state.excess_leaf_length_score << endl;
    cerr << "best_score               = " << best_score << endl;
    cerr << "temperature              = " << temperature << endl;
  }

  struct TreeNode {
    const SAState& state;

    vector<unique_ptr<TreeNode>> branch, leaf;
    vector<Position> node_positions;
    int subtree_size;

    TreeNode(const SAState& state): state(state) {}

    void dfs_create_tree(const Position& base_pos) {
      Position now_pos = base_pos;
      node_positions.push_back(now_pos);
      while (state.dir_is_child[now_pos.x][now_pos.y].count() == 1) {
        int next_child_dir = __builtin_ctz(state.dir_is_child[now_pos.x][now_pos.y].to_ulong());
        now_pos.x += Adj4::dx[next_child_dir];
        now_pos.y += Adj4::dy[next_child_dir];
        node_positions.push_back(now_pos);
      }
      reverse(node_positions.begin(), node_positions.end());
      subtree_size = node_positions.size();

      for (int d = 0; d < 4; d++) {
        if (state.dir_is_child[now_pos.x][now_pos.y][d]) {
          auto child = make_unique<TreeNode>(state);
          child->dfs_create_tree(Position(now_pos.x + Adj4::dx[d], now_pos.y + Adj4::dy[d]));
          subtree_size += child->subtree_size;
          if (child->branch.empty() && child->leaf.empty()) {
            leaf.push_back(move(child));
          }
          else {
            branch.push_back(move(child));
          }
        }
      }
      sort(branch.rbegin(), branch.rend(), [](auto &a, auto &b){return a->subtree_size < b->subtree_size;});
      sort(leaf.rbegin(), leaf.rend(), [](auto &a, auto &b){return a->subtree_size < b->subtree_size;});
    }
  };

  auto tree_root = make_unique<TreeNode>(state);
  tree_root->dfs_create_tree(input.entrance_pos);

  struct DPChainCreator {
    const Input& input;
    
    vector<vector<int>> plant_harvest_pair_num;
    vector<int> plant_time_num;
    vector<int> harvest_time_num;
    vector<vector<vector<int>>> plant_harvest_pair_ids;

    vector<double> plant_time_num_ratio;
    vector<double> harvest_time_num_ratio;

    DPChainCreator(const Input& input): input(input) {
      plant_harvest_pair_num = vector<vector<int>>(input.turn_max, vector<int>(input.turn_max));
      plant_time_num = vector<int>(input.turn_max);
      harvest_time_num = vector<int>(input.turn_max);
      plant_harvest_pair_ids = vector<vector<vector<int>>>(input.turn_max, vector<vector<int>>(input.turn_max));
      for (auto &c: input.crop_infos) {
        plant_harvest_pair_num[c.plant_time][c.harvest_time]++;
        plant_time_num[c.plant_time]++;
        harvest_time_num[c.harvest_time]++;
        plant_harvest_pair_ids[c.plant_time][c.harvest_time].push_back(c.id);
      }

      plant_time_num_ratio = vector<double>(input.turn_max);
      harvest_time_num_ratio = vector<double>(input.turn_max);
      for (int i = 0; i < input.turn_max; i++) {
        plant_time_num_ratio[i] = (double) plant_time_num[i] / (double) input.crop_infos.size();
        harvest_time_num_ratio[i] = (double) harvest_time_num[i] / (double) input.crop_infos.size();
      }
    }

    // 最大長さmax_lengthのchainを作成する
    vector<vector<int>> get_chain(const int max_length, const vector<Pii>& timespan_restriction, const vector<double>& separation_penalty) {
      vector<vector<bool>> allowed_timespan(input.turn_max, vector<bool>(input.turn_max));
      for (auto &[timespan_begin, timespan_end]: timespan_restriction) {
        for (int i = timespan_begin; i <= timespan_end; i++) {
          for (int j = i; j <= timespan_end; j++) {
            allowed_timespan[i][j] = true;
          }
        }
      }

      auto separation_time = get_separation_time(max_length, allowed_timespan, separation_penalty);
      auto best_raw_chain = get_best_raw_chain(separation_time, allowed_timespan, max_length);

      // for (auto &x: best_raw_chain) {
      //   for (auto &y: x) {
      //     cerr << "(" << y.first << ", " << y.second << "), ";
      //   }
      //   cerr << endl;
      // }
      // cerr << endl;

      vector<vector<int>> best_chain(best_raw_chain.size());
      vector<vector<int>> id_use_count(input.turn_max, vector<int>(input.turn_max, 0));
      for (int i = 0; i < (int) best_raw_chain.size(); i++) {
        for (int j = 0; j < (int) best_raw_chain[i].size(); j++) {
          best_chain[i].push_back(plant_harvest_pair_ids[best_raw_chain[i][j].first][best_raw_chain[i][j].second][id_use_count[best_raw_chain[i][j].first][best_raw_chain[i][j].second]]);
          id_use_count[best_raw_chain[i][j].first][best_raw_chain[i][j].second]++;
        }
      }

      // for (auto &x: best_chain) {
      //   for (auto &y: x) {
      //     cerr << y << ", ";
      //   }
      //   cerr << endl;
      // }

      for (int i = 0; i < input.turn_max; i++) {
        for (int j = 0; j < input.turn_max; j++) {
          plant_harvest_pair_num[i][j] -= id_use_count[i][j];
          plant_time_num[i] -= id_use_count[i][j];
          harvest_time_num[j] -= id_use_count[i][j];
          plant_harvest_pair_ids[i][j].erase(plant_harvest_pair_ids[i][j].begin(), plant_harvest_pair_ids[i][j].begin() + id_use_count[i][j]);
        }
      }

      int remaining_crop = 0;
      for (int i = 0; i < input.turn_max; i++) {
        remaining_crop += plant_time_num[i];
      }
      for (int i = 0; i < input.turn_max; i++) {
        plant_time_num_ratio[i] = (double) plant_time_num[i] / (double) max(1, remaining_crop);
        harvest_time_num_ratio[i] = (double) harvest_time_num[i] / (double) max(1, remaining_crop);
      }

      return best_chain;
    }

    // dp[i][j][k] := 区間[i, j]の中から部分長が最大でkのchainを使う場合の、実現可能な最大スコア + chainを構成する作物の収穫までの長さによるボーナス
    // sep[i][j][k] := 区間[i, j]の中から部分長が最大でkのchainを使う場合で、実現可能な最大スコアを得る場合の次に分割する時刻(-1ならこれ以上分割しない)
    // を求めて、sepを返す
    vector<vector<vector<int>>> get_separation_time(const int max_length, const vector<vector<bool>>& allowed_timespan, const vector<double>& separation_penalty) {
      const double base_score_factor = HyperParameter::base_score_factor;
      const double crop_lifetime_score_factor = HyperParameter::crop_lifetime_score_factor;
      const double plant_harvest_num_score_factor = HyperParameter::plant_harvest_num_score_factor;

      vector<vector<vector<double>>> dp(input.turn_max, vector<vector<double>>(input.turn_max, vector<double>(max_length + 1, 0)));
      vector<vector<vector<int>>> sep(input.turn_max, vector<vector<int>>(input.turn_max, vector<int>(max_length + 1, -1)));
      for (int j = 0; j < input.turn_max; j++) {
        for (int i = j; i >= 0; i--) {
          int use_num = allowed_timespan[i][j] ? min(max_length, plant_harvest_pair_num[i][j]) : 0;
          for (int k = 0; k <= max_length; k++) {
            if (k <= use_num) {
              dp[i][j][k] = (double)(k * (j - i + 1)) * base_score_factor + (double)(k * (j - i + 1) * (j - i + 1)) * crop_lifetime_score_factor + (1.0 - plant_time_num_ratio[i]) * (1.0 - harvest_time_num_ratio[j]) * plant_harvest_num_score_factor;
            }
            else {
              for (int l = i; l < j; l++) {
                if (dp[i][l][k - use_num] + dp[l + 1][j][k - use_num] - separation_penalty[l] > dp[i][j][k]) {
                  dp[i][j][k] = dp[i][l][k - use_num] + dp[l + 1][j][k - use_num];
                  sep[i][j][k] = l;
                }
              }
              dp[i][j][k] += (double)(use_num * (j - i + 1)) * base_score_factor + use_num * (double)((j - i + 1) * (j - i + 1)) * crop_lifetime_score_factor;
            }
          }
        }
      }

      // for (auto &x: dp) {
      //   for (auto &y: x) {
      //     cerr << y[max_length] << '\t';
      //   }
      //   cerr << endl;
      // }

      return sep;
    }

    vector<vector<Pii>> get_best_raw_chain(const vector<vector<vector<int>>>& separation_time, const vector<vector<bool>>& allowed_timespan, int max_length) {
      vector<vector<Pii>> ans;
      dfs_for_best_raw_chain(0, input.turn_max - 1, max_length, max_length, ans, separation_time, allowed_timespan);
      return ans;
    }

    void dfs_for_best_raw_chain(int begin, int end, int chain_remaining, const int max_length, vector<vector<Pii>>& ans, const vector<vector<vector<int>>>& sep, const vector<vector<bool>>& allowed_timespan) {
      int chain_remaining_now = chain_remaining;
      if (allowed_timespan[begin][end]) {
        for (int i = 0; i < plant_harvest_pair_num[begin][end]; i++) {
          if (chain_remaining_now <= 0) break;
          int idx = max_length - chain_remaining_now;
          if (idx >= (int) ans.size()) {
            ans.push_back(vector<Pii>());
          }
          ans[idx].push_back(Pii(begin, end));
          chain_remaining_now--;
        }
      }
      if (chain_remaining_now > 0 && sep[begin][end][chain_remaining] != -1) {
        dfs_for_best_raw_chain(begin, sep[begin][end][chain_remaining], chain_remaining_now, max_length, ans, sep, allowed_timespan);
        dfs_for_best_raw_chain(sep[begin][end][chain_remaining] + 1, end, chain_remaining_now, max_length, ans, sep, allowed_timespan);
      }
    }
  };

  int score = 0;
  DPChainCreator chain_creator(input);
  Output output;
  {
    vector<vector<vector<Pii>>> chain_timespans(input.board_size, vector<vector<Pii>>(input.board_size));

    const double base_separation_penalty = HyperParameter::base_separation_penalty;
    const double separation_penalty_decay_rate = HyperParameter::separation_penalty_decay_rate;

    auto dfs_chain_creation = [&](auto self, const unique_ptr<TreeNode>& node_now, const vector<double>& parent_separation_penalty) {
      if (false) return pair<vector<Pii>, vector<double>>(vector<Pii>(), vector<double>());

      vector<double> separation_penalty = parent_separation_penalty;
      if (!(node_now->branch.empty() && node_now->leaf.empty())) {
        for (auto &sp: separation_penalty) {
          sp *= separation_penalty_decay_rate;
        }
      }

      bool is_first = true;
      vector<int> timespan_begin, timespan_end;
      for (auto &branch_node: node_now->branch) {
        auto [timespan_used, child_separation_penalty] = self(self, branch_node, separation_penalty);
        for (auto &[begin, end]: timespan_used) {
          timespan_begin.push_back(begin);
          timespan_end.push_back(end);
        }
        for (int i = 0; i < (int) separation_penalty.size(); i++) {
          if (is_first) {
            separation_penalty[i] = max(separation_penalty[i], child_separation_penalty[i]);
          }
          else {
            separation_penalty[i] = min(separation_penalty[i], child_separation_penalty[i]);
          }
        }
        is_first = false;
      }
      for (auto &leaf_node: node_now->leaf) {
        auto [timespan_used, child_separation_penalty] = self(self, leaf_node, separation_penalty);
        for (auto &[begin, end]: timespan_used) {
          timespan_begin.push_back(begin);
          timespan_end.push_back(end);
        }
        for (int i = 0; i < (int) separation_penalty.size(); i++) {
          if (is_first) {
            separation_penalty[i] = max(separation_penalty[i], child_separation_penalty[i]);
          }
          else {
            separation_penalty[i] = min(separation_penalty[i], child_separation_penalty[i]);
          }
        }
        is_first = false;
      }
      {
        sort(timespan_begin.begin(), timespan_begin.end());
        auto it_begin = unique(timespan_begin.begin(), timespan_begin.end());
        timespan_begin.erase(it_begin, timespan_begin.end());
        sort(timespan_end.begin(), timespan_end.end());
        auto it_end = unique(timespan_end.begin(), timespan_end.end());
        timespan_end.erase(it_end, timespan_end.end());
      }

      vector<Pii> timespan_restriction;
      {
        int begin_time = 0;
        while (begin_time < input.turn_max) {
          auto it_next_begin = upper_bound(timespan_begin.begin(), timespan_begin.end(), begin_time);
          int next_begin_time;
          if (it_next_begin == timespan_begin.end()) {
            next_begin_time = input.turn_max;
          }
          else {
            next_begin_time = *it_next_begin;
          }
          auto it_next_end = lower_bound(timespan_end.begin(), timespan_end.end(), begin_time);
          int next_end_time;
          if (it_next_end == timespan_end.end()) {
            next_end_time = input.turn_max - 1;
          }
          else {
            next_end_time = *it_next_end;
          }
          int end_time = min(next_begin_time - 1, next_end_time);
          timespan_restriction.emplace_back(begin_time, end_time);
          begin_time = end_time + 1;
        }
      }
      if (timespan_restriction.empty()){
        timespan_restriction.emplace_back(0, input.turn_max - 1);
      }

      // cerr << node_now->node_positions.back().x << " " << node_now->node_positions.back().y << endl;
      // for (auto &[b, e]: timespan_restriction) {
      //   cerr << "(" << b + 1 << ", " << e + 1 << "), ";
      // }
      // cerr << endl;
      // for (int i = 0; i < (int) separation_penalty.size(); i++) {
      //   cerr << "(" << i + 1 << ", " << parent_separation_penalty[i] << "), ";
      // }
      // cerr << endl;
      // for (int i = 0; i < (int) separation_penalty.size(); i++) {
      //   cerr << "(" << i + 1 << ", " << separation_penalty[i] << "), ";
      // }
      // cerr << endl;

      auto chain_ids = chain_creator.get_chain(node_now->node_positions.size(), timespan_restriction, separation_penalty);
      for (int i = 0; i < (int) chain_ids.size(); i++) {
        auto pos = node_now->node_positions[i];
        for (auto &id: chain_ids[i]) {
          output.plant_infos.emplace_back(id, pos, input.crop_infos[id].plant_time);
          timespan_begin.push_back(input.crop_infos[id].plant_time);
          timespan_end.push_back(input.crop_infos[id].harvest_time);
          score += (input.crop_infos[id].harvest_time - input.crop_infos[id].plant_time + 1) * 25;
        }
      }
      {
        sort(timespan_begin.begin(), timespan_begin.end());
        auto it_begin = unique(timespan_begin.begin(), timespan_begin.end());
        timespan_begin.erase(it_begin, timespan_begin.end());
        sort(timespan_end.begin(), timespan_end.end());
        auto it_end = unique(timespan_end.begin(), timespan_end.end());
        timespan_end.erase(it_end, timespan_end.end());
      }

      vector<Pii> timespan_restriction_after;
      {
        int begin_time = 0;
        while (begin_time < input.turn_max) {
          auto it_next_begin = upper_bound(timespan_begin.begin(), timespan_begin.end(), begin_time);
          int next_begin_time;
          if (it_next_begin == timespan_begin.end()) {
            next_begin_time = input.turn_max;
          }
          else {
            next_begin_time = *it_next_begin;
          }
          auto it_next_end = lower_bound(timespan_end.begin(), timespan_end.end(), begin_time);
          int next_end_time;
          if (it_next_end == timespan_end.end()) {
            next_end_time = input.turn_max - 1;
          }
          else {
            next_end_time = *it_next_end;
          }
          int end_time = min(next_begin_time - 1, next_end_time);
          timespan_restriction_after.emplace_back(begin_time, end_time);
          begin_time = end_time + 1;
        }
      }

      vector<double> separation_penalty_after;
      if (node_now->branch.empty() && node_now->leaf.empty()) {
        separation_penalty_after.resize(separation_penalty.size());
        for (auto &sp: separation_penalty_after) {
          sp = base_separation_penalty;
        }
      }
      else {
        separation_penalty_after = separation_penalty;
      }
      for (auto &[begin, end]: timespan_restriction_after) {
        separation_penalty_after[end] = 0.0;
      }

      return make_pair(timespan_restriction_after, separation_penalty_after);
    };
    dfs_chain_creation(dfs_chain_creation, tree_root, vector<double>(input.turn_max));
  }

  cerr << "score = " << score << endl;

  return output;
}

void print_output(const Output& output) {
  cout << output.plant_infos.size() << endl;
  for (auto &pi: output.plant_infos) {
    cout << pi.id + 1 << " " << pi.plant_pos.x << " " << pi.plant_pos.y << " " << pi.plant_time + 1 << endl;
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
