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
#include <bitset>
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
struct unionfind {
  vector<int> group;
  vector<int> sz;

  unionfind(int n): group(n), sz(n, 1) {
    init();
  }

  void init() {
    for (int i = 0; i < n; i++) group[i] = i;
    for (int i = 0; i < n; i++) sz[i] = 1;
  }

  int root(int x) {
    while (group[x] != x) {
      group[x] = group[group[x]];
      x = group[x];
    }
    return x;
  }

  bool unite(int x, int y) {
    x = root(x);
    y = root(y);
    if (x == y) return false;
    if (sz[x] < sz[y]) swap(x, y);
    sz[x] += sz[y];
    group[y] = x;
    return true;
  }

  bool same(int x, int y) {
    return root(x) == root(y);
  }

  bool size(int x) {
    return sz[root(x)];
  }
};

// constants
constexpr int server_num_per_type = 100;
constexpr int move_limit_base = 100;

// inputs
int board_size;
int server_type_num;
vector<vector<char> > initial_board;

// outputs
vector<tuple<int, int, int, int> > movement;
vector<tuple<int, int, int, int> > connection;

// internals
vector<vector<char> > board_id;

void get_input() {
  cin >> board_size >> server_type_num;
  initial_board = vector<vector<char> >(board_size, vector<char>(board_size));
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      char c;
      cin >> c;
      initial_board[i][j] = c - '0';
    }
  }
}

void init() {
  {
    board_id = vector<vector<char> >(board_size, vector<char>(board_size, -1));
    vector<int> server_count(server_type_num);
    for (int i = 0; i < board_size; i++) {
      for (int j = 0; j < board_size; j++) {
        if (initial_board[i][j] != 0) {
          board_id[i][j] = server_count[initial_board[i][j] - 1];
          server_count[initial_board[i][j] - 1]++;
        }
      }
    }
  }
}

bool is_connection_crossing(const tuple<int, int, int, int> con1, const tuple<int, int, int, int> con2) {
  auto [x11, y11, x12, y12] = con1;
  auto [x21, y21, x22, y22] = con2;
  if ((x11 == x12 && x21 == x22) || (y11 == y12 && y21 == y22)) return false; // same direction
  if (x11 == x12) {
    if (y11 > y12) swap(y11, y12);
    if (x21 > x22) swap(x21, x22);
    return (y11 < y21 && y21 < y12) && (x21 < x11 && x11 < x22);
  }
  else {
    if (x11 > x12) swap(x11, x12);
    if (y21 > y22) swap(y21, y22);
    return (x11 < x21 && x21 < x12) && (y21 < y11 && y11 < y22);
  }
}

bool within_board(int x, int y) {
  return (0 <= x && x < board_size) && (0 <= y && y < board_size);
}

void solve() {
  auto calc_score = [&](const vector<vector<char> > &board_to_calc, const bool update_ans) {
    static vector<unionfind> uf(server_type_num, server_num_per_type);
    for (int i = 0; i < server_type_num; i++) uf[i].init();

    static vector<vector<bool> >
  };
}

void output_ans() {
  cout << movement.size() << endl;
  for (auto &[x1, y1, x2, y2]: movement) cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
  cout << connection.size() << endl;
  for (auto &[x1, y1, x2, y2]: connection) cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  get_input();

  init();
  solve();

  output_ans();

  return 0;
}
