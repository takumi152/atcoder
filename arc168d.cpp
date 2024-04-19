#ifdef __GNUC__
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")
#endif

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <random>
#include <functional>
#include <cmath>
#include <cassert>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

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
    #ifdef _MSC_VER
      #ifdef LOCAL
        return __rdtsc() * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return __rdtsc() * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return __rdtsc() * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        return __rdtsc() * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
      #endif
    #else
      unsigned long long l, h;
        __asm__ ("rdtsc" : "=a"(l), "=d"(h));
      #ifdef LOCAL
        return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
      #endif
    #endif
  }
};

using namespace std;

typedef long long int ll;
typedef unsigned long long int ull;
typedef pair<int, int> Pii;
typedef unsigned char uchar;

const ll mod = 998244353;

timer theTimer;
xorshift64 theRandom;
mt19937 theMersenne(1);

int solve(const int l, const int r, vector<vector<int>>& dp, const vector<vector<int>>& overlap_count, const vector<vector<bool>>& allowed_actions) {
  if (dp[l][r] != -1) return dp[l][r];
  if (l >= r) return 0;

  int res = 0;
  // [l, r)を塗る操作を行う場合
  if (allowed_actions[l][r]) {
    for (int e = l; e < r; e++) {
      int left = solve(l, e, dp, overlap_count, allowed_actions);
      int right = solve(e + 1, r, dp, overlap_count, allowed_actions);
      res = max(res, left + right);
    }
    res++;
  }
  // [l, r)を塗る操作を行わない場合
  {
    for (int s = l + 1; s < r; s++) {
      int left = solve(l, s, dp, overlap_count, allowed_actions);
      int right = solve(s, r, dp, overlap_count, allowed_actions);
      res = max(res, left + right);
    }
  }

  dp[l][r] = res;
  return res;
}

int solve_stupid(const int n, const int m, const vector<Pii>& lr) {
  int ans = 0;
  vector<bool> cell;
  vector<int> use_order(m);
  for (int i = 0; i < m; i++) use_order[i] = i;
  do {
    cell.assign(n, false);
    int score = 0;
    for (int i = 0; i < m; i++) {
      auto &[l, r] = lr[use_order[i]];
      bool updated = false;
      for (int j = l - 1; j < r; j++) {
        if (!cell[j]) {
          updated = true;
          cell[j] = true;
        }
      }
      if (updated) score++;
    }
    ans = max(ans, score);
  } while (next_permutation(use_order.begin(), use_order.end()));
  return ans;
}

vector<vector<int>> calc_overlap_count(const int n, const int m, const vector<Pii>& lr) {
  vector<int> l_count(n + 1);
  for (auto &[l, r]: lr) {
    l_count[l - 1]++;
  }

  vector<vector<int>> overlap_count(n + 1, vector<int>(n + 1)); // [l, r)
  for (int b = 0; b < n; b++) {
    int c = 0;
    for (auto &[l, r]: lr) {
      if (l - 1 <= b && b < r) c++;
    }
    for (int e = b + 1; e <= n; e++) {
      overlap_count[b][e] = c;
      c += l_count[e];
    }
  }
  return overlap_count;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  vector<Pii> lr;

  auto get_input = [&](){
    cin >> n >> m;
    lr = vector<Pii>(m);
    for (auto &[l, r]: lr) cin >> l >> r;
  };
  auto generate_input = [&](int _n, int _m) {
    n = _n;
    m = _m;
    lr.clear();
    unordered_set<int> generated_pair_hash;
    while ((int) lr.size() < _m) {
      int l = theRandom.nextUIntMod(_n) + 1;
      int r = theRandom.nextUIntMod(_n) + 1;
      if (l > r) swap(l, r);
      int hash = l << 16 | r;
      if (generated_pair_hash.find(hash) == generated_pair_hash.end()) {
        lr.emplace_back(l, r);
        generated_pair_hash.insert(hash);
      }
    }
  };

  // get_input();

  // vector<vector<bool>> allowed_actions(n + 1, vector<bool>(n + 1, false)); // [l, r)
  // for (auto &[l, r]: lr) {
  //   allowed_actions[l - 1][r] = true;
  // }

  // vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
  // int ans = solve(0, n, dp, allowed_actions);

  // cout << ans << endl;

  int iter_count = 0;
  while (true) {
    generate_input(10, 5);

    vector<vector<int>> overlap_count = calc_overlap_count(n, m, lr);

    vector<vector<bool>> allowed_actions(n + 1, vector<bool>(n + 1, false)); // [l, r)
    for (auto &[l, r]: lr) {
      allowed_actions[l - 1][r] = true;
    }

    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    int ans = solve(0, n, dp, overlap_count, allowed_actions);

    int ans_stupid = solve_stupid(n, m, lr);

    if (ans != ans_stupid) {
      cout << n << " " << m << endl;
      for (auto &[l, r]: lr) {
        cout << l << " " << r << endl;
      }
      cout << "expected = " << ans_stupid << endl;
      cout << "actual   = " << ans << endl;
      for (auto &x: dp) {
        for (auto &y: x) cout << setw(2) << y << " ";
        cout << endl;
      }
      cout << endl;
      for (auto &x: overlap_count) {
        for (auto &y: x) cout << setw(2) << y << " ";
        cout << endl;
      }
      cout << endl;
      assert(false);
    }

    iter_count++;
    if (iter_count % 1000 == 0) cout << iter_count << endl;
  }

  return 0;
}