#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("sse4.2")

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <map>
#include <queue>
#include <functional>
#include <numeric>
#include <random>
#include <chrono>
#include <cstring>
#include <climits>
#include <cassert>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned short nextUShort() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUShortMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x000000000000ffff) * mod) >> 48;
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
  timer() {
    restart();
  }
  inline void restart() {
    t = now();
  }
  inline double time() {
    return now() - t;
  }
  inline double now() {
    unsigned long long l, h;
    __asm__ ("rdtsc" : "=a"(l), "=d"(h));
    return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9
  }
};

#define DEBUG

xorshift64 theRandom;
timer theTimer;

constexpr int n = 100;
constexpr int k = 4;
constexpr int q = 240;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int _n, _k, _q;
  cin >> _n >> _k >> _q;
  vector<vector<int> > a(n, vector<int>(n));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }
  vector<Pii> rc(q);
  for (auto &x: rc) cin >> x.first >> x.second;

  vector<vector<int> > board(n, vector<int>(n));
  vector<int> perm({1, 2, 3, 4});
  int bestScore = 0;
  auto bestBoard = board;

  do {
    vector<int> pattern({perm[2], perm[3], perm[0], perm[3], perm[2], perm[1]});
    for (int j = 0; j < n; j++) {
      board[0][j] = pattern[2];
      board[n-1][j] = pattern[5];
      for (int i = 1; i < n-1; i++) {
        board[i][j] = pattern[j % 6];
      }
      if (j % 6 >= 1 && j % 6 <= 3) board[n-2][j] = pattern[0];
      if (j % 6 == 2) board[n-3][j] = pattern[1];
      if (j % 6 >= 4 || j % 6 <= 0) board[1][j] = pattern[3];
      if (j % 6 == 5) board[2][j] = pattern[4];
      if (j == n-1) {
        for (int i = 0; i < n; i++) board[i][j] = pattern[5];
      }
      if (j <= 1) {
        for (int i = 0; i < n; i++) board[i][j] = pattern[5];
      }
    }
    board[97][98] = pattern[2];
    board[98][97] = pattern[1];
    board[98][98] = pattern[2];
    int score = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (a[i][j] == board[i][j]) score++;
      }
    }
    for (int i = 0; i < q; i++) {
      vector<bool> found({true, false, false, false, false});
      bool good = true;
      for (int j = rc[i].second-1; j < rc[i].second-1+k; j++) {
        if (found[board[rc[i].first-1][j]]) {
          good = false;
          break;
        }
        found[board[rc[i].first-1][j]] = true;
      }
      if (good) score += 100;
    }
    if (score > bestScore) {
      bestScore = score;
      bestBoard = board;
    }
  } while (next_permutation(perm.begin(), perm.end()));

  cerr << "bestScore = " << bestScore << endl;

  for (auto &x: bestBoard) {
    for (auto &y: x) cout << y << " ";
    cout << endl;
  }

  return 0;
}
