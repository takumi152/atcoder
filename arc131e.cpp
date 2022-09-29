#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

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

timer theTimer;
xorshift64 theRandom;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  if (n % 3 == 2) {
    cout << "No" << endl;
    return 0;
  }

  int target = n * (n - 1) / 6;
  vector<bitset<409> > dp(n);
  double time_limit = 2.950;
  while (theTimer.time() < time_limit) {
    vector<bool> chosen1(n);
    int sum1 = 0;
    double skip_chance = theRandom.nextDouble();
    for (int i = n-1; i >= 1; i--) {
      if (sum1 + i > target) continue;
      if (theRandom.nextDouble() < skip_chance) continue;
      chosen1[i] = true;
      sum1 += i;
    }
    if (sum1 != target) continue;

    dp[0] ^= dp[0];
    dp[0][0] = true;
    for (int i = 1; i < n; i++) {
      dp[i] = dp[i-1];
      if (!chosen1[i]) dp[i] |= (dp[i-1] << i);
    }

    if (!dp[n-1][target]) continue;

    vector<bool> chosen2(n);
    int p = target;
    for (int i = n-1; i >= 1; i--) {
      if (!dp[i-1][p]) {
        p -= i;
        chosen2[i] = true;
      }
    }

    cout << "Yes" << endl;
    for (int i = n-1; i >= 1; i--) {
      char o = 'W';
      if (chosen1[i]) o = 'R';
      else if (chosen2[i]) o = 'B';
      for (int j = 0; j < i; j++) cout << o;
      cout << endl;
    }
    return 0;
  }

  cout << "No" << endl;

  return 0;
}
