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

#define DEBUG

xorshift64 theRandom;
timer theTimer;

constexpr int n = 1000;
constexpr int s = 1000;
constexpr int k = 20;

vector<int> x(n), y(n), c(n);
vector<vector<int> > p(s, vector<int>(k));

vector<vector<bool> > isUsable(n, vector<bool>(n));
vector<vector<int> > edgeCardinality(n, vector<int>(n));

vector<vector<int> > vertexAssignment(s, vector<int>(k));

vector<vector<int> > usedBy(n);

int score, lastScore, bestScore;

int calcScore() {
  int score = 0;
  for (int t = 0; t < s; t++) {
    if (vertexAssignment[t][0] == -1) continue;
    int e = 0;
    for (int i = 0; i < k; i++) {
      for (int j = i+1; j < k; j++) {
        if (p[t][j] == i && edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][j]] == 0) {
          e = 3;
          break;
        }
        else if (p[t][j] != i && edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][j]] > 0) {
          e++;
          break;
        }
      }
      if (e >= 3) break;
    }
    if (e == 0) score += 100;
    else if (e == 1) score += 10;
    else if (e == 2) score += 1;
  }
  return score;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int _n, _s, _k;
  cin >> _n >> _s >> _k;
  for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> c[i];
  for (int i = 0; i < s; i++) {
    for (int j = 1; j < k; j++) {
      cin >> p[i][j];
      p[i][j]--; // zero-indexed
    }
  }

  // check if it's possible to create edge between i and j
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      // sqrt(x^2 + y^2) <= c[i] + c[j]
      // x^2 + y^2 <= (c[i] + c[j])^2
      int dx = x[i] - x[j];
      int dy = y[i] - y[j];
      int cs = c[i] + c[j];
      if (dx * dx + dy * dy <= cs * cs) {
        isUsable[i][j] = true;
        isUsable[j][i] = true;
      }
    }
  }

  // assign vertex
  for (int i = 0; i < s; i++) {
    for (int j = 0; j < k; j++) {
      vertexAssignment[i][j] = -1;
    }
  }

  int score = calcScore();
  int lastScore = score;
  int bestScore = score;

  int iterCount = 0;
  double temperature = 5e0;
  double decayRate = 6e-6;

  double timeLimit = 4.900;
  while (theTimer.time() < timeLimit) {

    int t = theRandom.nextUIntMod(s);
    if (vertexAssignment[t][0] == -1) {
      iterCount++;
      vector<int> v(k);
      int a = 0;
      for (int i = 0; i < k; i++) {
        a = 0;
        int r = theRandom.nextUIntMod(n);
        while (true) {
          if (a >= 100) break;
          a++;
          r = theRandom.nextUIntMod(n);
          if (!isUsable[r][v[p[t][i]]]) continue;
          bool good = true;
          for (int j = 0; j < i; j++) {
            if (j == p[t][i]) continue;
            else if (edgeCardinality[r][v[j]] > 0 || r == v[j]) {
              good = false;
              break;
            }
          }
          int p1 = 0;
          int p2 = 0;
          while (p1 < usedBy[r].size() && p2 < usedBy[v[p[t][i]]].size()) {
            if (usedBy[r][p1] == usedBy[v[p[t][i]]][p2]) {
              good = false;
              break;
            }
            else if (usedBy[r][p1] < usedBy[v[p[t][i]]][p2]) p1++;
            else p2++;
          }
          if (good) break;
        }
        if (a >= 100) break;
        v[i] = r;
      }
      if (a >= 100) continue;

      vertexAssignment[t] = v;
      for (int i = 1; i < k; i++) {
        edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]++;
        edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]++;
      }
      bool ok = true;

      score = calcScore();

      //cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << endl;

      if (score >= lastScore + 100) {
        for (int i = 0; i < k; i++) {
          usedBy[vertexAssignment[t][i]].push_back(t);
          sort(usedBy[vertexAssignment[t][i]].begin(), usedBy[vertexAssignment[t][i]].end());
        }
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else {
        for (int i = 1; i < k; i++) {
          edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]--;
          edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]--;
        }
        vertexAssignment[t][0] = -1;
      }
    }
    else continue;

  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "bestScore   = " << bestScore << endl;

  vector<pair<int, int> > ans;
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      if (edgeCardinality[i][j] > 0) ans.emplace_back(i+1, j+1);
    }
  }

  cout << ans.size() << endl;
  for (auto &x: ans) cout << x.first << " " << x.second << endl;
  for (int i = 0; i < s; i++) {
    if (vertexAssignment[i][0] == -1) {
      for (int j = 0; j < k; j++) cout << j+1 << " ";
    }
    else {
      for (int j = 0; j < k; j++) cout << vertexAssignment[i][j]+1 << " ";
    }
    cout << endl;
  }

  return 0;
}
