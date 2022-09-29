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

int h, w, n;
vector<int> a, b, c, d;

vector<int> assignment_h;
vector<int> assignment_w;
vector<Pii> assignment_n;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  cin >> h >> w >> n;
  a = vector<int>(n);
  b = vector<int>(n);
  c = vector<int>(n);
  d = vector<int>(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i] >> b[i] >> c[i] >> d[i];
    a[i]--;
    b[i]--;
  }

  // preprocessing
  theRandom.x = ((ll) random_device()() << 32) | (ll) random_device()();

  assignment_h = vector<int>(h, -1);
  assignment_w = vector<int>(w, -1);
  assignment_n = vector<Pii>(n, Pii(-1, -1));

  // optimization
  int score = 0;
  for (int i = 0; i < n; i++) {
    if (assignment_n[i].first != -1) score++;
  }

  int lastScore = score;
  int bestScore = score;

  double baseTemperature = 1.0e-1;
  double temperature = baseTemperature;
  double decayRate = 5.0e-8;
  double timeLimit = 1.980;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.50) {
      int p = theRandom.nextUIntMod(n);
      int x = theRandom.nextUIntMod(c[p] - a[p]) + a[p];
      int y = theRandom.nextUIntMod(d[p] - b[p]) + b[p];

      if (assignment_n[p].first == -1) score++;
      if (assignment_h[x] != -1 && assignment_h[x] != p) score--;
      if (assignment_w[y] != -1 && assignment_w[y] != p) score--;
      if (assignment_h[x] == assignment_w[y] && assignment_h[x] != -1 && assignment_h[x] != p) score++;

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= lastScore) {
        if (assignment_n[p].first != -1) {
          assignment_h[assignment_n[p].first] = -1;
          assignment_w[assignment_n[p].second] = -1;
        }
        if (assignment_h[x] != -1) {
          assignment_w[assignment_n[assignment_h[x]].second] = -1;
          assignment_n[assignment_h[x]] = Pii(-1, -1);
          assignment_h[x] = -1;
        }
        if (assignment_w[y] != -1) {
          assignment_h[assignment_n[assignment_w[y]].first] = -1;
          assignment_n[assignment_w[y]] = Pii(-1, -1);
          assignment_w[y] = -1;
        }
        assignment_n[p] = Pii(x, y);
        assignment_h[x] = p;
        assignment_w[y] = p;
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        if (assignment_n[p].first != -1) {
          assignment_h[assignment_n[p].first] = -1;
          assignment_w[assignment_n[p].second] = -1;
        }
        if (assignment_h[x] != -1) {
          assignment_w[assignment_n[assignment_h[x]].second] = -1;
          assignment_n[assignment_h[x]] = Pii(-1, -1);
          assignment_h[x] = -1;
        }
        if (assignment_w[y] != -1) {
          assignment_h[assignment_n[assignment_w[y]].first] = -1;
          assignment_n[assignment_w[y]] = Pii(-1, -1);
          assignment_w[y] = -1;
        }
        assignment_n[p] = Pii(x, y);
        assignment_h[x] = p;
        assignment_w[y] = p;
        lastScore = score;
      }
      else { // rollback
        score = lastScore;
      }
    }
    else if (roll < 0.75) {
      int p = theRandom.nextUIntMod(n);
      if (assignment_n[p].first == -1) continue;
      int x = theRandom.nextUIntMod(c[p] - a[p]) + a[p];
      if (x == assignment_n[p].first) continue;

      if (assignment_h[x] != -1) continue;

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= lastScore) {
        assignment_h[assignment_n[p].first] = -1;
        assignment_h[x] = p;
        assignment_n[p].first = x;
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        assignment_h[assignment_n[p].first] = -1;
        assignment_h[x] = p;
        assignment_n[p].first = x;
        lastScore = score;
      }
      else { // rollback
        score = lastScore;
      }
    }
    else if (roll < 1.00) {
      int p = theRandom.nextUIntMod(n);
      if (assignment_n[p].first == -1) continue;
      int y = theRandom.nextUIntMod(d[p] - b[p]) + b[p];
      if (y == assignment_n[p].second) continue;

      if (assignment_w[y] != -1) continue;

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= lastScore) {
        assignment_w[assignment_n[p].second] = -1;
        assignment_w[y] = p;
        assignment_n[p].second = y;
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        assignment_w[assignment_n[p].second] = -1;
        assignment_w[y] = p;
        assignment_n[p].second = y;
        lastScore = score;
      }
      else { // rollback
        score = lastScore;
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
    // temperature = baseTemperature * ((timeLimit - theTimer.time()) / timeLimit);
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "score       = " << score << endl;
  cerr << "bestScore   = " << bestScore << endl;

  // postprocess & output
  cout << bestScore << endl;

  return 0;
}
