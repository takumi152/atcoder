#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <cmath>

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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<pair<double, double> > xy(n);
  for (auto &x: xy) cin >> x.first >> x.second;

  double px = 0.0;
  double py = 0.0;
  double score = 1e308;

  int iterCount = 0;
  double delta = 100.0;
  double deltaLimit = 1e-6;
  double decayRate = 1e-6;

  while (theTimer.time() < 1.990) {
    double dx = (theRandom.nextDouble() * delta * 2.0) - delta;
    double dy = (theRandom.nextDouble() * delta * 2.0) - delta;
    if (theRandom.nextDouble() < 0.001) dx = 0;
    else if (theRandom.nextDouble() < 0.001) dy = 0;

    px += dx;
    py += dy;

    double maximum = 0.0;
    for (int i = 0; i < n; i++) {
      double d = (px - xy[i].first) * (px - xy[i].first) + (py - xy[i].second) * (py - xy[i].second);
      if (d > maximum) maximum = d;
    }

    //if (iterCount % 100000 == 0) cerr << iterCount << " " << setprecision(16) << px << " " << setprecision(16) << py << " " << setprecision(16) << maximum << " " << setprecision(16) << score << " " << delta << endl;

    if (maximum <= score) {
      score = maximum;
    }
    else {
      px -= dx;
      py -= dy;
    }

    iterCount++;
    if (delta > deltaLimit) delta *= 1.0 - decayRate;
  }

  double r = 0.0;
  for (int i = 0; i < n; i++) {
    double d = sqrt((px - xy[i].first) * (px - xy[i].first) + (py - xy[i].second) * (py - xy[i].second));
    if (d > r) r = d;
  }

  cout << setprecision(16) << r << endl;

  return 0;
}
