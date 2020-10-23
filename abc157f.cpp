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

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;

//#define DEBUG

int n, k;
vector<double> x, y, c;
double px, py;

double calcScore() {
  vector<double> timeNeeded(n);
  for (int i = 0; i < n; i++) timeNeeded[i] = c[i] * sqrt((px - x[i]) * (px - x[i]) + (py - y[i]) * (py - y[i]));
  nth_element(timeNeeded.begin(), timeNeeded.begin() + k - 1, timeNeeded.end());
  return timeNeeded[k-1];
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  cin >> n >> k;
  x = vector<double>(n);
  y = vector<double>(n);
  c = vector<double>(n);
  for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> c[i];

  // preprocessing
  if (k == 1) { // possibly a corner case
    cout << 0.0 << endl;
    return 0;
  }
  priority_queue<vector<double> > pque;
  double bestpx = 0.0;
  double bestpy = 0.0;
  double bestScorePre = 1e308;

  for (int i = -1001; i <= 1001; i += 2) {
    for (int j = -1001; j <= 1001; j += 2) {
      px = 0.5 + i;
      py = 0.5 + j;
      double score = calcScore();
      if (score < bestScorePre) {
        bestpx = px;
        bestpy = py;
        bestScorePre = score;
      }
      pque.push(vector<double>({-score, bestpx, bestpy}));
    }
  }
  for (int i = 0; i < 500000; i++) {
    px = theRandom.nextDouble() * 2000.0 - 1000.0;
    py = theRandom.nextDouble() * 2000.0 - 1000.0;
    double score = calcScore();
    if (score < bestScorePre) {
      bestpx = px;
      bestpy = py;
      bestScorePre = score;
    }
    pque.push(vector<double>({-score, bestpx, bestpy}));
  }
  for (int j = 0; j < 100; j++) {
    auto now = pque.top();
    pque.pop();
    double tx = now[1];
    double ty = now[2];
    double delta = 4.0;
    for (int i = 0; i < 1000; i++) {
      px = tx + theRandom.nextDouble() * delta * 2.0 - delta;
      py = ty + theRandom.nextDouble() * delta * 2.0 - delta;
      double score = calcScore();
      if (score < bestScorePre) {
        bestpx = px;
        bestpy = py;
        bestScorePre = score;
      }
      delta *= 1.0 - 5e-3;
    }
  }
  px = bestpx;
  py = bestpy;

  // optimization
  double score = calcScore();
  double lastScore = score;
  double bestScore = score;

  double baseTemperature = 1e-1;
  double temperature = baseTemperature;
  double decayRate = 5e-6;
  double maxDelta = 1.0;
  double deltaLimit = 0.0001;
  double deltaDecayRate = 5e-6;
  double timeLimit = 1.900;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {
    double dx = theRandom.nextDouble() * maxDelta * 2.0 - maxDelta;
    double dy = theRandom.nextDouble() * maxDelta * 2.0 - maxDelta;
    if (theRandom.nextDouble() < 0.001) dx = 0.0;
    if (theRandom.nextDouble() < 0.001) dy = 0.0;

    px += dx;
    py += dy;

    score = calcScore();

    #ifdef DEBUG
    if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
    #endif

    if (score <= lastScore) {
      lastScore = score;
      if (score < bestScore) {
        bestScore = score;
        bestpx = px;
        bestpy = py;
      }
    }
    else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
      lastScore = score;
    }
    else { // rollback
      px -= dx;
      py -= dy;
    }

    if (theRandom.nextDouble() < 0.0001) {
      px = bestpx;
      py = bestpy;
      lastScore = bestScore;
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
    if (maxDelta > deltaLimit) maxDelta *= 1.0 - deltaDecayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "bestScore   = " << bestScore << endl;

  // postprocess & output
  cout << setprecision(16) << bestScore << endl;

  return 0;
}
