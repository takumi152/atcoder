#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
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

// hyper parameters

// constants
constexpr int turn_limit = 300;
constexpr int board_size = 30;

struct {
  const vector<int>  x = { -1,   1,   0,   0};
  const vector<int>  y = {  0,   0,  -1,   1};
  const vector<char> b = {'u', 'd', 'l', 'r'};
  const vector<char> m = {'U', 'D', 'L', 'R'};
} delta;

// inputs
int n, m;
vector<int> px, py, pt;
vector<int> hx, hy;

// outputs

// internals
int turn_count = 0;

vector<vector<bool> > blocked(board_size, vector<bool>(board_size, false));
vector<vector<bool> > blockable(board_size, vector<bool>(board_size, true));
vector<vector<bool> > move_on_next_turn(board_size, vector<bool>(board_size, false));
vector<vector<bool> > block_on_next_turn(board_size, vector<bool>(board_size, false));

void comment(string s) {
  cout << "#" << s << endl;
}

bool within_board(int x, int y) {
  return x >= 0 && x < board_size && y >= 0 && y < board_size;
}

string shortest_movement(int sx, int sy, int tx, int ty) {
  assert(within_board(sx, sy));
  assert(within_board(tx, ty));

  if (sx == tx && sy == ty) return ""; // already at target

  // search from (tx, ty) to (sx, sy), so the final path does not have to be reversed
  vector<vector<bool> > visited(board_size, vector<bool>(board_size, false));
  vector<vector<int> > prev(board_size, vector<int>(board_size, -1));
  queue<tuple<int, int, int> > que;
  que.emplace(tx, ty, -1);
  while (!que.empty()) {
    auto [nx, ny, dir] = que.front();
    que.pop();
    if (visited[nx][ny]) continue;
    visited[nx][ny] = true;
    prev[nx][ny] = dir;
    if (nx == sx && ny == sy) break;
    for (int d = 0; d < 4; d++) {
      if (within_board(nx + delta.x[d], ny + delta.y[d])) {
        if (!visited[nx + delta.x[d]][ny + delta.y[d]] && !blocked[nx + delta.x[d]][ny + delta.y[d]] && !block_on_next_turn[nx + delta.x[d]][ny + delta.y[d]]) {
          que.emplace(nx + delta.x[d], ny + delta.y[d], d ^ 1); // xor with 1 to reverse direction
        }
      }
    }
  }

  // use prev to obtain final path
  if (prev[sx][sy] == -1) return "."; // no path found
  string shortest_str = "";
  {
    int nx = sx;
    int ny = sy;
    while (!(nx == tx && ny == ty)) {
      assert(prev[nx][ny] != -1);
      assert(within_board(nx + delta.x[prev[nx][ny]], ny + delta.y[prev[nx][ny]]));
      shortest_str.push_back(delta.m[prev[nx][ny]]);
      int dx = delta.x[prev[nx][ny]];
      int dy = delta.y[prev[nx][ny]];
      nx += dx;
      ny += dy;
    }
  }

  return shortest_str;
}

void update(vector<char> human_move) {
  assert((int) human_move.size() == m);
  for (int i = 0; i < m; i++) {
    for (int d = 0; d < 4; d++) {
      if (human_move[i] == delta.b[d]) {
        assert(within_board(hx[i] + delta.x[d], hy[i] + delta.y[d]));
        assert(blockable[hx[i] + delta.x[d]][hy[i] + delta.y[d]]);
        assert(!blocked[hx[i] + delta.x[d]][hy[i] + delta.y[d]]);
        break;
      }
      else if (human_move[i] == delta.m[d]) {
        assert(within_board(hx[i] + delta.x[d], hy[i] + delta.y[d]));
        assert(!blocked[hx[i] + delta.x[d]][hy[i] + delta.y[d]]);
        break;
      }
    }
    cout << human_move[i];
  }
  cout << endl;

  vector<string> pet_move(n);
  for (int i = 0; i < n; i++) {
    cin >> pet_move[i];
  }

  blockable = vector<vector<bool> >(board_size, vector<bool>(board_size, true));
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      if (blocked[i][j]) blockable[i][j] = false; // no "double blocking"
    }
  }
  // apply human moves
  for (int i = 0; i < m; i++) {
    for (int d = 0; d < 4; d++) {
      if (human_move[i] == delta.b[d]) {
        blocked[hx[i] + delta.x[d]][hy[i] + delta.y[d]] = true;
        break;
      }
      else if (human_move[i] == delta.m[d]) {
        hx[i] += delta.x[d];
        hy[i] += delta.y[d];
        break;
      }
    }
    blockable[hx[i]][hy[i]] = false;
  }
  // apply pet moves
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < (int) pet_move[i].size(); j++) {
      for (int d = 0; d < 4; d++) {
        if (pet_move[i][j] == delta.m[d]) {
          assert(within_board(px[i] + delta.x[d], py[i] + delta.y[d]));
          px[i] += delta.x[d];
          py[i] += delta.y[d];
          break;
        }
      }
    }
    blockable[px[i]][py[i]] = false;
    for (int d = 0; d < 4; d++) {
      if (within_board(px[i] + delta.x[d], py[i] + delta.y[d])) {
        blockable[px[i] + delta.x[d]][py[i] + delta.y[d]] = false;
      }
    }
  }

  move_on_next_turn = vector<vector<bool> >(board_size, vector<bool>(board_size, false));
  block_on_next_turn = vector<vector<bool> >(board_size, vector<bool>(board_size, false));
}

void init() {
  for (int i = 0; i < m; i++) {
    blockable[hx[i]][hy[i]] = false;
  }
  for (int i = 0; i < n; i++) {
    blockable[px[i]][py[i]] = false;
    for (int d = 0; d < 4; d++) {
      if (within_board(px[i] + delta.x[d], py[i] + delta.y[d])) {
        blockable[px[i] + delta.x[d]][py[i] + delta.y[d]] = false;
      }
    }
  }
}

void solve_10x2() {
  vector<queue<char> > move_queue(m);

  struct _wall_manager {
    const vector<string> wall_creation_move = {          // 10x2 cells
      "lRRUdlUlUlUlUlUlUlUlUlUlUlUlUlRRRlDlDlDlDlDlDlDlDlDlDlDlDlDRlRRUdlUlUlUlUlUlUlUlUlUlUlUlUlRRRlDlDlDlDlDlDlDlDlDlDlDlDlDRl", // upper left walls
      "DlDulDlDlDlDlDlDlDlDlDlDlDDu", // upper center wall
      "rLLUdrUrUrUrUrUrUrUrUrUrUrUrUrLLLrDrDrDrDrDrDrDrDrDrDrDrDrDLrLLUdrUrUrUrUrUrUrUrUrUrUrUrUrLLLrDrDurDrDrDrDrDrDrDrDrDrDrDLr", // upper right walls
      "lRRDulDlDlDlDlDlDlDlDlDlDlDlDlDlRRRlUlUlUlUlUlUlUlUlUlUlUlUlUlURlRRDulDlDlDlDlDlDlDlDlDlDlDlDlDlRRRlUlUlUlUlUlUlUlUlUlUlUlUlUlURl", // lower left walls
      "rUrUrUrUrUrUrUrUrUrUrUrUrUrUUd", // lower center wall
      "rLLDurDrDrDrDrDrDrDrDrDrDrDrDrDrLLLrUrUrUrUrUrUrUrUrUrUrUrUrUrULrLLDurDrDrDrDrDrDrDrDrDrDrDrDrDrLLLrUrUrUrUrUrUrUrUrUrUrUrUrUrULr", // lower right walls
    };
    const int wall_count = 6;
    bool all_assigned;
    vector<bool> assigned;
    vector<tuple<int, int> > wall_assignment;
    const vector<vector<tuple<int, int, int, int> > > wall_start_info = { // sx, sy, move_type, priority mult
      {tuple<int, int, int, int>(13,  1,  0,  1)},
      {tuple<int, int, int, int>( 0, 15,  1, 99)},
      {tuple<int, int, int, int>(13, 28,  2,  1)},
      {tuple<int, int, int, int>(15,  1,  3,  1)},
      {tuple<int, int, int, int>(29, 14,  4, 99)},
      {tuple<int, int, int, int>(15, 28,  5,  1)},
    };

    _wall_manager() {
      all_assigned = false;
      assigned = vector<bool>(wall_count);
      wall_assignment = vector<tuple<int, int> >(m, tuple<int, int>(-1, -1));
    }

    char get_next_movement(int human_id, vector<queue<char> > &move_queue) {
      assert(0 <= human_id && human_id < m);

      if (get<0>(wall_assignment[human_id]) == -1) {
        if (all_assigned) return '.'; // no wall to assign

        // assign to nearest wall
        int nearest_id = -1;
        int nearest_type = -1;
        int nearest_distance = 998244353;
        for (int i = 0; i < wall_count; i++) {
          if (!assigned[i]) {
            for (int j = 0; j < (int) wall_start_info[i].size(); j++) {
              string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(wall_start_info[i][j]), get<1>(wall_start_info[i][j]));
              if (movement == ".") continue;
              if ((int) movement.size() * get<3>(wall_start_info[i][j]) < nearest_distance) {
                nearest_id = i;
                nearest_type = j;
                nearest_distance = (int) movement.size() * get<3>(wall_start_info[i][j]);
              }
            }
          }
        }
        if (nearest_id != -1) {
          wall_assignment[human_id] = tuple<int, int>(nearest_id, nearest_type);
          assigned[nearest_id] = true;
          comment("wall manager: human " + to_string(human_id + 1) + " assigned to wall " + to_string(nearest_id) + ", type " + to_string(nearest_type));

          all_assigned = true;
          for (int i = 0; i < wall_count; i++) {
            if (!assigned[i]) {
              all_assigned = false;
              break;
            }
          }
        }
      }

      if (get<0>(wall_assignment[human_id]) == -1) return '.'; // no wall assigned
      else if (hx[human_id] == get<0>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]) && hy[human_id] == get<1>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])])) {
        comment("wall manager: human " + to_string(human_id + 1) + " reached the start position of wall " + to_string(get<0>(wall_assignment[human_id])) + ", type " + to_string(get<1>(wall_assignment[human_id])));
        for (auto &c: wall_creation_move[get<2>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])])]) {
          move_queue[human_id].push(c);
        }
        wall_assignment[human_id] = tuple<int, int>(-1, -1);
        return '.';
      }
      else {
        string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]), get<1>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]));
        if (!movement.empty()) return movement[0];
        else return '.';
      }
    }
  } wall_manager;

  struct _capture_manager {
    enum state {
      CAPTURE_DOG_STANDBY,
      CAPTURE_DOG_ACTIVE,
      CAPTURE_OTHER_STANDBY,
      CAPTURE_OTHER_ACTIVE,
    };
    state current_state;
    const int block_group_num = 20;
    const vector<vector<int> > detection_group = {
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      {  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      { 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
    };
    const vector<tuple<int, int, char> > block_info = { // human_x, human_y, dir
      tuple<int, int, char>( 14,  1, 'u'),
      tuple<int, int, char>( 14,  4, 'u'),
      tuple<int, int, char>( 14,  7, 'u'),
      tuple<int, int, char>( 14, 10, 'u'),
      tuple<int, int, char>( 14, 13, 'u'),
      tuple<int, int, char>( 14, 16, 'u'),
      tuple<int, int, char>( 14, 19, 'u'),
      tuple<int, int, char>( 14, 22, 'u'),
      tuple<int, int, char>( 14, 25, 'u'),
      tuple<int, int, char>( 14, 28, 'u'),
      tuple<int, int, char>( 14,  1, 'd'),
      tuple<int, int, char>( 14,  4, 'd'),
      tuple<int, int, char>( 14,  7, 'd'),
      tuple<int, int, char>( 14, 10, 'd'),
      tuple<int, int, char>( 14, 13, 'd'),
      tuple<int, int, char>( 14, 16, 'd'),
      tuple<int, int, char>( 14, 19, 'd'),
      tuple<int, int, char>( 14, 22, 'd'),
      tuple<int, int, char>( 14, 25, 'd'),
      tuple<int, int, char>( 14, 28, 'd'),
    };
    vector<tuple<int, int> > block_target; // block_x, block_y (dynamically created)
    vector<int> assignment_for_block;

    _capture_manager() {
      current_state = CAPTURE_OTHER_STANDBY;
      for (int i = 0; i < n; i++) {
        if (pt[i] == 4) {
          current_state = CAPTURE_DOG_STANDBY;
          break;
        }
      }

      block_target = vector<tuple<int, int> >(block_group_num);
      assignment_for_block = vector<int>(m, -1);
      for (int i = 0; i < block_group_num; i++) {
        for (int d = 0; d < 4; d++) {
          if (get<2>(block_info[i]) == delta.b[d]) {
            block_target[i] = tuple<int, int>(get<0>(block_info[i]) + delta.x[d], get<1>(block_info[i]) + delta.y[d]);
            break;
          }
        }
      }
    }

    void update_state() {
      switch (current_state) {
      case CAPTURE_DOG_STANDBY:
        {
          bool everyone_on_target = true;
          for (int i = 0; i < m; i++) {
            if ((i % 2 == 0 && !(hx[i] == 0 && hy[i] == 16)) || (i % 2 != 0 && !(hx[i] == 14 && hy[i] == 16))) {
              everyone_on_target = false;
              break;
            }
          }
          if (everyone_on_target) {
            current_state = CAPTURE_DOG_ACTIVE;
            comment("capture manager: starting dog capture");
          }
        }
        break;
      case CAPTURE_DOG_ACTIVE:
        {
          if (blocked[1][16] && blocked[13][16]) {
            current_state = CAPTURE_OTHER_STANDBY;
            comment("capture manager: dog capture completed");
          }
        }
        break;
      case CAPTURE_OTHER_STANDBY:
        {
          bool is_ready = true;
          for (int i = 0; i < m; i++) {
            if (hx[i] != 14) {
              is_ready = false;
              break;
            }
          }
          if (!blocked[0][14]) is_ready = false;
          if (is_ready) {
            current_state = CAPTURE_OTHER_ACTIVE;
            comment("capture manager: starting non-dog capture");
          }
        }
        break;
      case CAPTURE_OTHER_ACTIVE:
        {
          assignment_for_block = vector<int>(m, -1);

          vector<int> detection_count(block_group_num);
          for (int i = 0; i < n; i++) {
            if (detection_group[px[i]][py[i]] != -1) {
              detection_count[detection_group[px[i]][py[i]]]++;
            }
          }

          vector<tuple<int, int, int> > combination_distance; // distance, human_id, block_group
          for (int i = 0; i < m; i++) {
            for (int j = 0; j < block_group_num; j++) {
              if (blocked[get<0>(block_target[j])][get<1>(block_target[j])]) continue;
              if (!(detection_count[j] >= 3 || (turn_count >= 200 && detection_count[j] >= 2) || (turn_count >= 250 && detection_count[j] >= 1))) continue;
              string movement = shortest_movement(hx[i], hy[i], get<0>(block_info[j]), get<1>(block_info[j]));
              if (movement == ".") continue;
              combination_distance.emplace_back((int) movement.size(), i, j);
            }
          }

          sort(combination_distance.begin(), combination_distance.end());
          vector<bool> block_group_assigned(block_group_num, false);
          for (auto [_, human_id, block_group_id]: combination_distance) {
            if (assignment_for_block[human_id] != -1) continue;
            if (block_group_assigned[block_group_id]) continue;
            assignment_for_block[human_id] = block_group_id;
            block_group_assigned[block_group_id] = true;
          }
        }
        break;
      default:
        break;
      }
    }

    char get_next_movement(int human_id) {
      switch (current_state) {
      case CAPTURE_DOG_STANDBY:
        {
          string movement;
          if (human_id % 2 == 0) movement = shortest_movement(hx[human_id], hy[human_id],  0, 16);
          else                   movement = shortest_movement(hx[human_id], hy[human_id], 14, 16);
          if (!movement.empty()) return movement[0];
          else                   return '.';
        }
        break;
      case CAPTURE_DOG_ACTIVE:
        {
          bool all_dog_capturable = true;
          for (int i = 0; i < n; i++) {
            if (pt[i] == 4) {
              if (!(2 <= px[i] && px[i] <= 12 && 15 <= py[i] && py[i] <= 17)) {
                all_dog_capturable = false;
                break;
              }
            }
          }
          if (!(blockable[1][16] && blockable[13][16])) {
            all_dog_capturable = false;
          }
          if (all_dog_capturable) {
            if (human_id % 2 == 0) return 'd';
            else                   return 'u';
          }
          else return '.';
        }
        break;
      case CAPTURE_OTHER_STANDBY:
        {
          if (!blocked[0][14]) {
            if (hx[human_id] == 0 && hy[human_id] == 13) {
              if (blockable[0][14]) return 'r';
              else return '.';
            }
            else {
              string movement = shortest_movement(hx[human_id], hy[human_id], 0, 13);
              if (!movement.empty()) return movement[0];
              else                   return '.';
            }
          }
          else if (hx[human_id] != 14) {
            string movement = shortest_movement(hx[human_id], hy[human_id], 14, hy[human_id]);
            if (!movement.empty()) return movement[0];
            else                   return '.';
          }
          else return '.';
        }
        break;
      case CAPTURE_OTHER_ACTIVE:
        {
          if (assignment_for_block[human_id] == -1) return '.';

          string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(block_info[assignment_for_block[human_id]]), get<1>(block_info[assignment_for_block[human_id]]));
          if (!movement.empty()) return movement[0];
          else {
            if (blockable[get<0>(block_target[assignment_for_block[human_id]])][get<1>(block_target[assignment_for_block[human_id]])]) return get<2>(block_info[assignment_for_block[human_id]]);
            else return '.';
          }
        }
        break;
      default:
        return '.';
        break;
      }
    }
  } capture_manager;

  while (turn_count < turn_limit) {
    vector<char> human_move(m, '.');

    capture_manager.update_state();

    for (int i = 0; i < m; i++) {
      if (move_queue[i].empty()) {
        human_move[i] = wall_manager.get_next_movement(i, move_queue);
        if (human_move[i] == '.' && move_queue[i].empty() && wall_manager.all_assigned) {
          human_move[i] = capture_manager.get_next_movement(i);
        }
      }

      if (!move_queue[i].empty()) {
        char next_move = move_queue[i].front();
        for (int d = 0; d < 4; d++) {
          if (next_move == delta.b[d]) {
            if (blockable[hx[i] + delta.x[d]][hy[i] + delta.y[d]] && !move_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]]) {
              human_move[i] = next_move;
              move_queue[i].pop();
              break;
            }
          }
          else if (next_move == delta.m[d] && !block_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]]) {
            human_move[i] = next_move;
            move_queue[i].pop();
            break;
          }
          else if (next_move == '.') {
            human_move[i] = next_move;
            move_queue[i].pop();
            break;
          }
        }
        if (move_queue[i].empty()) {
          comment("human " + to_string(i + 1) + ": move queue completed");
        }
      }

      for (int d = 0; d < 4; d++) {
        if (human_move[i] == delta.b[d]) {
          block_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]] = true;
          break;
        }
        else if (human_move[i] == delta.m[d]) {
          move_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]] = true;
          break;
        }
      }
    }

    update(human_move);
    turn_count++;
  }
}

void solve_10x3() {
  vector<queue<char> > move_queue(m);

  struct _wall_manager {
    const vector<string> wall_creation_move = {          // 10x3 cells
      "lUrUUdrUUrUUdrURRRlDrDDurDlDrDDurDl",   // upper walls (leftmost)
      "lUrUUdrUlUrUUdrUlRRRlDrDDurDlDrDDurDl", // upper walls
      "lUrUUdrUlUrUUdrUlRRRlDDDDlDDDDlr",      // upper walls (rightmost)
      "rlDDrDDurDDrDDurDl",                    // center walls (up to down, leftmost)
      "lUrUUdrUUrUUdrUUrl",                    // center walls (down to up, leftmost)
      "rDlDrDDurDlDrDDurDl",                   // center walls (up to down)
      "lUrUUdrUlUrUUdrUlUr",                   // center walls (down to up)
      "rDlDDDDlDDDDlr",                        // center walls (up to down, rightmost)
      "lrUUUUlUUUUlUr",                        // center walls (up to down, rightmost)
      "lDrDDurDDrDDurDRRRlUrUUdrUlUrUUdrUl",   // lower walls (leftmost)
      "lDrDDurDlDrDDurDlRRRlUrUUdrUlUrUUdrUl", // lower walls
      "lDrDDurDlDrDDurDlRRRlUUUUlUUUUlr",      // lower walls (rightmost)
    };
    const int wall_count = 20;
    bool all_assigned;
    vector<bool> assigned;
    vector<tuple<int, int> > wall_assignment;
    const vector<vector<tuple<int, int, int, int> > > wall_start_info = { // sx, sy, move_type, priority mult
      {tuple<int, int, int, int>( 8,  1,  0,  1)},
      {tuple<int, int, int, int>( 8,  7,  1,  2)},
      {tuple<int, int, int, int>( 8, 13,  1,  2)},
      {tuple<int, int, int, int>( 8, 19,  1,  2)},
      {tuple<int, int, int, int>( 8, 25,  2,  1)},
      {tuple<int, int, int, int>(10,  1,  3,  2), tuple<int, int, int, int>(19,  1,  4,  2)},
      {tuple<int, int, int, int>(10,  4,  5,  2), tuple<int, int, int, int>(19,  4,  6,  2)},
      {tuple<int, int, int, int>(10,  7,  5,  2), tuple<int, int, int, int>(19,  7,  6,  2)},
      {tuple<int, int, int, int>(10, 10,  5,  2), tuple<int, int, int, int>(19, 10,  6,  2)},
      {tuple<int, int, int, int>(10, 13,  5,  2), tuple<int, int, int, int>(19, 13,  6,  2)},
      {tuple<int, int, int, int>(10, 16,  5,  2), tuple<int, int, int, int>(19, 16,  6,  2)},
      {tuple<int, int, int, int>(10, 19,  5,  2), tuple<int, int, int, int>(19, 19,  6,  2)},
      {tuple<int, int, int, int>(10, 22,  5,  2), tuple<int, int, int, int>(19, 22,  6,  2)},
      {tuple<int, int, int, int>(10, 25,  5,  2), tuple<int, int, int, int>(19, 25,  6,  2)},
      {tuple<int, int, int, int>(10, 28,  7,  2), tuple<int, int, int, int>(19, 28,  8,  2)},
      {tuple<int, int, int, int>(21,  1,  9,  1)},
      {tuple<int, int, int, int>(21,  7, 10,  2)},
      {tuple<int, int, int, int>(21, 13, 10,  2)},
      {tuple<int, int, int, int>(21, 19, 10,  2)},
      {tuple<int, int, int, int>(21, 25, 11,  1)},
    };

    _wall_manager() {
      all_assigned = false;
      assigned = vector<bool>(wall_count);
      wall_assignment = vector<tuple<int, int> >(m, tuple<int, int>(-1, -1));
    }

    char get_next_movement(int human_id, vector<queue<char> > &move_queue) {
      assert(0 <= human_id && human_id < m);

      if (get<0>(wall_assignment[human_id]) == -1) {
        if (all_assigned) return '.'; // no wall to assign

        // assign to nearest wall
        int nearest_id = -1;
        int nearest_type = -1;
        int nearest_distance = 998244353;
        for (int i = 0; i < wall_count; i++) {
          if (!assigned[i]) {
            for (int j = 0; j < (int) wall_start_info[i].size(); j++) {
              string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(wall_start_info[i][j]), get<1>(wall_start_info[i][j]));
              if (movement == ".") continue;
              if ((int) movement.size() * get<3>(wall_start_info[i][j]) < nearest_distance) {
                nearest_id = i;
                nearest_type = j;
                nearest_distance = (int) movement.size() * get<3>(wall_start_info[i][j]);
              }
            }
          }
        }
        if (nearest_id != -1) {
          wall_assignment[human_id] = tuple<int, int>(nearest_id, nearest_type);
          assigned[nearest_id] = true;
          comment("wall manager: human " + to_string(human_id + 1) + " assigned to wall " + to_string(nearest_id) + ", type " + to_string(nearest_type));

          all_assigned = true;
          for (int i = 0; i < wall_count; i++) {
            if (!assigned[i]) {
              all_assigned = false;
              break;
            }
          }
        }
      }

      if (get<0>(wall_assignment[human_id]) == -1) return '.'; // no wall assigned
      else if (hx[human_id] == get<0>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]) && hy[human_id] == get<1>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])])) {
        comment("wall manager: human " + to_string(human_id + 1) + " reached the start position of wall " + to_string(get<0>(wall_assignment[human_id])) + ", type " + to_string(get<1>(wall_assignment[human_id])));
        for (auto &c: wall_creation_move[get<2>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])])]) {
          move_queue[human_id].push(c);
        }
        wall_assignment[human_id] = tuple<int, int>(-1, -1);
        return '.';
      }
      else {
        string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]), get<1>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]));
        if (!movement.empty()) return movement[0];
        else return '.';
      }
    }
  } wall_manager;

  struct _capture_manager {
    enum state {
      CAPTURE_DOG_STANDBY,
      CAPTURE_DOG_ACTIVE,
      CAPTURE_OTHER_STANDBY,
      CAPTURE_OTHER_ACTIVE,
    };
    state current_state;
    const int block_group_num = 30;
    const int human_group_num = 2;
    const vector<vector<int> > detection_group = {
      { 0,  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      { 0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9,  9},
      { 0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9,  9,  9},
      { 0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9,  9},
      { 0,  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      { 0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9,  9},
      { 0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9,  9,  9},
      { 0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9,  9},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {10, 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      {10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, 19},
      {10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, 19, 19},
      {10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, 19},
      {10, 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      {10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, 19},
      {10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, 19, 19},
      {10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, 19},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, 29},
      {20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, 29, 29},
      {20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, 29},
      {20, 20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29},
      {20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, 29},
      {20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, 29, 29},
      {20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, 29},
      {20, 20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29}
    };
    const vector<vector<tuple<int, int, int, char> > > block_info = { // human_x, human_y, human_group, dir
      {tuple<int, int, int, char>(  9,  1,  0, 'u')},
      {tuple<int, int, int, char>(  9,  4,  0, 'u')},
      {tuple<int, int, int, char>(  9,  7,  0, 'u')},
      {tuple<int, int, int, char>(  9, 10,  0, 'u')},
      {tuple<int, int, int, char>(  9, 13,  0, 'u')},
      {tuple<int, int, int, char>(  9, 16,  0, 'u')},
      {tuple<int, int, int, char>(  9, 19,  0, 'u')},
      {tuple<int, int, int, char>(  9, 22,  0, 'u')},
      {tuple<int, int, int, char>(  9, 25,  0, 'u')},
      {tuple<int, int, int, char>(  9, 28,  0, 'u')},
      {tuple<int, int, int, char>(  9,  1,  0, 'd'), tuple<int, int, int, char>( 20,  1,  1, 'u')},
      {tuple<int, int, int, char>(  9,  4,  0, 'd'), tuple<int, int, int, char>( 20,  4,  1, 'u')},
      {tuple<int, int, int, char>(  9,  7,  0, 'd'), tuple<int, int, int, char>( 20,  7,  1, 'u')},
      {tuple<int, int, int, char>(  9, 10,  0, 'd'), tuple<int, int, int, char>( 20, 10,  1, 'u')},
      {tuple<int, int, int, char>(  9, 13,  0, 'd'), tuple<int, int, int, char>( 20, 13,  1, 'u')},
      {tuple<int, int, int, char>(  9, 16,  0, 'd'), tuple<int, int, int, char>( 20, 16,  1, 'u')},
      {tuple<int, int, int, char>(  9, 19,  0, 'd'), tuple<int, int, int, char>( 20, 19,  1, 'u')},
      {tuple<int, int, int, char>(  9, 22,  0, 'd'), tuple<int, int, int, char>( 20, 22,  1, 'u')},
      {tuple<int, int, int, char>(  9, 25,  0, 'd'), tuple<int, int, int, char>( 20, 25,  1, 'u')},
      {tuple<int, int, int, char>(  9, 28,  0, 'd'), tuple<int, int, int, char>( 20, 28,  1, 'u')},
      {tuple<int, int, int, char>( 20,  1,  1, 'd')},
      {tuple<int, int, int, char>( 20,  4,  1, 'd')},
      {tuple<int, int, int, char>( 20,  7,  1, 'd')},
      {tuple<int, int, int, char>( 20, 10,  1, 'd')},
      {tuple<int, int, int, char>( 20, 13,  1, 'd')},
      {tuple<int, int, int, char>( 20, 16,  1, 'd')},
      {tuple<int, int, int, char>( 20, 19,  1, 'd')},
      {tuple<int, int, int, char>( 20, 22,  1, 'd')},
      {tuple<int, int, int, char>( 20, 25,  1, 'd')},
      {tuple<int, int, int, char>( 20, 28,  1, 'd')},
    };
    vector<vector<tuple<int, int> > > block_target; // block_x, block_y (dynamically created)
    vector<tuple<int, int, char> > assignment_for_block; // target_x, target_y, block_dir

    _capture_manager() {
      current_state = CAPTURE_OTHER_STANDBY;
      for (int i = 0; i < n; i++) {
        if (pt[i] == 4) {
          current_state = CAPTURE_DOG_STANDBY;
          break;
        }
      }

      block_target = vector<vector<tuple<int, int> > >(block_group_num);
      assignment_for_block = vector<tuple<int, int, char> >(m, tuple<int, int, char>(-1, -1, '.'));
      for (int i = 0; i < block_group_num; i++) {
        for (int j = 0; j < (int) block_info[i].size(); j++) {
          for (int d = 0; d < 4; d++) {
            if (get<3>(block_info[i][j]) == delta.b[d]) {
              block_target[i].push_back(tuple<int, int>(get<0>(block_info[i][j]) + delta.x[d], get<1>(block_info[i][j]) + delta.y[d]));
              break;
            }
          }
        }
      }
    }

    void update_state() {
      switch (current_state) {
      case CAPTURE_DOG_STANDBY:
        {
          bool everyone_on_target = true;
          for (int i = 0; i < m; i++) {
            if ((i % 2 == 0 && !(hx[i] == 9 && hy[i] == 13)) || (i % 2 == 1 && !(hx[i] == 20 && hy[i] == 13))) {
              everyone_on_target = false;
              break;
            }
          }
          if (everyone_on_target) {
            current_state = CAPTURE_DOG_ACTIVE;
            comment("capture manager: starting dog capture");
          }
        }
        if (current_state == CAPTURE_DOG_STANDBY) break;
        /* fallthrough */
      case CAPTURE_DOG_ACTIVE:
        {
          if (blocked[10][13] && blocked[19][13]) {
            current_state = CAPTURE_OTHER_STANDBY;
            comment("capture manager: dog capture completed");
          }
        }
        if (current_state == CAPTURE_DOG_ACTIVE) break;
        /* fallthrough */
      case CAPTURE_OTHER_STANDBY:
        {
          bool is_ready = true;
          for (int i = 0; i < m; i++) {
            if (!((i % 2 == 0 && hx[i] == 9) || (i % 2 == 1 && hx[i] == 20))) {
              is_ready = false;
              break;
            }
          }
          if (is_ready) {
            current_state = CAPTURE_OTHER_ACTIVE;
            comment("capture manager: starting non-dog capture");
          }
        }
        if (current_state == CAPTURE_OTHER_STANDBY) break;
        /* fallthrough */
      case CAPTURE_OTHER_ACTIVE:
        {
          assignment_for_block = vector<tuple<int, int, char> >(m, tuple<int, int, char>(-1, -1, '.'));

          vector<bool> detected_group(block_group_num);
          for (int i = 0; i < n; i++) {
            if (detection_group[px[i]][py[i]] != -1 && (shortest_movement(px[i], py[i], 9, 0) != "." || shortest_movement(px[i], py[i], 20, 0) != ".")) {
              detected_group[detection_group[px[i]][py[i]]] = true;
            }
          }

          priority_queue<tuple<int, int, vector<int> >, vector<tuple<int, int, vector<int> > >, greater<tuple<int, int, vector<int> > > > distance_que; // distance, group_id, human_ids
          for (int i = 0; i < block_group_num; i++) {
            if (detected_group[i]) {
              vector<unordered_map<int, int> > distance_list(block_info[i].size()); // [human_group_idx][human_id, distance]
              for (int j = 0; j < (int) block_info[i].size(); j++) {
                for (int k = 0; k < m; k++) {
                  if (k % human_group_num == get<2>(block_info[i][j])) {
                    distance_list[j][k] = abs(get<1>(block_info[i][j]) - hy[k]);
                  }
                }
              }
              auto recursive_enque = [&](auto self, int human_group_idx, int max_distance, vector<int> &human_ids) {
                if (human_group_idx >= (int) block_info[i].size() && !human_ids.empty()) {
                  distance_que.emplace(max_distance, i, human_ids);
                  return;
                }
                for (int j = 0; j < m; j++) {
                  if (distance_list[human_group_idx].find(j) != distance_list[human_group_idx].end()) {
                    human_ids.push_back(j);
                    self(self, human_group_idx + 1, max(max_distance, distance_list[human_group_idx][j]), human_ids);
                    human_ids.pop_back();
                  }
                }
              };
              vector<int> human_ids;
              recursive_enque(recursive_enque, 0, 0, human_ids);
            }
          }

          vector<bool> detection_group_assigned(block_group_num);
          while (!distance_que.empty()) {
            auto [dist, block_group_id, human_ids] = distance_que.top();
            distance_que.pop();
            if (detection_group_assigned[block_group_id]) continue;
            bool all_human_free = true;
            for (auto &h: human_ids) {
              if (get<0>(assignment_for_block[h]) != -1) {
                all_human_free = false;
                break;
              }
            }
            if (!all_human_free) continue;
            for (auto &h: human_ids) {
              for (int i = 0; i < (int) block_info[block_group_id].size(); i++) {
                if (h % human_group_num == get<2>(block_info[block_group_id][i])) {
                  char action_at_target = '.';
                  if (dist == 0) {
                    bool all_blockable = true;
                    for (int j = 0; j < (int) block_target[block_group_id].size(); j++) {
                      if (!blockable[get<0>(block_target[block_group_id][j])][get<1>(block_target[block_group_id][j])]) {
                        all_blockable = false;
                        break;
                      }
                    }
                    if (all_blockable) action_at_target = get<3>(block_info[block_group_id][i]);
                  }
                  assignment_for_block[h] = tuple<int, int, char>(get<0>(block_info[block_group_id][i]), get<1>(block_info[block_group_id][i]), action_at_target);
                  break;
                }
              }
              assert(get<0>(assignment_for_block[h]) != -1);
            }
            detection_group_assigned[block_group_id] = true;
          }

          vector<vector<int> > nearest_pet_distance_per_cell(board_size, vector<int>(board_size, 998244353));
          for (int i = 0; i < n; i++) {
            vector<vector<int> > pet_distance_per_cell(board_size, vector<int>(board_size, 998244353));
            queue<tuple<int, int, int> > que;
            que.emplace(0, px[i], py[i]);
            while (!que.empty()) {
              auto [dist, nx, ny] = que.front();
              que.pop();
              if (pet_distance_per_cell[nx][ny] != 998244353) continue;
              pet_distance_per_cell[nx][ny] = dist;
              for (int d = 0; d < 4; d++) {
                if (within_board(nx + delta.x[d], ny + delta.y[d])) {
                  if (!blocked[nx + delta.x[d]][ny + delta.y[d]] && pet_distance_per_cell[nx + delta.x[d]][ny + delta.y[d]] == 998244353) {
                    que.emplace(dist + 1, nx + delta.x[d], ny + delta.y[d]);
                  }
                }
              }
            }
            for (int s = 0; s < board_size; s++) {
              for (int t = 0; t < board_size; t++) {
                nearest_pet_distance_per_cell[s][t] = min(nearest_pet_distance_per_cell[s][t], pet_distance_per_cell[s][t]);
              }
            }
          }

          vector<int> nearest_pet_distance_per_group(block_group_num, 998244353);
          for (int s = 0; s < board_size; s++) {
            for (int t = 0; t < board_size; t++) {
              if (detection_group[s][t] != -1) {
                nearest_pet_distance_per_group[detection_group[s][t]] = min(nearest_pet_distance_per_group[detection_group[s][t]], nearest_pet_distance_per_cell[s][t]);
              }
            }
          }

          for (int i = 0; i < m; i++) {
            if (get<0>(assignment_for_block[i]) == -1) {
              int shortest_target_x = -1;
              int shortest_target_y = -1;
              int shortest_target_distance = 998244353;
              for (int j = 0; j < block_group_num; j++) {
                for (int k = 0; k < (int) block_info[j].size(); k++) {
                  if (blocked[get<0>(block_target[j][k])][get<1>(block_target[j][k])]) continue;
                  if (i % human_group_num == get<2>(block_info[j][k])) {
                    string movement = shortest_movement(hx[i], hy[i], get<0>(block_info[j][k]), get<1>(block_info[j][k]));
                    if (movement == ".") continue;
                    int dist = max(nearest_pet_distance_per_group[j], (int) movement.size());
                    if (dist < shortest_target_distance) {
                      shortest_target_x = get<0>(block_info[j][k]);
                      shortest_target_y = get<1>(block_info[j][k]);
                      shortest_target_distance = dist;
                    }
                  }
                }
              }
              if (shortest_target_x != -1) {
                assignment_for_block[i] = tuple<int, int, char>(shortest_target_x, shortest_target_y, '.');
              }
            }
          }
        }
        break;
      default:
        break;
      }
    }

    char get_next_movement(int human_id) {
      switch (current_state) {
      case CAPTURE_DOG_STANDBY:
        {
          string movement;
          if (human_id % 2 == 0) movement = shortest_movement(hx[human_id], hy[human_id],  9, 13);
          else                   movement = shortest_movement(hx[human_id], hy[human_id], 20, 13);
          if (!movement.empty()) return movement[0];
          else                   return '.';
        }
        break;
      case CAPTURE_DOG_ACTIVE:
        {
          bool all_dog_capturable = true;
          for (int i = 0; i < n; i++) {
            if (pt[i] == 4) {
              if (detection_group[px[i]][py[i]] != 14) {
                all_dog_capturable = false;
                break;
              }
            }
          }
          if (!(blockable[10][13] && blockable[19][13])) {
            all_dog_capturable = false;
          }
          if (all_dog_capturable) {
            if (human_id % 2 == 0) return 'd';
            else                   return 'u';
          }
          else return '.';
        }
        break;
      case CAPTURE_OTHER_STANDBY:
        {
          int target_x = (human_id % 2 == 0) ? 9 : 20;
          string movement = shortest_movement(hx[human_id], hy[human_id], target_x, hy[human_id]);
          if (!movement.empty()) return movement[0];
          else                   return '.';
        }
        break;
      case CAPTURE_OTHER_ACTIVE:
        {
          if (get<0>(assignment_for_block[human_id]) == -1) return '.';
          if (hx[human_id] == get<0>(assignment_for_block[human_id]) && hy[human_id] == get<1>(assignment_for_block[human_id])) return get<2>(assignment_for_block[human_id]);
          string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(assignment_for_block[human_id]), get<1>(assignment_for_block[human_id]));
          if (!movement.empty()) return movement[0];
          else                   return '.';
        }
        break;
      default:
        return '.';
        break;
      }
    }
  } capture_manager;

  while (turn_count < turn_limit) {
    vector<char> human_move(m, '.');

    // wall_manager.update_state();
    capture_manager.update_state();

    for (int i = 0; i < m; i++) {
      if (move_queue[i].empty()) {
        // human_move[i] = wall_manager.get_next_movement(i);
        human_move[i] = wall_manager.get_next_movement(i, move_queue);
        // if (human_move[i] == '.' && move_queue[i].empty() && wall_manager.wall_remaining == 0) {
        if (human_move[i] == '.' && move_queue[i].empty() && wall_manager.all_assigned) {
          human_move[i] = capture_manager.get_next_movement(i);
        }
      }

      if (!move_queue[i].empty()) {
        char next_move = move_queue[i].front();
        for (int d = 0; d < 4; d++) {
          if (next_move == delta.b[d]) {
            if (blockable[hx[i] + delta.x[d]][hy[i] + delta.y[d]] && !move_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]]) {
              human_move[i] = next_move;
              move_queue[i].pop();
              break;
            }
          }
          else if (next_move == delta.m[d] && !block_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]]) {
            human_move[i] = next_move;
            move_queue[i].pop();
            break;
          }
          else if (next_move == '.') {
            human_move[i] = next_move;
            move_queue[i].pop();
            break;
          }
        }
        if (move_queue[i].empty()) {
          comment("human " + to_string(i + 1) + ": move queue completed");
        }
      }

      for (int d = 0; d < 4; d++) {
        if (human_move[i] == delta.b[d]) {
          block_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]] = true;
          break;
        }
        else if (human_move[i] == delta.m[d]) {
          move_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]] = true;
          break;
        }
      }
    }

    update(human_move);
    turn_count++;
  }
}

void solve_10x5() {
  vector<queue<char> > move_queue(m);

  struct _wall_manager {
    const vector<string> wall_creation_move = {                     // 10x5 cells
      "uRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRRu", // row 1-3 (left to right)
      "uLLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLu", // row 1-3 (right to left)
      "dRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRlu",      // row 1-3 (left to right, top)
      "uLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrd",      // row 1-3 (right to left, top)
      "dRRRldRRRldRRRldRRRldRRRldRRRldRRRldRRRldRRRldRRd",          // row 4-5 (left to right)
      "dLLdLLrLdLLrLdLLrLdLLrLdLLrLdLLrLdLLrLdLLrLdLLrLd",          // row 4-5 (right to left)
      "ldRRRldRRRldRRRldRRRldRRRldRRRldRRRldRRRld",                 // row 4-5 (left to right, bottom)
      "dLLrLdLLrLdLLrLdLLrLdLLrLdLLrLdLLrLdLLrLdl",                 // row 4-5 (right to left, bottom)
      "luRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRlu",   // row 1-3 (left to right, center)
      "uLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLul",   // row 1-3 (right to left, center)
      "dRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRRd", // row 4-6 (left to right, center)
      "dLLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLd", // row 4-6 (right to left, center)
    };
    const int wall_count = 10;
    bool all_assigned;
    vector<bool> assigned;
    vector<tuple<int, int> > wall_assignment;
    const vector<vector<tuple<int, int, int, int> > > wall_start_info = { // sx, sy, move_type, priority mult
      {tuple<int, int, int, int>( 1,  1,  2,  1), tuple<int, int, int, int>( 1, 27,  3,  1)},
      {tuple<int, int, int, int>( 3,  0,  4,  2), tuple<int, int, int, int>( 3, 29,  5,  2)},
      {tuple<int, int, int, int>( 7,  0,  0,  5), tuple<int, int, int, int>( 7, 29,  1,  5)},
      {tuple<int, int, int, int>( 9,  0,  4, 10), tuple<int, int, int, int>( 9, 29,  5, 10)},
      {tuple<int, int, int, int>(13,  1,  8, 25), tuple<int, int, int, int>(13, 28,  9, 25)},
      {tuple<int, int, int, int>(16,  0, 10, 25), tuple<int, int, int, int>(16, 29, 11, 25)},
      {tuple<int, int, int, int>(20,  0,  0, 10), tuple<int, int, int, int>(20, 29,  1, 10)},
      {tuple<int, int, int, int>(22,  0,  4,  5), tuple<int, int, int, int>(22, 29,  5,  5)},
      {tuple<int, int, int, int>(26,  0,  0,  2), tuple<int, int, int, int>(26, 29,  1,  2)},
      {tuple<int, int, int, int>(28,  3,  6,  1), tuple<int, int, int, int>(28, 27,  7,  1)},
    };

    _wall_manager() {
      all_assigned = false;
      assigned = vector<bool>(wall_count);
      wall_assignment = vector<tuple<int, int> >(m, tuple<int, int>(-1, -1));
    }

    char get_next_movement(int human_id, vector<queue<char> > &move_queue) {
      assert(0 <= human_id && human_id < m);

      if (get<0>(wall_assignment[human_id]) == -1) {
        if (all_assigned) return '.'; // no wall to assign

        // assign to nearest wall
        int nearest_id = -1;
        int nearest_type = -1;
        int nearest_distance = 998244353;
        for (int i = 0; i < wall_count; i++) {
          if (!assigned[i]) {
            for (int j = 0; j < (int) wall_start_info[i].size(); j++) {
              string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(wall_start_info[i][j]), get<1>(wall_start_info[i][j]));
              if (movement == ".") continue;
              if ((int) movement.size() * get<3>(wall_start_info[i][j]) < nearest_distance) {
                nearest_id = i;
                nearest_type = j;
                nearest_distance = (int) movement.size() * get<3>(wall_start_info[i][j]);
              }
            }
          }
        }
        if (nearest_id != -1) {
          wall_assignment[human_id] = tuple<int, int>(nearest_id, nearest_type);
          assigned[nearest_id] = true;
          comment("wall manager: human " + to_string(human_id + 1) + " assigned to wall " + to_string(nearest_id) + ", type " + to_string(nearest_type));

          all_assigned = true;
          for (int i = 0; i < wall_count; i++) {
            if (!assigned[i]) {
              all_assigned = false;
              break;
            }
          }
        }
      }

      if (get<0>(wall_assignment[human_id]) == -1) return '.'; // no wall assigned
      else if (hx[human_id] == get<0>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]) && hy[human_id] == get<1>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])])) {
        comment("wall manager: human " + to_string(human_id + 1) + " reached the start position of wall " + to_string(get<0>(wall_assignment[human_id])) + ", type " + to_string(get<1>(wall_assignment[human_id])));
        for (auto &c: wall_creation_move[get<2>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])])]) {
          move_queue[human_id].push(c);
        }
        wall_assignment[human_id] = tuple<int, int>(-1, -1);
        return '.';
      }
      else {
        string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]), get<1>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]));
        if (!movement.empty()) return movement[0];
        else return '.';
      }
    }
  } wall_manager;

  struct _capture_manager {
    enum state {
      CAPTURE_DOG_STANDBY,
      CAPTURE_DOG_ACTIVE,
      CAPTURE_OTHER_STANDBY,
      CAPTURE_OTHER_ACTIVE,
    };
    state current_state;
    const int block_group_num = 50;
    const int human_group_num = 4;
    const vector<vector<int> > detection_group = {
      { 0,  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      { 0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9,  9},
      { 0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9,  9,  9},
      { 0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9,  9},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, 19},
      {10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, 19, 19},
      {10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, 19},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, 20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29},
      {20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, 29},
      {20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, 29, 29},
      {20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, 29},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {30, 30, -1, 31, 31, -1, 32, 32, -1, 33, 33, -1, 34, 34, -1, 35, 35, -1, 36, 36, -1, 37, 37, -1, 38, 38, -1, 39, 39, 39},
      {30, -1, 31, 31, -1, 32, 32, -1, 33, 33, -1, 34, 34, -1, 35, 35, -1, 36, 36, -1, 37, 37, -1, 38, 38, -1, 39, 39, 39, 39},
      {30, 30, -1, 31, 31, -1, 32, 32, -1, 33, 33, -1, 34, 34, -1, 35, 35, -1, 36, 36, -1, 37, 37, -1, 38, 38, -1, 39, 39, 39},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {40, 40, -1, 41, 41, -1, 42, 42, -1, 43, 43, -1, 44, 44, -1, 45, 45, -1, 46, 46, -1, 47, 47, -1, 48, 48, -1, 49, 49, 49},
      {40, -1, 41, 41, -1, 42, 42, -1, 43, 43, -1, 44, 44, -1, 45, 45, -1, 46, 46, -1, 47, 47, -1, 48, 48, -1, 49, 49, 49, 49},
      {40, 40, -1, 41, 41, -1, 42, 42, -1, 43, 43, -1, 44, 44, -1, 45, 45, -1, 46, 46, -1, 47, 47, -1, 48, 48, -1, 49, 49, 49},
      {40, 40, 40, -1, 41, 41, -1, 42, 42, -1, 43, 43, -1, 44, 44, -1, 45, 45, -1, 46, 46, -1, 47, 47, -1, 48, 48, -1, 49, 49}
    };
    const vector<vector<tuple<int, int, int, char> > > block_info = { // human_x, human_y, human_group, dir
      {tuple<int, int, int, char>(  5,  1,  0, 'u')},
      {tuple<int, int, int, char>(  5,  4,  0, 'u')},
      {tuple<int, int, int, char>(  5,  7,  0, 'u')},
      {tuple<int, int, int, char>(  5, 10,  0, 'u')},
      {tuple<int, int, int, char>(  5, 13,  0, 'u')},
      {tuple<int, int, int, char>(  5, 16,  0, 'u')},
      {tuple<int, int, int, char>(  5, 19,  0, 'u')},
      {tuple<int, int, int, char>(  5, 22,  0, 'u')},
      {tuple<int, int, int, char>(  5, 25,  0, 'u')},
      {tuple<int, int, int, char>(  5, 28,  0, 'u')},
      {tuple<int, int, int, char>(  5,  1,  0, 'd'), tuple<int, int, int, char>( 11,  1,  1, 'u')},
      {tuple<int, int, int, char>(  5,  4,  0, 'd'), tuple<int, int, int, char>( 11,  4,  1, 'u')},
      {tuple<int, int, int, char>(  5,  7,  0, 'd'), tuple<int, int, int, char>( 11,  7,  1, 'u')},
      {tuple<int, int, int, char>(  5, 10,  0, 'd'), tuple<int, int, int, char>( 11, 10,  1, 'u')},
      {tuple<int, int, int, char>(  5, 13,  0, 'd'), tuple<int, int, int, char>( 11, 13,  1, 'u')},
      {tuple<int, int, int, char>(  5, 16,  0, 'd'), tuple<int, int, int, char>( 11, 16,  1, 'u')},
      {tuple<int, int, int, char>(  5, 19,  0, 'd'), tuple<int, int, int, char>( 11, 19,  1, 'u')},
      {tuple<int, int, int, char>(  5, 22,  0, 'd'), tuple<int, int, int, char>( 11, 22,  1, 'u')},
      {tuple<int, int, int, char>(  5, 25,  0, 'd'), tuple<int, int, int, char>( 11, 25,  1, 'u')},
      {tuple<int, int, int, char>(  5, 28,  0, 'd'), tuple<int, int, int, char>( 11, 28,  1, 'u')},
      {tuple<int, int, int, char>( 11,  2,  1, 'd'), tuple<int, int, int, char>( 18,  1,  2, 'u')},
      {tuple<int, int, int, char>( 11,  5,  1, 'd'), tuple<int, int, int, char>( 18,  4,  2, 'u')},
      {tuple<int, int, int, char>( 11,  8,  1, 'd'), tuple<int, int, int, char>( 18,  7,  2, 'u')},
      {tuple<int, int, int, char>( 11, 11,  1, 'd'), tuple<int, int, int, char>( 18, 10,  2, 'u')},
      {tuple<int, int, int, char>( 11, 14,  1, 'd'), tuple<int, int, int, char>( 18, 13,  2, 'u')},
      {tuple<int, int, int, char>( 11, 17,  1, 'd'), tuple<int, int, int, char>( 18, 16,  2, 'u')},
      {tuple<int, int, int, char>( 11, 20,  1, 'd'), tuple<int, int, int, char>( 18, 19,  2, 'u')},
      {tuple<int, int, int, char>( 11, 23,  1, 'd'), tuple<int, int, int, char>( 18, 22,  2, 'u')},
      {tuple<int, int, int, char>( 11, 26,  1, 'd'), tuple<int, int, int, char>( 18, 25,  2, 'u')},
      {tuple<int, int, int, char>( 11, 29,  1, 'd'), tuple<int, int, int, char>( 18, 28,  2, 'u')},
      {tuple<int, int, int, char>( 18,  1,  2, 'd'), tuple<int, int, int, char>( 24,  1,  3, 'u')},
      {tuple<int, int, int, char>( 18,  4,  2, 'd'), tuple<int, int, int, char>( 24,  4,  3, 'u')},
      {tuple<int, int, int, char>( 18,  7,  2, 'd'), tuple<int, int, int, char>( 24,  7,  3, 'u')},
      {tuple<int, int, int, char>( 18, 10,  2, 'd'), tuple<int, int, int, char>( 24, 10,  3, 'u')},
      {tuple<int, int, int, char>( 18, 13,  2, 'd'), tuple<int, int, int, char>( 24, 13,  3, 'u')},
      {tuple<int, int, int, char>( 18, 16,  2, 'd'), tuple<int, int, int, char>( 24, 16,  3, 'u')},
      {tuple<int, int, int, char>( 18, 19,  2, 'd'), tuple<int, int, int, char>( 24, 19,  3, 'u')},
      {tuple<int, int, int, char>( 18, 22,  2, 'd'), tuple<int, int, int, char>( 24, 22,  3, 'u')},
      {tuple<int, int, int, char>( 18, 25,  2, 'd'), tuple<int, int, int, char>( 24, 25,  3, 'u')},
      {tuple<int, int, int, char>( 18, 28,  2, 'd'), tuple<int, int, int, char>( 24, 28,  3, 'u')},
      {tuple<int, int, int, char>( 24,  1,  3, 'd')},
      {tuple<int, int, int, char>( 24,  4,  3, 'd')},
      {tuple<int, int, int, char>( 24,  7,  3, 'd')},
      {tuple<int, int, int, char>( 24, 10,  3, 'd')},
      {tuple<int, int, int, char>( 24, 13,  3, 'd')},
      {tuple<int, int, int, char>( 24, 16,  3, 'd')},
      {tuple<int, int, int, char>( 24, 19,  3, 'd')},
      {tuple<int, int, int, char>( 24, 22,  3, 'd')},
      {tuple<int, int, int, char>( 24, 25,  3, 'd')},
      {tuple<int, int, int, char>( 24, 28,  3, 'd')},
    };
    vector<vector<tuple<int, int> > > block_target; // block_x, block_y (dynamically created)
    vector<tuple<int, int, char> > assignment_for_block; // target_x, target_y, block_dir

    _capture_manager() {
      current_state = CAPTURE_OTHER_STANDBY;
      // for (int i = 0; i < n; i++) {
      //   if (pt[i] == 4) {
      //     current_state = CAPTURE_DOG_STANDBY;
      //     break;
      //   }
      // }

      block_target = vector<vector<tuple<int, int> > >(block_group_num);
      assignment_for_block = vector<tuple<int, int, char> >(m, tuple<int, int, char>(-1, -1, '.'));
      for (int i = 0; i < block_group_num; i++) {
        for (int j = 0; j < (int) block_info[i].size(); j++) {
          for (int d = 0; d < 4; d++) {
            if (get<3>(block_info[i][j]) == delta.b[d]) {
              block_target[i].push_back(tuple<int, int>(get<0>(block_info[i][j]) + delta.x[d], get<1>(block_info[i][j]) + delta.y[d]));
              break;
            }
          }
        }
      }
    }

    void update_state() {
      switch (current_state) {
      case CAPTURE_DOG_STANDBY:
        {
          bool everyone_on_target = true;
          for (int i = 0; i < m; i++) {
            if ((i % human_group_num == 0 && !(hx[i] == 11 && hy[i] == 14))
             || (i % human_group_num == 1 && !(hx[i] == 11 && hy[i] == 14))
             || (i % human_group_num == 2 && !(hx[i] == 18 && hy[i] == 13))
             || (i % human_group_num == 3 && !(hx[i] == 18 && hy[i] == 13))) {
              everyone_on_target = false;
              break;
            }
          }
          if (everyone_on_target) {
            current_state = CAPTURE_DOG_ACTIVE;
            comment("capture manager: starting dog capture");
          }
        }
        if (current_state == CAPTURE_DOG_STANDBY) break;
        /* fallthrough */
      case CAPTURE_DOG_ACTIVE:
        {
          if (blocked[12][14] && blocked[17][13]) {
            current_state = CAPTURE_OTHER_STANDBY;
            comment("capture manager: dog capture completed");
          }
        }
        if (current_state == CAPTURE_DOG_ACTIVE) break;
        /* fallthrough */
      case CAPTURE_OTHER_STANDBY:
        {
          bool is_ready = true;
          for (int i = 0; i < m; i++) {
            if ((i % human_group_num == 0 && !(hx[i] == 5))
             || (i % human_group_num == 1 && !(hx[i] == 11))
             || (i % human_group_num == 2 && !(hx[i] == 18))
             || (i % human_group_num == 3 && !(hx[i] == 24))) {
              is_ready = false;
              break;
            }
          }
          if (is_ready) {
            current_state = CAPTURE_OTHER_ACTIVE;
            comment("capture manager: starting non-dog capture");
          }
        }
        if (current_state == CAPTURE_OTHER_STANDBY) break;
        /* fallthrough */
      case CAPTURE_OTHER_ACTIVE:
        {
          assignment_for_block = vector<tuple<int, int, char> >(m, tuple<int, int, char>(-1, -1, '.'));

          vector<bool> detected_group(block_group_num);
          for (int i = 0; i < n; i++) {
            if (detection_group[px[i]][py[i]] != -1 && (
              shortest_movement(px[i], py[i],  5, 0) != "."
           || shortest_movement(px[i], py[i], 11, 0) != "."
           || shortest_movement(px[i], py[i], 18, 0) != "."
           || shortest_movement(px[i], py[i], 24, 0) != "."
            )) {
              detected_group[detection_group[px[i]][py[i]]] = true;
            }
          }

          priority_queue<tuple<int, int, vector<int> >, vector<tuple<int, int, vector<int> > >, greater<tuple<int, int, vector<int> > > > distance_que; // distance, group_id, human_ids
          for (int i = 0; i < block_group_num; i++) {
            if (detected_group[i]) {
              vector<unordered_map<int, int> > distance_list(block_info[i].size()); // [human_group_idx][human_id, distance]
              for (int j = 0; j < (int) block_info[i].size(); j++) {
                for (int k = 0; k < m; k++) {
                  if (k % human_group_num == get<2>(block_info[i][j])) {
                    distance_list[j][k] = abs(get<1>(block_info[i][j]) - hy[k]);
                  }
                }
              }
              auto recursive_enque = [&](auto self, int human_group_idx, int max_distance, vector<int> &human_ids) {
                if (human_group_idx >= (int) block_info[i].size() && !human_ids.empty()) {
                  distance_que.emplace(max_distance, i, human_ids);
                  return;
                }
                for (int j = 0; j < m; j++) {
                  if (distance_list[human_group_idx].find(j) != distance_list[human_group_idx].end()) {
                    human_ids.push_back(j);
                    self(self, human_group_idx + 1, max(max_distance, distance_list[human_group_idx][j]), human_ids);
                    human_ids.pop_back();
                  }
                }
              };
              vector<int> human_ids;
              recursive_enque(recursive_enque, 0, 0, human_ids);
            }
          }

          vector<bool> detection_group_assigned(block_group_num);
          while (!distance_que.empty()) {
            auto [dist, block_group_id, human_ids] = distance_que.top();
            distance_que.pop();
            if (detection_group_assigned[block_group_id]) continue;
            bool all_human_free = true;
            for (auto &h: human_ids) {
              if (get<0>(assignment_for_block[h]) != -1) {
                all_human_free = false;
                break;
              }
            }
            if (!all_human_free) continue;
            for (auto &h: human_ids) {
              for (int i = 0; i < (int) block_info[block_group_id].size(); i++) {
                if (h % human_group_num == get<2>(block_info[block_group_id][i])) {
                  char action_at_target = '.';
                  if (dist == 0) {
                    bool all_blockable = true;
                    for (int j = 0; j < (int) block_target[block_group_id].size(); j++) {
                      if (!blockable[get<0>(block_target[block_group_id][j])][get<1>(block_target[block_group_id][j])]) {
                        all_blockable = false;
                        break;
                      }
                    }
                    if (all_blockable) action_at_target = get<3>(block_info[block_group_id][i]);
                  }
                  assignment_for_block[h] = tuple<int, int, char>(get<0>(block_info[block_group_id][i]), get<1>(block_info[block_group_id][i]), action_at_target);
                  break;
                }
              }
              assert(get<0>(assignment_for_block[h]) != -1);
            }
            detection_group_assigned[block_group_id] = true;
          }

          vector<vector<int> > nearest_pet_distance_per_cell(board_size, vector<int>(board_size, 998244353));
          for (int i = 0; i < n; i++) {
            vector<vector<int> > pet_distance_per_cell(board_size, vector<int>(board_size, 998244353));
            queue<tuple<int, int, int> > que;
            que.emplace(0, px[i], py[i]);
            while (!que.empty()) {
              auto [dist, nx, ny] = que.front();
              que.pop();
              if (pet_distance_per_cell[nx][ny] != 998244353) continue;
              pet_distance_per_cell[nx][ny] = dist;
              for (int d = 0; d < 4; d++) {
                if (within_board(nx + delta.x[d], ny + delta.y[d])) {
                  if (!blocked[nx + delta.x[d]][ny + delta.y[d]] && pet_distance_per_cell[nx + delta.x[d]][ny + delta.y[d]] == 998244353) {
                    que.emplace(dist + 1, nx + delta.x[d], ny + delta.y[d]);
                  }
                }
              }
            }
            for (int s = 0; s < board_size; s++) {
              for (int t = 0; t < board_size; t++) {
                nearest_pet_distance_per_cell[s][t] = min(nearest_pet_distance_per_cell[s][t], pet_distance_per_cell[s][t]);
              }
            }
          }

          vector<int> nearest_pet_distance_per_group(block_group_num, 998244353);
          for (int s = 0; s < board_size; s++) {
            for (int t = 0; t < board_size; t++) {
              if (detection_group[s][t] != -1) {
                nearest_pet_distance_per_group[detection_group[s][t]] = min(nearest_pet_distance_per_group[detection_group[s][t]], nearest_pet_distance_per_cell[s][t]);
              }
            }
          }

          for (int i = 0; i < m; i++) {
            if (get<0>(assignment_for_block[i]) == -1) {
              int shortest_target_x = -1;
              int shortest_target_y = -1;
              int shortest_target_distance = 998244353;
              for (int j = 0; j < block_group_num; j++) {
                for (int k = 0; k < (int) block_info[j].size(); k++) {
                  if (blocked[get<0>(block_target[j][k])][get<1>(block_target[j][k])]) continue;
                  if (i % human_group_num == get<2>(block_info[j][k])) {
                    string movement = shortest_movement(hx[i], hy[i], get<0>(block_info[j][k]), get<1>(block_info[j][k]));
                    if (movement == ".") continue;
                    int dist = max(nearest_pet_distance_per_group[j], (int) movement.size());
                    if (dist < shortest_target_distance) {
                      shortest_target_x = get<0>(block_info[j][k]);
                      shortest_target_y = get<1>(block_info[j][k]);
                      shortest_target_distance = dist;
                    }
                  }
                }
              }
              if (shortest_target_x != -1) {
                assignment_for_block[i] = tuple<int, int, char>(shortest_target_x, shortest_target_y, '.');
              }
            }
          }
        }
        break;
      default:
        break;
      }
    }

    char get_next_movement(int human_id) {
      switch (current_state) {
      case CAPTURE_DOG_STANDBY:
        {
          string movement;
               if (human_id % human_group_num == 0) movement = shortest_movement(hx[human_id], hy[human_id], 11, 14);
          else if (human_id % human_group_num == 1) movement = shortest_movement(hx[human_id], hy[human_id], 11, 14);
          else if (human_id % human_group_num == 2) movement = shortest_movement(hx[human_id], hy[human_id], 18, 13);
          else if (human_id % human_group_num == 3) movement = shortest_movement(hx[human_id], hy[human_id], 18, 13);
          if (!movement.empty()) return movement[0];
          else                   return '.';
        }
        break;
      case CAPTURE_DOG_ACTIVE:
        {
          bool all_dog_capturable = true;
          for (int i = 0; i < n; i++) {
            if (pt[i] == 4) {
              if (detection_group[px[i]][py[i]] != 24) {
                all_dog_capturable = false;
                break;
              }
            }
          }
          if (!(blockable[12][14] && blockable[17][13])) {
            all_dog_capturable = false;
          }
          if (all_dog_capturable) {
                 if (human_id % human_group_num == 0) return 'd';
            else if (human_id % human_group_num == 1) return 'd';
            else if (human_id % human_group_num == 2) return 'u';
            else if (human_id % human_group_num == 3) return 'u';
            else return '.';
          }
          else return '.';
        }
        break;
      case CAPTURE_OTHER_STANDBY:
        {
          int target_x = (
            (human_id % human_group_num == 0) ?  5 :
            (human_id % human_group_num == 1) ? 11 :
            (human_id % human_group_num == 2) ? 18 :
            (human_id % human_group_num == 3) ? 24 : 24
          );
          string movement = shortest_movement(hx[human_id], hy[human_id], target_x, hy[human_id]);
          if (!movement.empty()) return movement[0];
          else                   return '.';
        }
        break;
      case CAPTURE_OTHER_ACTIVE:
        {
          if (get<0>(assignment_for_block[human_id]) == -1) return '.';
          if (hx[human_id] == get<0>(assignment_for_block[human_id]) && hy[human_id] == get<1>(assignment_for_block[human_id])) return get<2>(assignment_for_block[human_id]);
          string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(assignment_for_block[human_id]), get<1>(assignment_for_block[human_id]));
          if (!movement.empty()) return movement[0];
          else                   return '.';
        }
        break;
      default:
        return '.';
        break;
      }
    }
  } capture_manager;

  while (turn_count < turn_limit) {
    vector<char> human_move(m, '.');

    capture_manager.update_state();

    for (int i = 0; i < m; i++) {
      if (move_queue[i].empty()) {
        human_move[i] = wall_manager.get_next_movement(i, move_queue);
        if (human_move[i] == '.' && move_queue[i].empty() && wall_manager.all_assigned) {
          human_move[i] = capture_manager.get_next_movement(i);
        }
      }

      if (!move_queue[i].empty()) {
        char next_move = move_queue[i].front();
        for (int d = 0; d < 4; d++) {
          if (next_move == delta.b[d]) {
            if (blockable[hx[i] + delta.x[d]][hy[i] + delta.y[d]] && !move_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]]) {
              human_move[i] = next_move;
              move_queue[i].pop();
              break;
            }
          }
          else if (next_move == delta.m[d] && !block_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]]) {
            human_move[i] = next_move;
            move_queue[i].pop();
            break;
          }
          else if (next_move == '.') {
            human_move[i] = next_move;
            move_queue[i].pop();
            break;
          }
        }
        if (move_queue[i].empty()) {
          comment("human " + to_string(i + 1) + ": move queue completed");
        }
      }

      for (int d = 0; d < 4; d++) {
        if (human_move[i] == delta.b[d]) {
          block_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]] = true;
          break;
        }
        else if (human_move[i] == delta.m[d]) {
          move_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]] = true;
          break;
        }
      }
    }

    update(human_move);
    turn_count++;
  }
}

void solve_10x6() {
  vector<queue<char> > move_queue(m);

  struct _wall_manager {
    const vector<string> wall_creation_move = {                     // 10x6 cells
      "dRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRd",    // row 00-02 (left to right)
      "dLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrd",    // row 00-02 (right to left)
      "udRRRudRRRudRRRudRRRudRRRudRRRudRRRudRRRudRRRud",            // row 03-05 (left to right)
      "udLLLudLLLudLLLudLLLudLLLudLLLudLLLudLLLudLLLud",            // row 03-05 (right to left)
      "ldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRu", // row 06-08 (left to right)
      "uLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdl", // row 06-08 (right to left)
      "luRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRd", // row 10-12 (left to right)
      "dLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLul", // row 10-12 (right to left)
      "udRRRudRRRudRRRudRRRudRRRudRRRudRRRudRRRudRRRud",            // row 13-15 (left to right)
      "udLLLudLLLudLLLudLLLudLLLudLLLudLLLudLLLudLLLud",            // row 13-15 (right to left)
      "ldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRu", // row 16-18 (left to right)
      "uLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdl", // row 16-18 (right to left)
      "uRRRuRRRuRRRuRRRuRRRuRRRuRRRuRRRu",                          // row 19-20 (left to right)
      "uLLLuLLLuLLLuLLLuLLLuLLLuLLLuLLLu",                          // row 19-20 (right to left)
      "luRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRdRRluRd", // row 21-23 (left to right)
      "dLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLuLLrdLul", // row 21-23 (right to left)
      "udRRRudRRRudRRRudRRRudRRRudRRRudRRRudRRRudRRRud",            // row 24-26 (left to right)
      "udLLLudLLLudLLLudLLLudLLLudLLLudLLLudLLLudLLLud",            // row 24-26 (right to left)
      "uRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRuRRldRu",    // row 27-29 (left to right)
      "uLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLruLdLLru",    // row 27-29 (right to left)
    };
    const int wall_count = 10;
    bool all_assigned;
    vector<bool> assigned;
    vector<tuple<int, int> > wall_assignment;
    const vector<vector<tuple<int, int, int, int> > > wall_start_info = { // sx, sy, move_type, priority mult
      {tuple<int, int, int, int>( 1,  2,  0,  1), tuple<int, int, int, int>( 1, 29,  1,  1)},
      {tuple<int, int, int, int>( 4,  1,  2,  2), tuple<int, int, int, int>( 4, 28,  3,  2)},
      {tuple<int, int, int, int>( 7,  1,  4,  5), tuple<int, int, int, int>( 7, 29,  5,  5)},
      {tuple<int, int, int, int>(11,  1,  6, 10), tuple<int, int, int, int>(11, 29,  7, 10)},
      {tuple<int, int, int, int>(14,  1,  8, 25), tuple<int, int, int, int>(14, 28,  9, 25)},
      {tuple<int, int, int, int>(17,  1, 10, 25), tuple<int, int, int, int>(17, 29, 11, 25)},
      {tuple<int, int, int, int>(20,  2, 12, 10), tuple<int, int, int, int>(20, 26, 13, 10)},
      {tuple<int, int, int, int>(22,  1, 14,  5), tuple<int, int, int, int>(22, 29, 15,  5)},
      {tuple<int, int, int, int>(25,  1, 16,  2), tuple<int, int, int, int>(25, 28, 17,  2)},
      {tuple<int, int, int, int>(28,  2, 18,  1), tuple<int, int, int, int>(28, 29, 19,  1)},
    };

    _wall_manager() {
      all_assigned = false;
      assigned = vector<bool>(wall_count);
      wall_assignment = vector<tuple<int, int> >(m, tuple<int, int>(-1, -1));
    }

    char get_next_movement(int human_id, vector<queue<char> > &move_queue) {
      assert(0 <= human_id && human_id < m);

      if (get<0>(wall_assignment[human_id]) == -1) {
        if (all_assigned) return '.'; // no wall to assign

        // assign to nearest wall
        int nearest_id = -1;
        int nearest_type = -1;
        int nearest_distance = 998244353;
        for (int i = 0; i < wall_count; i++) {
          if (!assigned[i]) {
            for (int j = 0; j < (int) wall_start_info[i].size(); j++) {
              string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(wall_start_info[i][j]), get<1>(wall_start_info[i][j]));
              if (movement == ".") continue;
              if ((int) movement.size() * get<3>(wall_start_info[i][j]) < nearest_distance) {
                nearest_id = i;
                nearest_type = j;
                nearest_distance = (int) movement.size() * get<3>(wall_start_info[i][j]);
              }
            }
          }
        }
        if (nearest_id != -1) {
          wall_assignment[human_id] = tuple<int, int>(nearest_id, nearest_type);
          assigned[nearest_id] = true;
          comment("wall manager: human " + to_string(human_id + 1) + " assigned to wall " + to_string(nearest_id) + ", type " + to_string(nearest_type));

          all_assigned = true;
          for (int i = 0; i < wall_count; i++) {
            if (!assigned[i]) {
              all_assigned = false;
              break;
            }
          }
        }
      }

      if (get<0>(wall_assignment[human_id]) == -1) return '.'; // no wall assigned
      else if (hx[human_id] == get<0>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]) && hy[human_id] == get<1>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])])) {
        comment("wall manager: human " + to_string(human_id + 1) + " reached the start position of wall " + to_string(get<0>(wall_assignment[human_id])) + ", type " + to_string(get<1>(wall_assignment[human_id])));
        for (auto &c: wall_creation_move[get<2>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])])]) {
          move_queue[human_id].push(c);
        }
        wall_assignment[human_id] = tuple<int, int>(-1, -1);
        return '.';
      }
      else {
        string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]), get<1>(wall_start_info[get<0>(wall_assignment[human_id])][get<1>(wall_assignment[human_id])]));
        if (!movement.empty()) return movement[0];
        else return '.';
      }
    }
  } wall_manager;

  struct _capture_manager {
    enum state {
      CAPTURE_OTHER_STANDBY,
      CAPTURE_OTHER_ACTIVE,
    };
    state current_state;
    const int block_group_num = 60;
    const int human_group_num = 5;
    const vector<vector<int> > detection_group = {
      { 0,  0,  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9},
      { 0,  0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9},
      { 0,  0, -1,  1,  1, -1,  2,  2, -1,  3,  3, -1,  4,  4, -1,  5,  5, -1,  6,  6, -1,  7,  7, -1,  8,  8, -1,  9,  9, -1},
      { 0, -1, -1,  1, -1, -1,  2, -1, -1,  3, -1, -1,  4, -1, -1,  5, -1, -1,  6, -1, -1,  7, -1, -1,  8, -1, -1,  9, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {10, -1, -1, 11, -1, -1, 12, -1, -1, 13, -1, -1, 14, -1, -1, 15, -1, -1, 16, -1, -1, 17, -1, -1, 18, -1, -1, 19, -1, -1},
      {10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19, -1},
      {-1, 10, 10, -1, 11, 11, -1, 12, 12, -1, 13, 13, -1, 14, 14, -1, 15, 15, -1, 16, 16, -1, 17, 17, -1, 18, 18, -1, 19, 19},
      {-1, -1, 10, -1, -1, 11, -1, -1, 12, -1, -1, 13, -1, -1, 14, -1, -1, 15, -1, -1, 16, -1, -1, 17, -1, -1, 18, -1, -1, 19},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, 20, -1, -1, 21, -1, -1, 22, -1, -1, 23, -1, -1, 24, -1, -1, 25, -1, -1, 26, -1, -1, 27, -1, -1, 28, -1, -1, 29},
      {-1, 20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29},
      {20, 20, -1, 21, 21, -1, 22, 22, -1, 23, 23, -1, 24, 24, -1, 25, 25, -1, 26, 26, -1, 27, 27, -1, 28, 28, -1, 29, 29, -1},
      {20, -1, -1, 21, -1, -1, 22, -1, -1, 23, -1, -1, 24, -1, -1, 25, -1, -1, 26, -1, -1, 27, -1, -1, 28, -1, -1, 29, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {30, -1, -1, 31, -1, -1, 32, -1, -1, 33, -1, -1, 34, -1, -1, 35, -1, -1, 36, -1, -1, 37, -1, -1, 38, -1, -1, 39, -1, -1},
      {30, 30, -1, 31, 31, -1, 32, 32, -1, 33, 33, -1, 34, 34, -1, 35, 35, -1, 36, 36, -1, 37, 37, -1, 38, 38, -1, 39, 39, -1},
      {-1, 30, 30, -1, 31, 31, -1, 32, 32, -1, 33, 33, -1, 34, 34, -1, 35, 35, -1, 36, 36, -1, 37, 37, -1, 38, 38, -1, 39, 39},
      {-1, -1, 30, 30, -1, 31, 31, -1, 32, 32, -1, 33, 33, -1, 34, 34, -1, 35, 35, -1, 36, 36, -1, 37, 37, -1, 38, 38, -1, 39},
      {-1, -1, -1, 30, -1, -1, 31, -1, -1, 32, -1, -1, 33, -1, -1, 34, -1, -1, 35, -1, -1, 36, -1, -1, 37, -1, -1, 38, -1, 39},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, 40, -1, -1, 41, -1, -1, 42, -1, -1, 43, -1, -1, 44, -1, -1, 45, -1, -1, 46, -1, -1, 47, -1, -1, 48, -1, -1, 49},
      {-1, 40, 40, -1, 41, 41, -1, 42, 42, -1, 43, 43, -1, 44, 44, -1, 45, 45, -1, 46, 46, -1, 47, 47, -1, 48, 48, -1, 49, 49},
      {40, 40, -1, 41, 41, -1, 42, 42, -1, 43, 43, -1, 44, 44, -1, 45, 45, -1, 46, 46, -1, 47, 47, -1, 48, 48, -1, 49, 49, -1},
      {40, -1, -1, 41, -1, -1, 42, -1, -1, 43, -1, -1, 44, -1, -1, 45, -1, -1, 46, -1, -1, 47, -1, -1, 48, -1, -1, 49, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      {50, -1, -1, 51, -1, -1, 52, -1, -1, 53, -1, -1, 54, -1, -1, 55, -1, -1, 56, -1, -1, 57, -1, -1, 58, -1, -1, 59, -1, -1},
      {50, 50, -1, 51, 51, -1, 52, 52, -1, 53, 53, -1, 54, 54, -1, 55, 55, -1, 56, 56, -1, 57, 57, -1, 58, 58, -1, 59, 59, -1},
      {50, 50, 50, -1, 51, 51, -1, 52, 52, -1, 53, 53, -1, 54, 54, -1, 55, 55, -1, 56, 56, -1, 57, 57, -1, 58, 58, -1, 59, 59},
      {50, 50, 50, 50, -1, 51, 51, -1, 52, 52, -1, 53, 53, -1, 54, 54, -1, 55, 55, -1, 56, 56, -1, 57, 57, -1, 58, 58, -1, 59}
    };
    const vector<vector<tuple<int, int, int, char> > > block_info = { // human_x, human_y, human_group, dir
      {tuple<int, int, int, char>(  4,  0,  0, 'u')},
      {tuple<int, int, int, char>(  4,  3,  0, 'u')},
      {tuple<int, int, int, char>(  4,  6,  0, 'u')},
      {tuple<int, int, int, char>(  4,  9,  0, 'u')},
      {tuple<int, int, int, char>(  4, 12,  0, 'u')},
      {tuple<int, int, int, char>(  4, 15,  0, 'u')},
      {tuple<int, int, int, char>(  4, 18,  0, 'u')},
      {tuple<int, int, int, char>(  4, 21,  0, 'u')},
      {tuple<int, int, int, char>(  4, 24,  0, 'u')},
      {tuple<int, int, int, char>(  4, 27,  0, 'u')},
      {tuple<int, int, int, char>(  4,  0,  0, 'd'), tuple<int, int, int, char>(  9,  2,  1, 'u')},
      {tuple<int, int, int, char>(  4,  3,  0, 'd'), tuple<int, int, int, char>(  9,  5,  1, 'u')},
      {tuple<int, int, int, char>(  4,  6,  0, 'd'), tuple<int, int, int, char>(  9,  8,  1, 'u')},
      {tuple<int, int, int, char>(  4,  9,  0, 'd'), tuple<int, int, int, char>(  9, 11,  1, 'u')},
      {tuple<int, int, int, char>(  4, 12,  0, 'd'), tuple<int, int, int, char>(  9, 14,  1, 'u')},
      {tuple<int, int, int, char>(  4, 15,  0, 'd'), tuple<int, int, int, char>(  9, 17,  1, 'u')},
      {tuple<int, int, int, char>(  4, 18,  0, 'd'), tuple<int, int, int, char>(  9, 20,  1, 'u')},
      {tuple<int, int, int, char>(  4, 21,  0, 'd'), tuple<int, int, int, char>(  9, 23,  1, 'u')},
      {tuple<int, int, int, char>(  4, 24,  0, 'd'), tuple<int, int, int, char>(  9, 26,  1, 'u')},
      {tuple<int, int, int, char>(  4, 27,  0, 'd'), tuple<int, int, int, char>(  9, 29,  1, 'u')},
      {tuple<int, int, int, char>(  9,  2,  1, 'd'), tuple<int, int, int, char>( 14,  0,  2, 'u')},
      {tuple<int, int, int, char>(  9,  5,  1, 'd'), tuple<int, int, int, char>( 14,  3,  2, 'u')},
      {tuple<int, int, int, char>(  9,  8,  1, 'd'), tuple<int, int, int, char>( 14,  6,  2, 'u')},
      {tuple<int, int, int, char>(  9, 11,  1, 'd'), tuple<int, int, int, char>( 14,  9,  2, 'u')},
      {tuple<int, int, int, char>(  9, 14,  1, 'd'), tuple<int, int, int, char>( 14, 12,  2, 'u')},
      {tuple<int, int, int, char>(  9, 17,  1, 'd'), tuple<int, int, int, char>( 14, 15,  2, 'u')},
      {tuple<int, int, int, char>(  9, 20,  1, 'd'), tuple<int, int, int, char>( 14, 18,  2, 'u')},
      {tuple<int, int, int, char>(  9, 23,  1, 'd'), tuple<int, int, int, char>( 14, 21,  2, 'u')},
      {tuple<int, int, int, char>(  9, 26,  1, 'd'), tuple<int, int, int, char>( 14, 24,  2, 'u')},
      {tuple<int, int, int, char>(  9, 29,  1, 'd'), tuple<int, int, int, char>( 14, 27,  2, 'u')},
      {tuple<int, int, int, char>( 14,  0,  2, 'd'), tuple<int, int, int, char>( 20,  3,  3, 'u')},
      {tuple<int, int, int, char>( 14,  3,  2, 'd'), tuple<int, int, int, char>( 20,  6,  3, 'u')},
      {tuple<int, int, int, char>( 14,  6,  2, 'd'), tuple<int, int, int, char>( 20,  9,  3, 'u')},
      {tuple<int, int, int, char>( 14,  9,  2, 'd'), tuple<int, int, int, char>( 20, 12,  3, 'u')},
      {tuple<int, int, int, char>( 14, 12,  2, 'd'), tuple<int, int, int, char>( 20, 15,  3, 'u')},
      {tuple<int, int, int, char>( 14, 15,  2, 'd'), tuple<int, int, int, char>( 20, 18,  3, 'u')},
      {tuple<int, int, int, char>( 14, 18,  2, 'd'), tuple<int, int, int, char>( 20, 21,  3, 'u')},
      {tuple<int, int, int, char>( 14, 21,  2, 'd'), tuple<int, int, int, char>( 20, 24,  3, 'u')},
      {tuple<int, int, int, char>( 14, 24,  2, 'd'), tuple<int, int, int, char>( 20, 27,  3, 'u')},
      {tuple<int, int, int, char>( 14, 27,  2, 'd'), tuple<int, int, int, char>( 20, 29,  3, 'u')},
      {tuple<int, int, int, char>( 20,  2,  3, 'd'), tuple<int, int, int, char>( 25,  0,  4, 'u')},
      {tuple<int, int, int, char>( 20,  5,  3, 'd'), tuple<int, int, int, char>( 25,  3,  4, 'u')},
      {tuple<int, int, int, char>( 20,  8,  3, 'd'), tuple<int, int, int, char>( 25,  6,  4, 'u')},
      {tuple<int, int, int, char>( 20, 11,  3, 'd'), tuple<int, int, int, char>( 25,  9,  4, 'u')},
      {tuple<int, int, int, char>( 20, 14,  3, 'd'), tuple<int, int, int, char>( 25, 12,  4, 'u')},
      {tuple<int, int, int, char>( 20, 17,  3, 'd'), tuple<int, int, int, char>( 25, 15,  4, 'u')},
      {tuple<int, int, int, char>( 20, 20,  3, 'd'), tuple<int, int, int, char>( 25, 18,  4, 'u')},
      {tuple<int, int, int, char>( 20, 23,  3, 'd'), tuple<int, int, int, char>( 25, 21,  4, 'u')},
      {tuple<int, int, int, char>( 20, 26,  3, 'd'), tuple<int, int, int, char>( 25, 24,  4, 'u')},
      {tuple<int, int, int, char>( 20, 29,  3, 'd'), tuple<int, int, int, char>( 25, 27,  4, 'u')},
      {tuple<int, int, int, char>( 25,  0,  4, 'd')},
      {tuple<int, int, int, char>( 25,  3,  4, 'd')},
      {tuple<int, int, int, char>( 25,  6,  4, 'd')},
      {tuple<int, int, int, char>( 25,  9,  4, 'd')},
      {tuple<int, int, int, char>( 25, 12,  4, 'd')},
      {tuple<int, int, int, char>( 25, 15,  4, 'd')},
      {tuple<int, int, int, char>( 25, 18,  4, 'd')},
      {tuple<int, int, int, char>( 25, 21,  4, 'd')},
      {tuple<int, int, int, char>( 25, 24,  4, 'd')},
      {tuple<int, int, int, char>( 25, 27,  4, 'd')},
    };
    vector<vector<tuple<int, int> > > block_target; // block_x, block_y (dynamically created)
    vector<tuple<int, int, char> > assignment_for_block; // target_x, target_y, block_dir

    _capture_manager() {
      current_state = CAPTURE_OTHER_STANDBY;

      block_target = vector<vector<tuple<int, int> > >(block_group_num);
      assignment_for_block = vector<tuple<int, int, char> >(m, tuple<int, int, char>(-1, -1, '.'));
      for (int i = 0; i < block_group_num; i++) {
        for (int j = 0; j < (int) block_info[i].size(); j++) {
          for (int d = 0; d < 4; d++) {
            if (get<3>(block_info[i][j]) == delta.b[d]) {
              block_target[i].push_back(tuple<int, int>(get<0>(block_info[i][j]) + delta.x[d], get<1>(block_info[i][j]) + delta.y[d]));
              break;
            }
          }
        }
      }
    }

    void update_state() {
      switch (current_state) {
      case CAPTURE_OTHER_STANDBY:
        {
          bool is_ready = true;
          for (int i = 0; i < m; i++) {
            if ((i % human_group_num == 0 && !(hx[i] == 4))
             || (i % human_group_num == 1 && !(hx[i] == 9))
             || (i % human_group_num == 2 && !(hx[i] == 14))
             || (i % human_group_num == 3 && !(hx[i] == 20))
             || (i % human_group_num == 4 && !(hx[i] == 25))) {
              is_ready = false;
              break;
            }
          }
          if (is_ready) {
            current_state = CAPTURE_OTHER_ACTIVE;
            comment("capture manager: starting non-dog capture");
          }
        }
        if (current_state == CAPTURE_OTHER_STANDBY) break;
        /* fallthrough */
      case CAPTURE_OTHER_ACTIVE:
        {
          assignment_for_block = vector<tuple<int, int, char> >(m, tuple<int, int, char>(-1, -1, '.'));

          vector<bool> detected_group(block_group_num);
          for (int i = 0; i < n; i++) {
            if (detection_group[px[i]][py[i]] != -1 && (
              shortest_movement(px[i], py[i],  4, 0) != "."
           || shortest_movement(px[i], py[i],  9, 0) != "."
           || shortest_movement(px[i], py[i], 14, 0) != "."
           || shortest_movement(px[i], py[i], 20, 0) != "."
           || shortest_movement(px[i], py[i], 25, 0) != "."
            )) {
              detected_group[detection_group[px[i]][py[i]]] = true;
            }
          }

          priority_queue<tuple<int, int, vector<int> >, vector<tuple<int, int, vector<int> > >, greater<tuple<int, int, vector<int> > > > distance_que; // distance, group_id, human_ids
          for (int i = 0; i < block_group_num; i++) {
            if (detected_group[i]) {
              vector<unordered_map<int, int> > distance_list(block_info[i].size()); // [human_group_idx][human_id, distance]
              for (int j = 0; j < (int) block_info[i].size(); j++) {
                for (int k = 0; k < m; k++) {
                  if (k % human_group_num == get<2>(block_info[i][j])) {
                    distance_list[j][k] = abs(get<1>(block_info[i][j]) - hy[k]);
                  }
                }
              }
              auto recursive_enque = [&](auto self, int human_group_idx, int max_distance, vector<int> &human_ids) {
                if (human_group_idx >= (int) block_info[i].size() && !human_ids.empty()) {
                  distance_que.emplace(max_distance, i, human_ids);
                  return;
                }
                for (int j = 0; j < m; j++) {
                  if (distance_list[human_group_idx].find(j) != distance_list[human_group_idx].end()) {
                    human_ids.push_back(j);
                    self(self, human_group_idx + 1, max(max_distance, distance_list[human_group_idx][j]), human_ids);
                    human_ids.pop_back();
                  }
                }
              };
              vector<int> human_ids;
              recursive_enque(recursive_enque, 0, 0, human_ids);
            }
          }

          vector<bool> detection_group_assigned(block_group_num);
          while (!distance_que.empty()) {
            auto [dist, block_group_id, human_ids] = distance_que.top();
            distance_que.pop();
            if (detection_group_assigned[block_group_id]) continue;
            bool all_human_free = true;
            for (auto &h: human_ids) {
              if (get<0>(assignment_for_block[h]) != -1) {
                all_human_free = false;
                break;
              }
            }
            if (!all_human_free) continue;
            for (auto &h: human_ids) {
              for (int i = 0; i < (int) block_info[block_group_id].size(); i++) {
                if (h % human_group_num == get<2>(block_info[block_group_id][i])) {
                  char action_at_target = '.';
                  if (dist == 0) {
                    bool all_blockable = true;
                    for (int j = 0; j < (int) block_target[block_group_id].size(); j++) {
                      if (!blockable[get<0>(block_target[block_group_id][j])][get<1>(block_target[block_group_id][j])]) {
                        all_blockable = false;
                        break;
                      }
                    }
                    if (all_blockable) action_at_target = get<3>(block_info[block_group_id][i]);
                  }
                  assignment_for_block[h] = tuple<int, int, char>(get<0>(block_info[block_group_id][i]), get<1>(block_info[block_group_id][i]), action_at_target);
                  break;
                }
              }
              assert(get<0>(assignment_for_block[h]) != -1);
            }
            detection_group_assigned[block_group_id] = true;
          }

          vector<vector<int> > nearest_pet_distance_per_cell(board_size, vector<int>(board_size, 998244353));
          for (int i = 0; i < n; i++) {
            vector<vector<int> > pet_distance_per_cell(board_size, vector<int>(board_size, 998244353));
            queue<tuple<int, int, int> > que;
            que.emplace(0, px[i], py[i]);
            while (!que.empty()) {
              auto [dist, nx, ny] = que.front();
              que.pop();
              if (pet_distance_per_cell[nx][ny] != 998244353) continue;
              pet_distance_per_cell[nx][ny] = dist;
              for (int d = 0; d < 4; d++) {
                if (within_board(nx + delta.x[d], ny + delta.y[d])) {
                  if (!blocked[nx + delta.x[d]][ny + delta.y[d]] && pet_distance_per_cell[nx + delta.x[d]][ny + delta.y[d]] == 998244353) {
                    que.emplace(dist + 1, nx + delta.x[d], ny + delta.y[d]);
                  }
                }
              }
            }
            for (int s = 0; s < board_size; s++) {
              for (int t = 0; t < board_size; t++) {
                nearest_pet_distance_per_cell[s][t] = min(nearest_pet_distance_per_cell[s][t], pet_distance_per_cell[s][t]);
              }
            }
          }

          vector<int> nearest_pet_distance_per_group(block_group_num, 998244353);
          for (int s = 0; s < board_size; s++) {
            for (int t = 0; t < board_size; t++) {
              if (detection_group[s][t] != -1) {
                nearest_pet_distance_per_group[detection_group[s][t]] = min(nearest_pet_distance_per_group[detection_group[s][t]], nearest_pet_distance_per_cell[s][t]);
              }
            }
          }

          for (int i = 0; i < m; i++) {
            if (get<0>(assignment_for_block[i]) == -1) {
              int shortest_target_x = -1;
              int shortest_target_y = -1;
              int shortest_target_distance = 998244353;
              for (int j = 0; j < block_group_num; j++) {
                for (int k = 0; k < (int) block_info[j].size(); k++) {
                  if (blocked[get<0>(block_target[j][k])][get<1>(block_target[j][k])]) continue;
                  if (i % human_group_num == get<2>(block_info[j][k])) {
                    string movement = shortest_movement(hx[i], hy[i], get<0>(block_info[j][k]), get<1>(block_info[j][k]));
                    if (movement == ".") continue;
                    int dist = max(nearest_pet_distance_per_group[j], (int) movement.size());
                    if (dist < shortest_target_distance) {
                      shortest_target_x = get<0>(block_info[j][k]);
                      shortest_target_y = get<1>(block_info[j][k]);
                      shortest_target_distance = dist;
                    }
                  }
                }
              }
              if (shortest_target_x != -1) {
                assignment_for_block[i] = tuple<int, int, char>(shortest_target_x, shortest_target_y, '.');
              }
            }
          }
        }
        break;
      default:
        break;
      }
    }

    char get_next_movement(int human_id) {
      switch (current_state) {
      case CAPTURE_OTHER_STANDBY:
        {
          int target_x = (
            (human_id % human_group_num == 0) ?  4 :
            (human_id % human_group_num == 1) ?  9 :
            (human_id % human_group_num == 2) ? 14 :
            (human_id % human_group_num == 3) ? 20 :
            (human_id % human_group_num == 4) ? 25 : 25
          );
          string movement = shortest_movement(hx[human_id], hy[human_id], target_x, hy[human_id]);
          if (!movement.empty()) return movement[0];
          else                   return '.';
        }
        break;
      case CAPTURE_OTHER_ACTIVE:
        {
          if (get<0>(assignment_for_block[human_id]) == -1) return '.';
          if (hx[human_id] == get<0>(assignment_for_block[human_id]) && hy[human_id] == get<1>(assignment_for_block[human_id])) return get<2>(assignment_for_block[human_id]);
          string movement = shortest_movement(hx[human_id], hy[human_id], get<0>(assignment_for_block[human_id]), get<1>(assignment_for_block[human_id]));
          if (!movement.empty()) return movement[0];
          else                   return '.';
        }
        break;
      default:
        return '.';
        break;
      }
    }
  } capture_manager;

  while (turn_count < turn_limit) {
    vector<char> human_move(m, '.');

    capture_manager.update_state();

    for (int i = 0; i < m; i++) {
      if (move_queue[i].empty()) {
        human_move[i] = wall_manager.get_next_movement(i, move_queue);
        if (human_move[i] == '.' && move_queue[i].empty() && wall_manager.all_assigned) {
          human_move[i] = capture_manager.get_next_movement(i);
        }
      }

      if (!move_queue[i].empty()) {
        char next_move = move_queue[i].front();
        for (int d = 0; d < 4; d++) {
          if (next_move == delta.b[d]) {
            if (blockable[hx[i] + delta.x[d]][hy[i] + delta.y[d]] && !move_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]]) {
              human_move[i] = next_move;
              move_queue[i].pop();
              break;
            }
          }
          else if (next_move == delta.m[d] && !block_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]]) {
            human_move[i] = next_move;
            move_queue[i].pop();
            break;
          }
          else if (next_move == '.') {
            human_move[i] = next_move;
            move_queue[i].pop();
            break;
          }
        }
        if (move_queue[i].empty()) {
          comment("human " + to_string(i + 1) + ": move queue completed");
        }
      }

      for (int d = 0; d < 4; d++) {
        if (human_move[i] == delta.b[d]) {
          block_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]] = true;
          break;
        }
        else if (human_move[i] == delta.m[d]) {
          move_on_next_turn[hx[i] + delta.x[d]][hy[i] + delta.y[d]] = true;
          break;
        }
      }
    }

    update(human_move);
    turn_count++;
  }
}

void solve() {
       if (m == 5 && n >= 18)             solve_10x2();
  else if (m == 5 || (m == 6 && n >= 13)) solve_10x3();
  else if (m <= 7 || (m == 8 && n >= 14)) solve_10x5();
  else                                    solve_10x6();
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  cin >> n;
  px = vector<int>(n);
  py = vector<int>(n);
  pt = vector<int>(n);
  for (int i = 0; i < n; i++) {
    cin >> px[i] >> py[i] >> pt[i];
    px[i]--;
    py[i]--;
  }
  cin >> m;
  hx = vector<int>(m);
  hy = vector<int>(m);
  for (int i = 0; i < m; i++) {
    cin >> hx[i] >> hy[i];
    hx[i]--;
    hy[i]--;
  }

  init();
  solve();

  return 0;
}
