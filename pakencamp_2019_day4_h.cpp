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

constexpr int n = 300;

vector<string> s(n);
vector<int> placement(n);
vector<int> cardinality(5000);

inline void replace(int p, int x) {
  int old = placement[p];
  placement[p] = x;
  for (int i = 0; i < s[p].length(); i++) {
    if (s[p][i] == '1') {
      cardinality[old+i]--;
      cardinality[x+i]++;
    }
  }
}

inline int calcScore() {
  int score = 0;
  int con = 0;
  for (int i = 0; i < 5000; i++) {
    if (cardinality[i] > 0) {
      con++;
      if (con > score) score = con;
    }
    else {
      con = 0;
    }
  }
  return score;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int _n;
  cin >> _n;
  for (auto &x: s) cin >> x;

  for (int i = 0; i < n; i++) {
    placement[i] = 0;
    for (int j = 0; j < s[i].length(); j++) {
      if (s[i][j] == '1') cardinality[j]++;
    }
  }

  vector<int> bestPlacement = placement;

  int score = calcScore();
  int lastScore = score;
  int bestScore = score;

  int iterCount = 0;
  double temperature = 1.0e0;
  double decayRate = 1.0e-5;

  while (theTimer.time() < 1.980) {
    int p = theRandom.nextUIntMod(300);
    int x = theRandom.nextUIntMod(4900);
    if (placement[p] == x) continue;

    int oldX = placement[p];
    replace(p, x);
    score = calcScore();

    #ifdef DEBUG
    if (iterCount % 1000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << endl;
    #endif

    if (score >= lastScore) {
      lastScore = score;
      if (score > bestScore) {
        bestScore = score;
        bestPlacement = placement;
      }
    }
    else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) {
      lastScore = score;
    }
    else {
      replace(p, oldX);
      score = lastScore;
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "bestScore   = " << bestScore << endl;

  placement = bestPlacement;

  for (int i = 0; i < n; i++) {
    cout << (1000 + placement[i]) << endl;
  }

  return 0;
}
