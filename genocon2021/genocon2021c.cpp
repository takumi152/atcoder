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

// outputs
vector<vector<char> > ans;

vector<vector<char> > multi_align(vector<int> &a, int b) {
  int na = ans[a[0]].size();
  int nb = ans[b].size();

  vector<vector<int> > dp(na+1, vector<int>(nb+1, 1000000007));
  vector<vector<int> > prev_p(na+1, vector<int>(nb+1));
  vector<vector<int> > prev_q(na+1, vector<int>(nb+1));
  dp[0][0] = 0;

  int b_score = a.size();
  for (int i = 0; i < na; i++) {
    int a_score = a.size();
    for (int k = 0; k < (int) a.size(); k++) {
      if (ans[a[k]][i] == '-') a_score--;
    }
    for (int j = 0; j < nb; j++) {
      int ab_score = a.size();
      for (int k = 0; k < (int) a.size(); k++) {
        if (ans[a[k]][i] == ans[b][j]) ab_score--;
      }
      if (dp[i+1][j+1] > dp[i][j] + ab_score) {
        dp[i+1][j+1] = dp[i][j] + ab_score;
        prev_p[i+1][j+1] = i;
        prev_q[i+1][j+1] = j;
      }
      if (dp[i+1][j] > dp[i][j] + a_score) {
        dp[i+1][j] = dp[i][j] + a_score;
        prev_p[i+1][j] = i;
        prev_q[i+1][j] = j;
      }
      if (dp[i][j+1] > dp[i][j] + b_score) {
        dp[i][j+1] = dp[i][j] + b_score;
        prev_p[i][j+1] = i;
        prev_q[i][j+1] = j;
      }
    }
    if (dp[i+1][nb] > dp[i][nb] + a_score) {
      dp[i+1][nb] = dp[i][nb] + a_score;
      prev_p[i+1][nb] = i;
      prev_q[i+1][nb] = nb;
    }
  }
  for (int j = 0; j < nb; j++) {
    if (dp[na][j+1] > dp[na][j] + b_score) {
      dp[na][j+1] = dp[na][j] + b_score;
      prev_p[na][j+1] = na;
      prev_q[na][j+1] = j;
    }
  }

  vector<vector<char> > res(a.size() + 1);
  int p = na;
  int q = nb;
  while (!(p == 0 && q == 0)) {
    if (prev_p[p][q] == p) {
      for (int i = 0; i < (int) a.size(); i++) res[i].push_back('-');
    }
    else {
      for (int i = 0; i < (int) a.size(); i++) res[i].push_back(ans[a[i]][p-1]);
    }
    if (prev_q[p][q] == q) res[res.size()-1].push_back('-');
    else res[res.size()-1].push_back(ans[b][q-1]);
    int pt = p;
    p = prev_p[pt][q];
    q = prev_q[pt][q];
  }

  for (int i = 0; i < (int) res.size(); i++) reverse(res[i].begin(), res[i].end());

  return res;
}

void solve() {
  ans = s;

  vector<int> a = {0};
  for (int i = 1; i < m; i++) {
    auto res = multi_align(a, i);
    a.push_back(i);
    for (int j = 0; j < (int) a.size(); j++) ans[a[j]] = res[j];
  }
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  cin >> m;
  s = vector<vector<char> >(m);
  for (int i = 0; i < m; i++) {
    string t;
    cin >> t;
    for (auto &x: t) s[i].push_back(x);
  }

  solve();

  for (auto &x: ans) {
    for (auto &y: x) cout << y;
    cout << endl;
  }

  return 0;
}
