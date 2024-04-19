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
#include <unordered_map>
#include <bitset>
#include <random>
#include <functional>
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
typedef unsigned long long int ull;
typedef pair<int, int> Pii;
typedef unsigned char uchar;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;
mt19937 theMersenne(1);

struct unionfind {
  vector<int> group;
  vector<int> rank;

  unionfind() {
    unionfind(0);
  }

  unionfind(int n) {
    group = vector<int>(n);
    rank = vector<int>(n);
    for (int i = 0; i < n; i++) group[i] = i;
  }

  int root(int x) {
    if (group[x] == x) return x;
    return group[x] = root(group[x]);
  }

  void unite(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    if (rank[rx] > rank[ry]) group[ry] = rx;
    else if (rank[ry] > rank[rx]) group[rx] = ry;
    else if (rx != ry) {
      group[ry] = rx;
      rank[rx]++;
    }
  }

  bool same(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    return rx == ry;
  }
};

// hyper parameters

// structs

// enums

// constants
constexpr int board_size = 10;

struct {
  const vector<char> c = {'F', 'B', 'L', 'R'};
  const vector<int>  x = { -1,   1,   0,   0};
  const vector<int>  y = {  0,   0,  -1,   1};
} delta;

// inputs
vector<int> candy_type;

// outputs

// internals
int turn_count = 0;
vector<vector<int>> current_board;
char prev_horizontal;
char prev_vertical;

// board weights

bool within_board(int px, int py) {
  return 0 <= px && px < board_size && 0 <= py && py < board_size;
}

void get_next_board_inplace(vector<vector<int>>& board, char dir) {
  switch (dir) {
  case 'F':
    for (int j = 0; j < board_size; j++) {
      int p = 0;
      for (int i = 0; i < board_size; i++) {
        if (board[i][j] != 0) {
          board[p][j] = board[i][j];
          if (p != i) board[i][j] = 0;
          p++;
        }
      }
    }
    break;
  case 'B':
    for (int j = 0; j < board_size; j++) {
      int p = board_size - 1;
      for (int i = board_size - 1; i >= 0; i--) {
        if (board[i][j] != 0) {
          board[p][j] = board[i][j];
          if (p != i) board[i][j] = 0;
          p--;
        }
      }
    }
    break;
  case 'L':
    for (int i = 0; i < board_size; i++) {
      int p = 0;
      for (int j = 0; j < board_size; j++) {
        if (board[i][j] != 0) {
          board[i][p] = board[i][j];
          if (p != j) board[i][j] = 0;
          p++;
        }
      }
    }
    break;
  case 'R':
    for (int i = 0; i < board_size; i++) {
      int p = board_size - 1;
      for (int j = board_size - 1; j >= 0; j--) {
        if (board[i][j] != 0) {
          board[i][p] = board[i][j];
          if (p != j) board[i][j] = 0;
          p--;
        }
      }
    }
    break;
  }
}

vector<vector<int>> get_next_board(vector<vector<int>>& board, char dir) {
  vector<vector<int>> next_board(board_size, vector<int>(board_size));
  switch (dir) {
  case 'F':
    for (int j = 0; j < board_size; j++) {
      int p = 0;
      for (int i = 0; i < board_size; i++) {
        if (board[i][j] != 0) {
          next_board[p][j] = board[i][j];
          p++;
        }
      }
    }
    break;
  case 'B':
    for (int j = 0; j < board_size; j++) {
      int p = board_size - 1;
      for (int i = board_size - 1; i >= 0; i--) {
        if (board[i][j] != 0) {
          next_board[p][j] = board[i][j];
          p--;
        }
      }
    }
    break;
  case 'L':
    for (int i = 0; i < board_size; i++) {
      int p = 0;
      for (int j = 0; j < board_size; j++) {
        if (board[i][j] != 0) {
          next_board[i][p] = board[i][j];
          p++;
        }
      }
    }
    break;
  case 'R':
    for (int i = 0; i < board_size; i++) {
      int p = board_size - 1;
      for (int j = board_size - 1; j >= 0; j--) {
        if (board[i][j] != 0) {
          next_board[i][p] = board[i][j];
          p--;
        }
      }
    }
    break;
  }
  return next_board;
}

void spawn_candy_inplace(vector<vector<int>>& board, int pos, int id) {
  int c = 0;
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      if (board[i][j] == 0) c++;
      if (c == pos) {
        board[i][j] = id;
        return;
      }
    }
  }
}

void get_first_input() {
  candy_type = vector<int>(board_size * board_size);
  for (auto &x: candy_type) cin >> x;
}

void get_next_input() {
  int pos;
  cin >> pos;
  spawn_candy_inplace(current_board, pos, candy_type[turn_count]);
}

void respond(char dir) {
  cout << dir << endl;
  get_next_board_inplace(current_board, dir);
  if (dir == 'F' || dir == 'B') prev_vertical = dir;
  else prev_horizontal = dir;
}

void init() {
  current_board = vector<vector<int>>(board_size, vector<int>(board_size));
}

int calc_score(vector<vector<int>>& board) {
  int max_score = 0;
  if (max_score == 0) {
    vector<int> type_count(3);
    for (int i = 0; i < board_size * board_size; i++) {
      type_count[candy_type[i]-1]++;
    }
    for (int i = 0; i < 3; i++) max_score += type_count[i] * type_count[i];
  }
  unionfind uf(board_size * board_size);
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      if (board[i][j] == 0) continue;
      if (i < board_size - 1 && board[i][j] == board[i+1][j]) uf.unite(i * board_size + j, (i + 1) * board_size + j);
      if (j < board_size - 1 && board[i][j] == board[i][j+1]) uf.unite(i * board_size + j, i * board_size + (j + 1));
    }
  }
  unordered_map<int, int> group_count;
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      if (board[i][j] != 0) group_count[uf.root(i * board_size + j)]++;
    }
  }
  int score = 0;
  for (auto &[_, c]: group_count) score += c * c;
  return 1e6 * ((double) score / (double) max_score);
}

void solve() {
  int dominant_type = 0;
  {
    vector<int> type_count(3);
    for (int i = 0; i < board_size * board_size; i++) {
      type_count[candy_type[i]-1]++;
    }
    auto max_elem = max_element(type_count.begin(), type_count.end());
    dominant_type = (max_elem - type_count.begin()) + 1;
  }
  for (; turn_count < board_size * board_size; turn_count++) {
    get_next_input();

    if (turn_count == board_size * board_size - 1) { // game over
      respond('F');
      continue;
    }

    if (turn_count < 50) {
      if (candy_type[turn_count + 1] == dominant_type) {
        respond('F');
      }
      else if (candy_type[turn_count] == dominant_type) {
        respond('B');
      }
      else {
        auto random_simulate = [&](vector<vector<int>> board) {
          for (int t = turn_count + 1; t < board_size * board_size; t++) {
            int spawn_pos = theRandom.nextUIntMod(board_size * board_size - t) + 1;
            spawn_candy_inplace(board, spawn_pos, candy_type[t]);
            if (candy_type[t + 1] == dominant_type) {
              get_next_board_inplace(board, 'F');
            }
            else if (candy_type[t] == dominant_type) {
              get_next_board_inplace(board, 'B');
            }
            else {
              // double roll = theRandom.nextDouble();
              // if (roll < 0.5) get_next_board_inplace(board, 'L');
              // else get_next_board_inplace(board, 'R');
              if (dominant_type == 2) {
                if (candy_type[t + 1] == 1) get_next_board_inplace(board, 'L');
                else get_next_board_inplace(board, 'R');
              }
              else {
                if (candy_type[t + 1] == 2) get_next_board_inplace(board, 'L');
                else get_next_board_inplace(board, 'R');
              }
            }
          }
          return calc_score(board);
        };
        auto board_l = get_next_board(current_board, 'L');
        auto board_r = get_next_board(current_board, 'R');
        int iter_count = 0;
        ll score_l = 0;
        ll score_r = 0;
        double time_limit = 0.0199 * (turn_count + 1);
        while (theTimer.time() < time_limit) {
          score_l += random_simulate(board_l);
          score_r += random_simulate(board_r);
          iter_count++;
        }
        cerr << "turn = " << setw(2) << turn_count << ", iter_count = " << setw(8) << iter_count << ", score_l = " << setw(7) << (int) ((double) score_l / (double) iter_count) << ", score_r = " << setw(7) << (int) ((double) score_r / (double) iter_count) << endl;
        if (score_l > score_r) respond('L');
        else respond('R');
      }
    }
    else {
      auto random_simulate = [&](vector<vector<int>> board) {
        for (int t = turn_count + 1; t < board_size * board_size; t++) {
          int spawn_pos = theRandom.nextUIntMod(board_size * board_size - t) + 1;
          spawn_candy_inplace(board, spawn_pos, candy_type[t]);
          if (candy_type[t + 1] == dominant_type) {
            get_next_board_inplace(board, 'F');
          }
          else if (candy_type[t] == dominant_type) {
            get_next_board_inplace(board, 'B');
          }
          else {
            // double roll = theRandom.nextDouble();
            // if (roll < 0.5) get_next_board_inplace(board, 'L');
            // else get_next_board_inplace(board, 'R');
            if (dominant_type == 2) {
              if (candy_type[t + 1] == 1) get_next_board_inplace(board, 'L');
              else get_next_board_inplace(board, 'R');
            }
            else {
              if (candy_type[t + 1] == 2) get_next_board_inplace(board, 'L');
              else get_next_board_inplace(board, 'R');
            }
          }
        }
        return calc_score(board);
      };
      auto board_f = get_next_board(current_board, 'F');
      auto board_b = get_next_board(current_board, 'B');
      auto board_l = get_next_board(current_board, 'L');
      auto board_r = get_next_board(current_board, 'R');
      int iter_count = 0;
      ll score_f = 0;
      ll score_b = 0;
      ll score_l = 0;
      ll score_r = 0;
      double time_limit = 0.0199 * (turn_count + 1);
      while (theTimer.time() < time_limit) {
        score_f += random_simulate(board_f);
        score_b += random_simulate(board_b);
        score_l += random_simulate(board_l);
        score_r += random_simulate(board_r);
        iter_count++;
      }
      cerr << "turn = " << setw(2) << turn_count << ", iter_count = " << setw(5) << iter_count << ", score_f = " << setw(6) << (int) ((double) score_f / (double) iter_count) << ", score_b = " << setw(6) << (int) ((double) score_b / (double) iter_count) << ", score_l = " << setw(6) << (int) ((double) score_l / (double) iter_count) << ", score_r = " << setw(6) << (int) ((double) score_r / (double) iter_count) << endl;
      vector<ll> score_all = {score_f, score_b, score_l, score_r};
      auto max_elem = max_element(score_all.begin(), score_all.end()) - score_all.begin();
      if (max_elem == 0) respond('F');
      else if (max_elem == 1) respond('B');
      else if (max_elem == 2) respond('L');
      else respond('R');
    }
  }
  cerr << "score = " << calc_score(current_board) << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  get_first_input();

  init();
  solve();

  return 0;
}
