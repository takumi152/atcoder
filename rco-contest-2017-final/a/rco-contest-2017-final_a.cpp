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
#include <unordered_map>
#include <set>
#include <random>
#include <cmath>
#include <cstdlib>
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
mt19937 theMersenne(1);

const int turn_max = 1000;
int turn_count = 0;

int d, t;
vector<int> c(8);
vector<int> a(8);

void action(string act, vector<int> arg) {
  // cerr << "Turn " << turn_count << endl;
  // cerr << d << " " << t << endl;
  // for (auto &x: c) cerr << x << " ";
  // cerr << endl;
  // for (auto &x: a) cerr << x << " ";
  // cerr << endl;
  //
  // cerr << act;
  // for (auto &x: arg) cerr << " " << x;
  // cerr << endl;

  cout << act;
  for (auto &x: arg) cout << " " << x;
  cout << endl;

  turn_count++;
  if (turn_count == 1000) exit(0);

  cin >> d >> t;
  for (auto &x: c) cin >> x;
  for (auto &x: a) cin >> x;
}

void action(string act) {
  action(act, vector<int>());
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  cin >> d >> t;
  for (auto &x: c) cin >> x;
  for (auto &x: a) cin >> x;

  while (true) {
    for (int i = 0; i < 8; i++) {
      while (c[i] <= 3) action("change", vector<int>({i+1}));
      if (c[i] != a[i]) action("fill", vector<int>({i+1}));
    }

    int best = -1;
    for (int i = 0; i < (1 << 8); i++) {
      int total = 0;
      for (int j = 0; j < 8; j++) {
        if (i & (1 << j)) total += a[j];
      }
      if (total == d && __builtin_popcount(i) < __builtin_popcount(best)) best = i;
    }

    if (best == -1) action("pass");
    else {
      vector<int> tank_to_sell({__builtin_popcount(best)});
      for (int i = 0; i < 8; i++) {
        if (best & (1 << i)) tank_to_sell.push_back(i+1);
      }

      action("sell", tank_to_sell);
    }
  }

  return 0;
}
