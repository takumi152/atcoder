// WIP

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
#include <deque>
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
  inline unsigned short nextUShortMod(unsigned long long int mod) {
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
    return now() - t;
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

int n;
vector<unsigned long long int> s, t, u, v;
vector<vector<int> > rc, cc;
vector<vector<unsigned long long int> > board;

int calcScore() {
  int score = 0;
  for (int i = 0; i < n; i++) {
    for (int m = 0; m < 64; m++) {
      rc[i][m] = 0;
      cc[i][m] = 0;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int m = 0; m < 64; m++) {
        if (board[i][j] & (1LL << m)) {
          rc[i][m]++;
          cc[i][m]++;
        }
      }
    }
  }
  for (int i = 0; i < n; i++) {
    for (int m = 0; m < 64; m++) {
      if (s[i] == 0 && (u[i] & (1LL << m))) score += n - rc[i][m];
      else if (s[i] == 0 && !(u[i] & (1LL << m)) && rc[i][m] == n) score += 1000;
      else if (s[i] == 1 && !(u[i] & (1LL << m))) score += rc[i][m];
      else if (s[i] == 1 && (u[i] & (1LL << m)) && rc[i][m] == 0) score += 1000;
      if (t[i] == 0 && (v[i] & (1LL << m))) score += n - cc[i][m];
      else if (t[i] == 0 && !(v[i] & (1LL << m)) && cc[i][m] == n) score += 1000;
      else if (t[i] == 1 && !(v[i] & (1LL << m))) score += cc[i][m];
      else if (t[i] == 1 && (v[i] & (1LL << m)) && cc[i][m] == 0) score += 1000;
    }
  }
  return score;
}

int calcScoreDelta(int r, int c, int b) {
  int scoreDelta = 0;
  board[r][c] ^= (1LL << b);
  if (board[r][c] & (1LL << b)) {
    rc[r][b]++;
    cc[c][b]++;
    if (s[r] == 0 && (u[r] & (1LL << b))) scoreDelta--;
    else if (s[r] == 0 && !(u[r] & (1LL << b)) && rc[r][b] == n) scoreDelta += 1000;
    else if (s[r] == 1 && !(u[r] & (1LL << b))) scoreDelta++;
    else if (s[r] == 1 && (u[r] & (1LL << b)) && rc[r][b] == 1) scoreDelta -= 1000;
    if (t[c] == 0 && (v[c] & (1LL << b))) scoreDelta--;
    else if (t[c] == 0 && !(v[c] & (1LL << b)) && cc[c][b] == n) scoreDelta += 1000;
    else if (t[c] == 1 && !(v[c] & (1LL << b))) scoreDelta++;
    else if (t[c] == 1 && (v[c] & (1LL << b)) && cc[c][b] == 1) scoreDelta -= 1000;
  }
  else {
    rc[r][b]--;
    cc[c][b]--;
    if (s[r] == 0 && (u[r] & (1LL << b))) scoreDelta++;
    else if (s[r] == 0 && !(u[r] & (1LL << b)) && rc[r][b] == n-1) scoreDelta -= 1000;
    else if (s[r] == 1 && !(u[r] & (1LL << b))) scoreDelta--;
    else if (s[r] == 1 && (u[r] & (1LL << b)) && rc[r][b] == 0) scoreDelta += 1000;
    if (t[c] == 0 && (v[c] & (1LL << b))) scoreDelta++;
    else if (t[c] == 0 && !(v[c] & (1LL << b)) && cc[c][b] == n-1) scoreDelta -= 1000;
    else if (t[c] == 1 && !(v[c] & (1LL << b))) scoreDelta--;
    else if (t[c] == 1 && (v[c] & (1LL << b)) && cc[c][b] == 0) scoreDelta += 1000;
  }
  return scoreDelta;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  cin >> n;
  s = vector<unsigned long long int>(n);
  t = vector<unsigned long long int>(n);
  u = vector<unsigned long long int>(n);
  v = vector<unsigned long long int>(n);
  for (auto &x: s) cin >> x;
  for (auto &x: t) cin >> x;
  for (auto &x: u) cin >> x;
  for (auto &x: v) cin >> x;

  // preprocess
  rc = vector<vector<int> >(n, vector<int>(64));
  cc = vector<vector<int> >(n, vector<int>(64));
  board = vector<vector<unsigned long long int> >(n, vector<unsigned long long int>(n));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      board[i][j] = u[i];
    }
  }

  // optimization
  int score = calcScore();
  int lastScore = score;
  int bestScore = score;

  double baseTemperature = 1e-100;
  double temperature = baseTemperature;
  double decayRate = 1e-7;
  double timeLimit = 9.900;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {
    int r = theRandom.nextUIntMod(n);
    int c = theRandom.nextUIntMod(n);
    int b = theRandom.nextUIntMod(64);

    score += calcScoreDelta(r, c, b);

    #ifdef DEBUG
    if (iterCount % 1000000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << endl;
    #endif

    if (score <= lastScore) {
      lastScore = score;
      if (score < bestScore) {
        bestScore = score;
        if (bestScore == 0) {
          for (auto &x: board) {
            for (auto &y: x) cout << y << " ";
            cout << endl;
          }
        }
      }
    }
    else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
      lastScore = score;
    }
    else { // rollback
      score += calcScoreDelta(r, c, b);
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "bestScore   = " << bestScore << endl;

  // postprocess & output
  for (auto &x: board) {
    for (auto &y: x) cout << y << " ";
    cout << endl;
  }

  return 0;
}
