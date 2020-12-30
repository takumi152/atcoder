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
#include <set>
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

// #define DEBUG

constexpr int n = 20000;

vector<int> a(n+1), b(n*2+1), c(n+1), d(n*2+1);
vector<Pii> pv;
set<Pii> ps;
int a_total = 0, b_total = 0, c_total = 0, d_total = 0;

double calc_score() {
  if (d_total == 0) return 0.0;
  return (double) d_total / (double) max(max(a_total, b_total), c_total);
}

void add_pin(int x, int y) {
  pv.emplace_back(x, y);
  ps.emplace(x, y);

  a[y]++;
  if (a[y] == 1) a_total++;
  b[x-y+n]++;
  if (b[x-y+n] == 1) b_total++;
  c[x]++;
  if (c[x] == 1) c_total++;
  d[x+y]++;
  if (d[x+y] == 1) d_total++;
}

void remove_pin(int i) {
  auto [x, y] = pv[i];
  a[y]--;
  if (a[y] == 0) a_total--;
  b[x-y+n]--;
  if (b[x-y+n] == 0) b_total--;
  c[x]--;
  if (c[x] == 0) c_total--;
  d[x+y]--;
  if (d[x+y] == 0) d_total--;

  ps.erase(pv[i]);
  swap(pv[i], pv[pv.size()-1]);
  pv.pop_back();
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  double score = 0;
  double lastScore = score;
  double bestScore = score;

  double baseTemperature = 1e-4;
  double temperature = baseTemperature;
  double decayRate = 1e-8;
  double timeLimit = 9999.900;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.5) {
      if ((int) pv.size() >= 100000) continue;
      int x = theRandom.nextUIntMod(n);
      int y = theRandom.nextUIntMod(n);
      if (ps.find(Pii(x, y)) != ps.end()) continue;

      add_pin(x, y);

      score = calc_score();

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << " " << pv.size() << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        remove_pin(pv.size()-1);
        score = lastScore;
      }
    }
    else {
      if ((int) pv.size() <= 1000) continue;
      int i = theRandom.nextUIntMod(pv.size());

      Pii pin_before = pv[i];
      remove_pin(i);

      score = calc_score();

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << " " << pv.size() << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        add_pin(pin_before.first, pin_before.second);
        score = lastScore;
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;

    if (score > 10.1) break;
  }

  cout << pv.size() << endl;
  for (auto &[x, y]: pv) cout << x << " " << y << endl;

  return 0;
}
