#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx")

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

xorshift64 theRandom;
timer theTimer;

//#define DEBUG

constexpr int n = 100;
constexpr int m = 300;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int _n, _m;
  cin >> _n >> _m;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  vector<string> board = s;
  vector<vector<vector<int> > > shotPath(n, vector<vector<int> >(n));
  vector<int> blockCount;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == 'Q') {
        vector<int> movy = { 0,  1,  1,  1,  0, -1, -1, -1};
        vector<int> movx = { 1,  1,  0, -1, -1, -1,  0,  1};
        for (int k = 0; k < 8; k++) {
          bool collide = false;
          for (int a = i + movy[k], b = j + movx[k]; 0 <= a && a < n && 0 <= b && b < n; a += movy[k], b += movx[k]) {
            if (board[a][b] == 'X') {
              collide = true;
              break;
            }
            else if (board[a][b] == 'Q') break;
          }
          if (collide) {
            blockCount.push_back(0);
            for (int a = i + movy[k], b = j + movx[k]; 0 <= a && a < n && 0 <= b && b < n; a += movy[k], b += movx[k]) {
              if (board[a][b] == 'X') break;
              shotPath[a][b].push_back(blockCount.size()-1);
            }
          }
        }
      }
    }
  }

  int q1 = blockCount.size();
  vector<bool> nonoverlap(q1, true);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (shotPath[i][j].size() >= 2) {
        for (auto &x: shotPath[i][j]) nonoverlap[x] = false;
      }
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (auto it = shotPath[i][j].begin(); it != shotPath[i][j].end();) {
        if (nonoverlap[*it]) {
          if (blockCount[*it] == 0) {
            board[i][j] = '#';
            blockCount[*it]++;
          }
          shotPath[i][j].erase(it);
        }
        else it++;
      }
      if (shotPath[i][j].size() >= 2) {
        board[i][j] = '#';
        for (auto &x: shotPath[i][j]) {
          blockCount[x]++;
        }
      }
    }
  }

  int score = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == '#') score++;
    }
  }

  auto bestBoard = board;

  int lastScore = score;
  int bestScore = score;

  int iterCount = 0;
  double temperature = 2.5e-1;
  double decayRate = 9.0e-8;

  while (iterCount < 1000000) {
    double roll = theRandom.nextDouble();
    if (roll < 0.5) {
      int i = theRandom.nextUIntMod(100);
      int j = theRandom.nextUIntMod(100);

      if (shotPath[i][j].empty()) continue;
      if (board[i][j] == '#') {
        bool bad = false;
        for (auto &x: shotPath[i][j]) {
          if (blockCount[x] == 1) {
            bad = true;
            break;
          }
        }
        if (bad) continue;
        for (auto &x: shotPath[i][j]) blockCount[x]--;
        board[i][j] = '.';
        score--;
      }
      else {
        for (auto &x: shotPath[i][j]) blockCount[x]++;
        board[i][j] = '#';
        score++;
      }

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << endl;
      #endif

      if (score < lastScore) {
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) {
        lastScore = score;
      }
      else {
        for (auto &x: shotPath[i][j]) blockCount[x]--;
        board[i][j] = '.';
        score--;
      }
    }
    else {
      int i1 = theRandom.nextUIntMod(100);
      int j1 = theRandom.nextUIntMod(100);
      int i2 = theRandom.nextUIntMod(100);
      int j2 = theRandom.nextUIntMod(100);
      if (i1 == i2 && j1 == j2) continue;

      if (shotPath[i1][j1].empty() || shotPath[i2][j2].empty()) continue;
      if (board[i1][j1] == '#') {
        bool bad = false;
        for (auto &x: shotPath[i1][j1]) {
          if (blockCount[x] == 1) {
            bad = true;
            break;
          }
        }
        if (bad) continue;
      }
      if (board[i2][j2] == '#') {
        bool bad = false;
        for (auto &x: shotPath[i2][j2]) {
          if (blockCount[x] == 1) {
            bad = true;
            break;
          }
        }
        if (bad) continue;
      }
      if (board[i1][j1] == '#') {
        for (auto &x: shotPath[i1][j1]) blockCount[x]--;
        board[i1][j1] = '.';
        score--;
      }
      else {
        for (auto &x: shotPath[i1][j1]) blockCount[x]++;
        board[i1][j1] = '#';
        score++;
      }
      if (board[i2][j2] == '#') {
        for (auto &x: shotPath[i2][j2]) blockCount[x]--;
        board[i2][j2] = '.';
        score--;
      }
      else {
        for (auto &x: shotPath[i2][j2]) blockCount[x]++;
        board[i2][j2] = '#';
        score++;
      }

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << endl;
      #endif

      if (score <= lastScore) {
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) {
        lastScore = score;
      }
      else {
        for (auto &x: shotPath[i1][j1]) blockCount[x]--;
        board[i1][j1] = '.';
        score--;
        for (auto &x: shotPath[i2][j2]) blockCount[x]--;
        board[i2][j2] = '.';
        score--;
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  bestBoard = board;

  while (theTimer.time() < 0.993) {
    double roll = theRandom.nextDouble();
    if (roll < 0.5) {
      int i = theRandom.nextUIntMod(100);
      int j = theRandom.nextUIntMod(100);

      if (shotPath[i][j].empty()) continue;
      if (board[i][j] == '#') {
        bool bad = false;
        for (auto &x: shotPath[i][j]) {
          if (blockCount[x] == 1) {
            bad = true;
            break;
          }
        }
        if (bad) continue;
        for (auto &x: shotPath[i][j]) blockCount[x]--;
        board[i][j] = '.';
        score--;
      }
      else {
        for (auto &x: shotPath[i][j]) blockCount[x]++;
        board[i][j] = '#';
        score++;
      }

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << endl;
      #endif

      if (score < lastScore) {
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
          bestBoard = board;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) {
        lastScore = score;
      }
      else {
        for (auto &x: shotPath[i][j]) blockCount[x]--;
        board[i][j] = '.';
        score--;
      }
    }
    else {
      int i1 = theRandom.nextUIntMod(100);
      int j1 = theRandom.nextUIntMod(100);
      int i2 = theRandom.nextUIntMod(100);
      int j2 = theRandom.nextUIntMod(100);
      if (i1 == i2 && j1 == j2) continue;

      if (shotPath[i1][j1].empty() || shotPath[i2][j2].empty()) continue;
      if (board[i1][j1] == '#') {
        bool bad = false;
        for (auto &x: shotPath[i1][j1]) {
          if (blockCount[x] == 1) {
            bad = true;
            break;
          }
        }
        if (bad) continue;
      }
      if (board[i2][j2] == '#') {
        bool bad = false;
        for (auto &x: shotPath[i2][j2]) {
          if (blockCount[x] == 1) {
            bad = true;
            break;
          }
        }
        if (bad) continue;
      }
      if (board[i1][j1] == '#') {
        for (auto &x: shotPath[i1][j1]) blockCount[x]--;
        board[i1][j1] = '.';
        score--;
      }
      else {
        for (auto &x: shotPath[i1][j1]) blockCount[x]++;
        board[i1][j1] = '#';
        score++;
      }
      if (board[i2][j2] == '#') {
        for (auto &x: shotPath[i2][j2]) blockCount[x]--;
        board[i2][j2] = '.';
        score--;
      }
      else {
        for (auto &x: shotPath[i2][j2]) blockCount[x]++;
        board[i2][j2] = '#';
        score++;
      }

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << endl;
      #endif

      if (score <= lastScore) {
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
          bestBoard = board;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) {
        lastScore = score;
      }
      else {
        for (auto &x: shotPath[i1][j1]) blockCount[x]--;
        board[i1][j1] = '.';
        score--;
        for (auto &x: shotPath[i2][j2]) blockCount[x]--;
        board[i2][j2] = '.';
        score--;
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "bestScore   = " << bestScore << endl;

  for (auto &x: board) cout << x << endl;

  return 0;
}
