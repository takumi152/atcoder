#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <fstream>
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

#define DEBUG

constexpr short n = 150;

vector<vector<bool> > board(n, vector<bool>(n));
vector<unordered_set<short> > row(n), col(n);

bool checkPlaceable(short r, short c) {
  for (auto &x: row[r]) {
    for (auto &y: col[c]) {
      if (board[y][x]) return false;
    }
  }
  return true;
}

short countUnplaceable(short r, short c) {
  return row[r].size() * col[c].size();
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input

  // preprocessing
  short score = 0;

  while (true) {
    vector<short> best_ij;
    short best_unplaceable = n * n;
    for (short i = 0; i < n; i++) {
      for (short j = 0; j < n; j++) {
        if (checkPlaceable(i, j)) {
          short unplaceable = countUnplaceable(i, j);
          if (unplaceable < best_unplaceable) {
            best_ij.clear();
            best_unplaceable = unplaceable;
          }
          if (unplaceable <= best_unplaceable) {
            best_ij.push_back(i*n+j);
          }
        }
      }
    }
    if (best_ij.empty()) break;
    short next = theRandom.nextUShortMod(best_ij.size());
    board[best_ij[next]/n][best_ij[next]%n] = true;
    row[best_ij[next]/n].insert(best_ij[next]%n);
    col[best_ij[next]%n].insert(best_ij[next]/n);
    score++;
  }

  // optimization
  short lastScore = score;
  short bestScore = score;

  double baseTemperature = 1e-1;
  double temperature = baseTemperature;
  double decayRate = 5e-8;
  double timeLimit = 9.900;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {
    short r = theRandom.nextUShortMod(n);
    short c = theRandom.nextUShortMod(n);

    if (!board[r][c]) {
      if (checkPlaceable(r, c)) score++;
      else continue;
    }
    else score--;

    #ifdef DEBUG
    if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
    #endif

    if (score >= lastScore) {
      if (!board[r][c]) {
        board[r][c] = true;
        row[r].insert(c);
        col[c].insert(r);
      }
      else {
        board[r][c] = false;
        row[r].erase(c);
        col[c].erase(r);
      }
      lastScore = score;
      if (score > bestScore) {
        bestScore = score;
      }
    }
    else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
      if (!board[r][c]) {
        board[r][c] = true;
        row[r].insert(c);
        col[c].insert(r);
      }
      else {
        board[r][c] = false;
        row[r].erase(c);
        col[c].erase(r);
      }
      lastScore = score;
    }
    else { // rollback
      score = lastScore;
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "score       = " << score << endl;
  cerr << "bestScore   = " << bestScore << endl;

  // postprocess & output
  sort(board.rbegin(), board.rend());
  cout << n << endl;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j]) cout << 'O';
      else cout << '.';
    }
    cout << endl;
  }

  return 0;
}
