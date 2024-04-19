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

struct index_set {
  vector<int> included;
  vector<int> included_pos;
  vector<int> excluded;
  vector<int> excluded_pos;

  index_set() {};

  index_set(int n) {
    included.reserve(n);
    excluded.reserve(n);

    for (int i = 0; i < n; i++) excluded.push_back(i);

    included_pos = vector<int>(n, -1);
    excluded_pos = vector<int>(n);
    for (int i = 0; i < n; i++) excluded_pos[i] = i;
  }

  void include(int index) {
    assert(0 <= index && index < (int) excluded.size());
    included.push_back(excluded[index]);
    included_pos[excluded[index]] = included.size()-1;
    swap(excluded_pos[excluded[index]], excluded_pos[excluded[excluded.size()-1]]);
    excluded_pos[excluded[index]] = -1;
    swap(excluded[index], excluded[excluded.size()-1]);
    excluded.pop_back();
  }

  void exclude(int index) {
    assert(0 <= index && index < (int) included.size());
    excluded.push_back(included[index]);
    excluded_pos[included[index]] = excluded.size()-1;
    swap(included_pos[included[index]], included_pos[included[included.size()-1]]);
    included_pos[included[index]] = -1;
    swap(included[index], included[included.size()-1]);
    included.pop_back();
  }

  bool is_included(int element) {
    return included_pos[element] != -1;
  }

  bool is_excluded(int element) {
    return excluded_pos[element] != -1;
  }

  bool include_element(int element) {
    assert(0 <= element && element < (int) excluded_pos.size());
    if (is_excluded(element)) {
      include(excluded_pos[element]);
      return true;
    }
    else return false;
  }

  bool exclude_element(int element) {
    assert(0 <= element && element < (int) included_pos.size());
    if (is_included(element)) {
      exclude(included_pos[element]);
      return true;
    }
    else return false;
  }
};

// inputs
int m;
vector<vector<char> > s;

vector<vector<char> > local_align(vector<char> &s, vector<char> &t) {
  int ns = s.size();
  int nt = t.size();

  vector<vector<int> > dp = vector<vector<int> >(ns+1, vector<int>(nt+1, -1000000007));
  vector<vector<Pii> > prev = vector<vector<Pii> >(ns+1, vector<Pii>(nt+1));
  dp[0][0] = 0;
  for (int i = 0; i < ns; i++) {
    for (int j = 0; j < nt; j++) {
      if (s[i] == t[j]) {
        if (dp[i+1][j+1] < dp[i][j] + 1) {
          dp[i+1][j+1] = dp[i][j] + 1;
          prev[i+1][j+1] = Pii(i, j);
        }
      }
      else {
        if (dp[i+1][j+1] < dp[i][j] - 3) {
          dp[i+1][j+1] = dp[i][j] - 3;
          prev[i+1][j+1] = Pii(i, j);
        }
      }
      if (dp[i+1][j] < dp[i][j] - 5) {
        dp[i+1][j] = dp[i][j] - 5;
        prev[i+1][j] = Pii(i, j);
      }
      if (dp[i][j+1] < dp[i][j] - 5) {
        dp[i][j+1] = dp[i][j] - 5;
        prev[i][j+1] = Pii(i, j);
      }
    }
    if (dp[i+1][nt] < dp[i][nt] - 5) {
      dp[i+1][nt] = dp[i][nt] - 5;
      prev[i+1][nt] = Pii(i, nt);
    }
  }
  for (int j = 0; j < nt; j++) {
    if (dp[ns][j+1] < dp[ns][j] - 5) {
      dp[ns][j+1] = dp[ns][j] - 5;
      prev[ns][j+1] = Pii(ns, j);
    }
  }
  // for (auto &x: prev) {
  //   for (auto &y: x) cerr << "(" << y.first << ", " << y.second << "), ";
  //   cerr << endl;
  // }

  vector<vector<char> > res(2);
  Pii p(ns, nt);
  while (p != Pii(0, 0)) {
    if (prev[p.first][p.second].first == p.first) res[0].push_back('-');
    else res[0].push_back(s[p.first-1]);
    if (prev[p.first][p.second].second == p.second) res[1].push_back('-');
    else res[1].push_back(t[p.second-1]);
    p = prev[p.first][p.second];
  }
  reverse(res[0].begin(), res[0].end());
  reverse(res[1].begin(), res[1].end());

  return res;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  m = 2;
  s = vector<vector<char> >(m);
  for (int i = 0; i < m; i++) {
    string t;
    cin >> t;
    for (auto &x: t) s[i].push_back(x);
  }

  auto res = local_align(s[0], s[1]);

  for (auto &x: res) {
    for (auto &y: x) cout << y;
    cout << endl;
  }

  return 0;
}
