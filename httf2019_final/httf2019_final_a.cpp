#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx")

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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

#define DEBUG

constexpr int t = 1000;
constexpr int n = 10;
constexpr int m = 30000;

vector<vector<ll> > target(m, vector<ll>(14));

vector<pair<int, int> > 
vector<vector<ll> > history(t, vector<ll>(11));
vector<bool> questUsed(30000);

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int _t, _n, _m;
  cin >> _t >> _n >> _m;
  for (int i = 0; i < m; i++) {
    target[i][0] = i; // id
    for (int j = 1; j < 14; j++) cin >> target[i][j];
  }

  sort(target.begin(), target.end(), [](auto &a, auto &b){return a[1] < b[1];});

  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 13; j++) cout << target[i][j] << " ";
    cout << endl;
  }

  return 0;
}
