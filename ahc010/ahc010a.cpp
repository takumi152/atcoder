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

// hyper parameters

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
};

// constants
constexpr int board_size = 30;

struct {
  const vector<int>  x = { -1,   1,   0,   0};
  const vector<int>  y = {  0,   0,  -1,   1};
  const vector<char> d = {'U', 'D', 'L', 'R'};
} delta;

const vector<vector<int> > rail_next_dir = { // rail_id, current_dir
  {-1,  2, -1,  0},
  { 2, -1, -1,  1},
  { 3, -1,  1, -1},
  {-1,  3,  0, -1},
  { 3,  2,  1,  0},
  { 2,  3,  0,  1},
  {-1, -1,  2,  3},
  { 0,  1, -1, -1},
};

const vector<vector<bool> > rail_exitable = { // rail_id, to_dir
  { true, false,  true, false},
  {false,  true,  true, false},
  {false,  true, false,  true},
  { true, false, false,  true},
  { true,  true,  true,  true},
  { true,  true,  true,  true},
  {false, false,  true,  true},
  { true,  true, false, false},
};

const vector<vector<vector<int> > > rail_rotation_id_table = { // orig_rail_id, from_dir, to_dir
  {
    {-1, -1,  1,  2},
    {-1, -1,  0,  3},
    { 3,  2, -1, -1},
    { 0,  1, -1, -1},
  },
  {
    {-1, -1,  1,  2},
    {-1, -1,  0,  3},
    { 3,  2, -1, -1},
    { 0,  1, -1, -1},
  },
  {
    {-1, -1,  1,  2},
    {-1, -1,  0,  3},
    { 3,  2, -1, -1},
    { 0,  1, -1, -1},
  },
  {
    {-1, -1,  1,  2},
    {-1, -1,  0,  3},
    { 3,  2, -1, -1},
    { 0,  1, -1, -1},
  },
  {
    {-1, -1,  5,  4},
    {-1, -1,  4,  5},
    { 5,  4, -1, -1},
    { 4,  5, -1, -1},
  },
  {
    {-1, -1,  5,  4},
    {-1, -1,  4,  5},
    { 5,  4, -1, -1},
    { 4,  5, -1, -1},
  },
  {
    { 7, -1, -1, -1},
    {-1,  7, -1, -1},
    {-1, -1,  6, -1},
    {-1, -1, -1,  6},
  },
  {
    { 7, -1, -1, -1},
    {-1,  7, -1, -1},
    {-1, -1,  6, -1},
    {-1, -1, -1,  6},
  },
};

const vector<vector<int> > rail_rotation_count_table = { // orig_id, now_id
  { 0,  1,  2,  3, -1, -1, -1, -1},
  { 3,  0,  1,  2, -1, -1, -1, -1},
  { 2,  3,  0,  1, -1, -1, -1, -1},
  { 1,  2,  3,  0, -1, -1, -1, -1},
  {-1, -1, -1, -1,  0,  1, -1, -1},
  {-1, -1, -1, -1,  1,  0, -1, -1},
  {-1, -1, -1, -1, -1, -1,  0,  1},
  {-1, -1, -1, -1, -1, -1,  1,  0},
};

// inputs
vector<vector<int> > board_orig(board_size, vector<int>(board_size));

// outputs
vector<vector<int> > ans(board_size, vector<int>(board_size));

// internals
vector<vector<int> > board(board_size, vector<int>(board_size));
vector<vector<int> > tile_use_count(board_size, vector<int>(board_size));
vector<vector<tuple<int, int, int> > > path_info(2);

void calc_ans_from_board() {
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      assert(rail_rotation_count_table[board_orig[i][j]][board[i][j]] != -1);
      ans[i][j] = rail_rotation_count_table[board_orig[i][j]][board[i][j]];
    }
  }
}

void output_ans() {
  assert(ans.size() == board_size);
  assert(ans[0].size() == board_size);
  calc_ans_from_board();
  for (auto &x: ans) {
    for (auto &y: x) {
      cout << y;
    }
  }
  cout << endl;
}

bool within_board(int x, int y) {
  return x >= 0 && x < board_size && y >= 0 && y < board_size;
}

bool is_loop(int start_x, int start_y, int start_dir) {
  if (!rail_exitable[board[start_x][start_y]][start_dir]) return false;

  int px = start_x + delta.x[start_dir];
  int py = start_y + delta.y[start_dir];
  int dir = start_dir;
  while (true) {
    if (!within_board(px, py)) return false;
    if (rail_next_dir[board[px][py]][dir] == -1) return false;
    dir = rail_next_dir[board[px][py]][dir];
    if (px == start_x && py == start_y && dir == start_dir) return true;
    px += delta.x[dir];
    py += delta.y[dir];
  }
}

bool is_curve(int rail_id) {
  assert(0 <= rail_id && rail_id <= 7);
  return rail_id <= 5;
}

bool is_double_curve(int rail_id) {
  assert(0 <= rail_id && rail_id <= 7);
  return 4 <= rail_id && rail_id <= 5;
}

void init() {
  board = board_orig;

  // find first loop to use
  vector<vector<bool> > used_in_loop(board_size, vector<bool>(board_size));
  priority_queue<tuple<int, int, int>, vector<tuple<int, int, int> >, greater<tuple<int, int, int> > > loop_preference_que; // score, x, y
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      if (used_in_loop[i][j]) continue;
      if (is_loop(i, j, 0)) {
        // loop found
        int px = i + delta.x[0];
        int py = j + delta.y[0];
        int dir = 0;
        while (true) {
          assert(within_board(px, py));
          used_in_loop[px][py] = true;
          assert(rail_next_dir[board[px][py]][dir] != -1);
          dir = rail_next_dir[board[px][py]][dir];
          if (px == i && py == j && dir == 0) break;
          px += delta.x[dir];
          py += delta.y[dir];
        }
        int preference_score = abs(i - board_size / 2) + abs(j - board_size / 2);
        loop_preference_que.emplace(preference_score, i, j);
      }
    }
  }

  for (int k = 0; k < 2; k++) {
    auto [_, sx, sy] = loop_preference_que.top();
    loop_preference_que.pop();
    int px = sx + delta.x[0];
    int py = sy + delta.y[0];
    int dir = 0;
    while (true) {
      assert(within_board(px, py));
      assert(rail_next_dir[board[px][py]][dir] != -1);
      dir = rail_next_dir[board[px][py]][dir];
      path_info[k].emplace_back(px, py, dir);
      tile_use_count[px][py]++;
      if (px == sx && py == sy && dir == 0) break;
      px += delta.x[dir];
      py += delta.y[dir];
    }
  }
}

void solve() {
  auto calc_score_1 = [](int len0, int len1) {
    return min(len0, len1) * 10 + max(len0, len1);
  };

  auto calc_score_2 = [](int len0, int len1) {
    return len0 * len1;
  };

  auto dfs = [&](auto self, int current_x, int current_y, int current_dir, int target_x, int target_y, int target_dir, int &step_count, int step_limit, vector<int> &next_path_info) {
    if (false) return false;

    if (!within_board(current_x, current_y)) return false;
    step_count++;
    if (step_count > step_limit) return false;
    if (!is_curve(board[current_x][current_y])) {
      // straight
      if (tile_use_count[current_x][current_y] > 0) return false;

      next_path_info.push_back(current_dir);
      tile_use_count[current_x][current_y]++;
      bool res = self(self, current_x + delta.x[current_dir], current_y + delta.y[current_dir], current_dir, target_x, target_y, target_dir, step_count, step_limit, next_path_info);
      tile_use_count[current_x][current_y]--;
      if (res) return true;
      next_path_info.pop_back();
    }
    else if (!is_double_curve(board[current_x][current_y])) {
      // single curve
      if (tile_use_count[current_x][current_y] > 0) return false;
      if (current_x == target_x && current_y == target_y) {
        if ((current_dir ^ 2) == target_dir) {
          next_path_info.push_back(current_dir ^ 2);
          return true;
        }
        else if ((current_dir ^ 3) == target_dir) {
          next_path_info.push_back(current_dir ^ 3);
          return true;
        }
        else return false;
      }

      int first_dir_to_try = theRandom.nextUIntMod(2); // random 180 deg

      int dir1 = (current_dir ^ 2) ^ first_dir_to_try;
      next_path_info.push_back(dir1);
      tile_use_count[current_x][current_y]++;
      bool res1 = self(self, current_x + delta.x[dir1], current_y + delta.y[dir1], dir1, target_x, target_y, target_dir, step_count, step_limit, next_path_info);
      tile_use_count[current_x][current_y]--;
      if (res1) return true;
      next_path_info.pop_back();

      int dir2 = (current_dir ^ 2) ^ first_dir_to_try ^ 1;
      next_path_info.push_back(dir2);
      tile_use_count[current_x][current_y]++;
      bool res2 = self(self, current_x + delta.x[dir2], current_y + delta.y[dir2], dir2, target_x, target_y, target_dir, step_count, step_limit, next_path_info);
      tile_use_count[current_x][current_y]--;
      if (res2) return true;
      next_path_info.pop_back();
    }
    else {
      // double curve
      if (tile_use_count[current_x][current_y] == 2) return false;
      else if (tile_use_count[current_x][current_y] == 1) {
        int next_dir = rail_next_dir[board[current_x][current_y]][current_dir];
        if (current_x == target_x && current_y == target_y) {
          if (next_dir == target_dir) {
            next_path_info.push_back(next_dir);
            return true;
          }
        }
        next_path_info.push_back(next_dir);
        tile_use_count[current_x][current_y]++;
        bool res = self(self, current_x + delta.x[next_dir], current_y + delta.y[next_dir], next_dir, target_x, target_y, target_dir, step_count, step_limit, next_path_info);
        tile_use_count[current_x][current_y]--;
        if (res) return true;
        next_path_info.pop_back();
      }
      else {
        int original_tile_id = board[current_x][current_y];

        int first_dir_to_try = theRandom.nextUIntMod(2); // random 180 deg

        int dir1 = (current_dir ^ 2) ^ first_dir_to_try;
        next_path_info.push_back(dir1);
        board[current_x][current_y] = rail_rotation_id_table[board[current_x][current_y]][current_dir][dir1];
        tile_use_count[current_x][current_y]++;
        bool res1 = self(self, current_x + delta.x[dir1], current_y + delta.y[dir1], dir1, target_x, target_y, target_dir, step_count, step_limit, next_path_info);
        tile_use_count[current_x][current_y]--;
        board[current_x][current_y] = original_tile_id;
        if (res1) return true;
        next_path_info.pop_back();

        int dir2 = (current_dir ^ 2) ^ first_dir_to_try ^ 1;
        next_path_info.push_back(dir2);
        board[current_x][current_y] = rail_rotation_id_table[board[current_x][current_y]][current_dir][dir2];
        tile_use_count[current_x][current_y]++;
        bool res2 = self(self, current_x + delta.x[dir2], current_y + delta.y[dir2], dir2, target_x, target_y, target_dir, step_count, step_limit, next_path_info);
        tile_use_count[current_x][current_y]--;
        board[current_x][current_y] = original_tile_id;
        if (res2) return true;
        next_path_info.pop_back();
      }
    }

    return false;
  };

  {

    int score = calc_score_1(path_info[0].size(), path_info[1].size());
    int last_score = score;
    int best_score = score;

    const double base_temperature = 2e2;
    const double target_temperature = 2e0;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    double time_start = theTimer.time();
    const double time_limit = 0.680;
    int iter_count = 0;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 1.00) {
        int lo = theRandom.nextUIntMod(2);
        int id1 = theRandom.nextUIntMod(path_info[lo].size());
        int id2 = theRandom.nextUIntMod(path_info[lo].size());
        if (id1 == id2) continue;
        if (id1 > id2) swap(id1, id2);

        // unuse current path
        for (int i = id1; i <= id2; i++) {
          tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])]--;
          // cerr << i << " " << get<0>(path_info[lo][i]) << " " << get<1>(path_info[lo][i]) << " " << get<2>(path_info[lo][i]) << " " << tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])] << endl;
          assert(tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])] >= 0);
        }

        vector<int> next_path_info;
        int step_count = 0;
        bool res = dfs(dfs, get<0>(path_info[lo][id1]), get<1>(path_info[lo][id1]), rail_next_dir[board[get<0>(path_info[lo][id1])][get<1>(path_info[lo][id1])]][get<2>(path_info[lo][id1]) ^ 1] ^ 1, get<0>(path_info[lo][id2]), get<1>(path_info[lo][id2]), get<2>(path_info[lo][id2]), step_count, 300, next_path_info);
        if (!res) {
          // restore current path
          for (int i = id1; i <= id2; i++) {
            tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])]++;
          }
          continue;
        }

        if (lo == 0) score = calc_score_1(path_info[0].size() - (id2 - id1 + 1) + next_path_info.size(), path_info[1].size());
        else         score = calc_score_1(path_info[0].size(), path_info[1].size() - (id2 - id1 + 1) + next_path_info.size());

        #ifdef DEBUG
        if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          // reserve start info
          int px = get<0>(path_info[lo][id1]);
          int py = get<1>(path_info[lo][id1]);
          int pdir = rail_next_dir[board[get<0>(path_info[lo][id1])][get<1>(path_info[lo][id1])]][get<2>(path_info[lo][id1]) ^ 1] ^ 1;
          // remove current path
          path_info[lo].erase(path_info[lo].begin() + id1, path_info[lo].begin() + id2 + 1);
          // insert new path
          vector<tuple<int, int, int> > path_info_to_insert;
          for (auto &dir: next_path_info) {
            tile_use_count[px][py]++;
            board[px][py] = rail_rotation_id_table[board[px][py]][pdir][dir];
            path_info_to_insert.emplace_back(px, py, dir);
            px += delta.x[dir];
            py += delta.y[dir];
            pdir = dir;
          }
          path_info[lo].insert(path_info[lo].begin() + id1, path_info_to_insert.begin(), path_info_to_insert.end());
          if (!is_loop(get<0>(path_info[lo][0]), get<1>(path_info[lo][0]), get<2>(path_info[lo][0]))) {
            output_ans();
            assert(false);
          }

          last_score = score;
          if (score > best_score) {
            best_score = score;
            output_ans();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          // reserve start info
          int px = get<0>(path_info[lo][id1]);
          int py = get<1>(path_info[lo][id1]);
          int pdir = rail_next_dir[board[get<0>(path_info[lo][id1])][get<1>(path_info[lo][id1])]][get<2>(path_info[lo][id1]) ^ 1] ^ 1;
          // remove current path
          path_info[lo].erase(path_info[lo].begin() + id1, path_info[lo].begin() + id2 + 1);
          // insert new path
          vector<tuple<int, int, int> > path_info_to_insert;
          for (auto &dir: next_path_info) {
            tile_use_count[px][py]++;
            board[px][py] = rail_rotation_id_table[board[px][py]][pdir][dir];
            path_info_to_insert.emplace_back(px, py, dir);
            px += delta.x[dir];
            py += delta.y[dir];
            pdir = dir;
          }
          path_info[lo].insert(path_info[lo].begin() + id1, path_info_to_insert.begin(), path_info_to_insert.end());
          if (!is_loop(get<0>(path_info[lo][0]), get<1>(path_info[lo][0]), get<2>(path_info[lo][0]))) {
            output_ans();
            assert(false);
          }

          last_score = score;
        }
        else { // rollback
          for (int i = id1; i <= id2; i++) {
            tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])]++;
          }
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
  }

  {

    int score = calc_score_2(path_info[0].size(), path_info[1].size());
    int last_score = score;
    int best_score = score;

    const double base_temperature = 1e3;
    const double target_temperature = 1e2;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    double time_start = theTimer.time();
    const double time_limit = 1.980;
    int iter_count = 0;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 1.00) {
        int lo = theRandom.nextUIntMod(2);
        int id1 = theRandom.nextUIntMod(path_info[lo].size());
        int id2 = theRandom.nextUIntMod(path_info[lo].size());
        if (id1 == id2) continue;
        if (id1 > id2) swap(id1, id2);

        // unuse current path
        for (int i = id1; i <= id2; i++) {
          tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])]--;
          // cerr << i << " " << get<0>(path_info[lo][i]) << " " << get<1>(path_info[lo][i]) << " " << get<2>(path_info[lo][i]) << " " << tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])] << endl;
          assert(tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])] >= 0);
        }

        vector<int> next_path_info;
        int step_count = 0;
        bool res = dfs(dfs, get<0>(path_info[lo][id1]), get<1>(path_info[lo][id1]), rail_next_dir[board[get<0>(path_info[lo][id1])][get<1>(path_info[lo][id1])]][get<2>(path_info[lo][id1]) ^ 1] ^ 1, get<0>(path_info[lo][id2]), get<1>(path_info[lo][id2]), get<2>(path_info[lo][id2]), step_count, 300, next_path_info);
        if (!res) {
          // restore current path
          for (int i = id1; i <= id2; i++) {
            tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])]++;
          }
          continue;
        }

        if (lo == 0) score = calc_score_2(path_info[0].size() - (id2 - id1 + 1) + next_path_info.size(), path_info[1].size());
        else         score = calc_score_2(path_info[0].size(), path_info[1].size() - (id2 - id1 + 1) + next_path_info.size());

        #ifdef DEBUG
        if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          // reserve start info
          int px = get<0>(path_info[lo][id1]);
          int py = get<1>(path_info[lo][id1]);
          int pdir = rail_next_dir[board[get<0>(path_info[lo][id1])][get<1>(path_info[lo][id1])]][get<2>(path_info[lo][id1]) ^ 1] ^ 1;
          // remove current path
          path_info[lo].erase(path_info[lo].begin() + id1, path_info[lo].begin() + id2 + 1);
          // insert new path
          vector<tuple<int, int, int> > path_info_to_insert;
          for (auto &dir: next_path_info) {
            tile_use_count[px][py]++;
            board[px][py] = rail_rotation_id_table[board[px][py]][pdir][dir];
            path_info_to_insert.emplace_back(px, py, dir);
            px += delta.x[dir];
            py += delta.y[dir];
            pdir = dir;
          }
          path_info[lo].insert(path_info[lo].begin() + id1, path_info_to_insert.begin(), path_info_to_insert.end());
          if (!is_loop(get<0>(path_info[lo][0]), get<1>(path_info[lo][0]), get<2>(path_info[lo][0]))) {
            output_ans();
            assert(false);
          }

          last_score = score;
          if (score > best_score) {
            best_score = score;
            output_ans();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          // reserve start info
          int px = get<0>(path_info[lo][id1]);
          int py = get<1>(path_info[lo][id1]);
          int pdir = rail_next_dir[board[get<0>(path_info[lo][id1])][get<1>(path_info[lo][id1])]][get<2>(path_info[lo][id1]) ^ 1] ^ 1;
          // remove current path
          path_info[lo].erase(path_info[lo].begin() + id1, path_info[lo].begin() + id2 + 1);
          // insert new path
          vector<tuple<int, int, int> > path_info_to_insert;
          for (auto &dir: next_path_info) {
            tile_use_count[px][py]++;
            board[px][py] = rail_rotation_id_table[board[px][py]][pdir][dir];
            path_info_to_insert.emplace_back(px, py, dir);
            px += delta.x[dir];
            py += delta.y[dir];
            pdir = dir;
          }
          path_info[lo].insert(path_info[lo].begin() + id1, path_info_to_insert.begin(), path_info_to_insert.end());
          if (!is_loop(get<0>(path_info[lo][0]), get<1>(path_info[lo][0]), get<2>(path_info[lo][0]))) {
            output_ans();
            assert(false);
          }

          last_score = score;
        }
        else { // rollback
          for (int i = id1; i <= id2; i++) {
            tile_use_count[get<0>(path_info[lo][i])][get<1>(path_info[lo][i])]++;
          }
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
  }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  for (auto &x: board_orig) {
    for (auto &y: x) {
      char c;
      cin >> c;
      y = c - '0';
    }
  }

  init();
  solve();

  output_ans();

  return 0;
}
